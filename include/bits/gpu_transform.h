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

#ifndef __GPU_TRANSFORM_H__
#define __GPU_TRANSFORM_H__

#include <functional>
#include <iterator>
#include <thread>
#include <utility>
#include <vector>

#include <CL/sycl.hpp>

#include <bits/policies.h>

namespace cppcon {

template <class ContiguousIt, class UnaryOperation, typename KernelName>
ContiguousIt transform(sycl_execution_policy_t<KernelName> policy,
                   ContiguousIt first, ContiguousIt last, ContiguousIt d_first,
                   UnaryOperation unary_op) {
  using diff_t = typename std::iterator_traits<ContiguousIt>::difference_type;
  using pointer_t = typename std::iterator_traits<ContiguousIt>::pointer;
  using value_t = typename std::iterator_traits<ContiguousIt>::value_type;

  if (first == last) return d_first;

  auto asyncHandler = [=](cl::sycl::exception_list eL) {
    try {
      for (auto& e : eL) {
        std::rethrow_exception(e);
      }
    } catch (cl::sycl::exception e) {
      std::cout << "SYCL exception caught: " << e.what() << std::endl;
    }
  };

  try {
    cl::sycl::queue q(cl::sycl::intel_selector{}, asyncHandler);

    size_t dataSize = std::distance(first, last);

    auto globalRange = cl::sycl::range<1>(dataSize);

    pointer_t outputAddress = &(*d_first);

    cl::sycl::buffer<diff_t, 1> inputBuf(first, last);
    cl::sycl::buffer<value_t, 1> outputBuf(outputAddress, globalRange);
    inputBuf.set_final_data(nullptr);

    q.submit([&](cl::sycl::handler& cgh) {

      auto inputAcc =
          inputBuf.template get_access<cl::sycl::access::mode::read>(cgh);
      auto outputAcc =
          outputBuf.template get_access<cl::sycl::access::mode::discard_write>(
              cgh);

      cgh.parallel_for<KernelName>(globalRange, [=](cl::sycl::id<1> idx) {
        outputAcc[idx] = unary_op(inputAcc[idx]);
      });
    });

    q.wait_and_throw();
  } catch (cl::sycl::exception e) {
    std::cout << "SYCL exception caught: " << e.what() << std::endl;
  }

  return d_first;
}

}  // namespace cppcon

#endif  // __GPU_TRANSFORM_H__
