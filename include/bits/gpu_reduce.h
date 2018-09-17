/*
Copyright 2018 Gordon Brown

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef __GPU_REDUCE_H__
#define __GPU_REDUCE_H__

#include <functional>
#include <iterator>
#include <thread>
#include <utility>
#include <vector>

#include <CL/sycl.hpp>

#include <bits/sycl_policy.h>

namespace cppcon {

template <class ContiguousIt, class T, class BinaryOperation,
          typename KernelName>
T reduce(sycl_execution_policy_t<KernelName> policy, ContiguousIt first,
         ContiguousIt last, T init, BinaryOperation binary_op) {
  using diff_t = typename std::iterator_traits<ContiguousIt>::difference_type;
  using value_t = typename std::iterator_traits<ContiguousIt>::value_type;

  if (first == last) return init;

  T result;

  try {
    auto q = policy.get_queue();
    auto d = q.get_device();

    /*
    auto maxWorkGroupSize =
        d.template get_info<cl::sycl::info::device::max_work_group_size>();
    auto maxDims =
        d.template get_info<cl::sycl::info::device::max_work_group_size>();
    */

    cl::sycl::program prog(q.get_context());
    prog.build_with_kernel_type<KernelName>();
    auto kernel = prog.get_kernel<KernelName>();
    auto maxWorkGroupSize = kernel.template get_work_group_info<
        cl::sycl::info::kernel_work_group::work_group_size>(d);

    size_t dataSize = std::distance(first, last);

    auto globalRange = cl::sycl::range<1>(std::max(dataSize, maxWorkGroupSize));
    auto localRange = cl::sycl::range<1>(std::min(dataSize, maxWorkGroupSize));
    auto ndRange = cl::sycl::nd_range<1>(globalRange, localRange);

    cl::sycl::buffer<value_t, 1> inputBuf(first, last);
    inputBuf.set_final_data(nullptr);

    do {
      q.submit([&](cl::sycl::handler& cgh) {

        auto inputAcc =
            inputBuf.template get_access<cl::sycl::access::mode::read_write>(
                cgh);

        cl::sycl::accessor<value_t, 1, cl::sycl::access::mode::read_write,
                           cl::sycl::access::target::local>
            scratchPad(localRange, cgh);

        cgh.parallel_for<KernelName>(ndRange, [=](cl::sycl::nd_item<1> ndItem) {
          size_t globalId = ndItem.get_global_id(0);
          size_t localId = ndItem.get_local_id(0);
          size_t groupId = ndItem.get_group(0);

          scratchPad[localId] = inputAcc[globalId];

          ndItem.barrier(cl::sycl::access::fence_space::local_space);

          // if (globalId < dataSize) {
          // auto min = cl::sycl::min(dataSize, maxWorkGroupSize);
          int min = (dataSize < maxWorkGroupSize) ? dataSize : maxWorkGroupSize;
          for (size_t offset = min / 2; offset > 0; offset /= 2) {
            if (localId < offset) {
              scratchPad[localId] =
                  binary_op(scratchPad[localId], scratchPad[localId + offset]);
            }

            ndItem.barrier(cl::sycl::access::fence_space::local_space);
          }

          if (localId == 0) {
            inputAcc[groupId] = scratchPad[localId];
          }
          // }
        });
      });
      dataSize = dataSize / maxWorkGroupSize;
    } while (dataSize > 1);

    {
      auto inputHostAcc =
          inputBuf.template get_access<cl::sycl::access::mode::read>();
      result = inputHostAcc[0];
    }

    q.wait_and_throw();
  } catch (cl::sycl::exception e) {
    std::cout << "SYCL exception caught: " << e.what() << std::endl;
  }

  return result + init;
}

}  // namespace cppcon

#endif  // __GPU_REDUCE_H__
