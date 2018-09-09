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

#ifndef __PARALLEL_TRANSFORM_H__
#define __PARALLEL_TRANSFORM_H__

#include <functional>
#include <iterator>
#include <thread>
#include <vector>

#include <bits/policies.h>

namespace cppcon {

template <class ForwardIt, class OutputIt, class UnaryOperation>
OutputIt transform(par_execution_policy_t policy, ForwardIt first,
                   ForwardIt last, OutputIt d_first, UnaryOperation unary_op) {
  using diff_t = typename std::iterator_traits<ForwardIt>::difference_type;

  if (first == last) return d_first;

  unsigned int concurrency = std::thread::hardware_concurrency();
  diff_t numElements = std::distance(first, last);
  diff_t elementsPerThread = numElements / concurrency;
  diff_t remainder = numElements % concurrency;

  auto processChunk = [unary_op](ForwardIt input, OutputIt output,
                                 size_t numElements) {
    for (unsigned int i = 0; i < numElements; i++) {
      *output++ = unary_op(*input++);
    }
  };

  std::vector<std::thread> threads(0);
  threads.reserve(concurrency - 1);
  for (unsigned int t = 1; t < concurrency; t++) {
    diff_t currentChunkSize =
        elementsPerThread + static_cast<diff_t>(t < remainder);
    diff_t currentOffset = (t * elementsPerThread) +
                           std::min(static_cast<diff_t>(t), remainder);
    threads.emplace_back(
        [processChunk, currentChunkSize,
         batch = std::next(first, currentOffset),
         d_batch = std::next(d_first, currentOffset)]() mutable {
          processChunk(batch, d_batch, currentChunkSize);
        });
  }
  diff_t currentChunkSize =
      elementsPerThread + static_cast<diff_t>(0 < remainder);
  processChunk(first, d_first, currentChunkSize);

  for (auto &thread : threads) {
    thread.join();
  }

  return d_first;
}

}  // namespace cppcon

#endif  // __PARALLEL_TRANSFORM_H__
