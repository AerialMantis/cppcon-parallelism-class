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

#ifndef __PARALLEL_TRANSFORM_REDUCE_H__
#define __PARALLEL_TRANSFORM_REDUCE_H__

#include <functional>
#include <iterator>
#include <thread>
#include <utility>
#include <vector>

#include <bits/policies.h>

namespace cppcon {

template <class ForwardIt, class T, class BinaryOperation, class UnaryOperation>
T transform_reduce(par_execution_policy_t policy, ForwardIt first,
                   ForwardIt last, T init, BinaryOperation binary_op,
                   UnaryOperation unary_op) {
  using diff_t = typename std::iterator_traits<ForwardIt>::difference_type;

  if (first == last) return init;

  unsigned int concurrency = std::thread::hardware_concurrency();
  diff_t numElements = std::distance(first, last);
  diff_t elementsPerThread = numElements / concurrency;
  diff_t remainder = numElements % concurrency;

  auto processChunk = [binary_op](ForwardIt c_first, ForwardIt c_last) {
    T partialReduce{0};
    while (c_first != c_last) {
      partialReduce = binary_op(partialReduce, *c_first++);
    }
    return partialReduce;
  };

  auto processChunkWithTransform = [unary_op, binary_op](ForwardIt c_first,
                                                         ForwardIt c_last) {
    T partialReduce{0};
    while (c_first != c_last) {
      partialReduce = binary_op(partialReduce, unary_op(*c_first++));
    }
    return partialReduce;
  };

  std::vector<std::thread> threads(0);
  threads.reserve(concurrency - 1);
  std::vector<T> partialReductions(8);
  for (unsigned t = 1; t < concurrency; t++) {
    diff_t currentChunkSize =
        elementsPerThread + static_cast<diff_t>(t < remainder);
    diff_t currentOffset =
        (t * elementsPerThread) + std::min(static_cast<diff_t>(t), remainder);
    threads.emplace_back([
      t, processChunkWithTransform = std::move(processChunkWithTransform),
      currentChunkSize = std::move(currentChunkSize),
      c_first = std::next(first, currentOffset),
      c_last = std::next(first, currentOffset + currentChunkSize),
      &partialReductions
    ]() mutable {
      partialReductions[t] = processChunkWithTransform(c_first, c_last);
    });
  }
  diff_t currentChunkSize =
      elementsPerThread + static_cast<diff_t>(0 < remainder);
  partialReductions[0] =
      processChunkWithTransform(first, std::next(first, currentChunkSize));

  for (auto &thread : threads) {
    thread.join();
  }

  return binary_op(
      init, processChunk(partialReductions.begin(), partialReductions.end()));
}

}  // namespace cppcon

#endif  // __PARALLEL_TRANSFORM_REDUCE_H__
