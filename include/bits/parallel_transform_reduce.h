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

#include <bits/std_policies.h>

namespace cppcon {

template <class ForwardIt, class T, class BinaryOperation, class UnaryOperation>
T transform_reduce(par_execution_policy_t policy, ForwardIt first,
                   ForwardIt last, T init, BinaryOperation binary_op,
                   UnaryOperation unary_op) {
  using diff_t = typename std::iterator_traits<ForwardIt>::difference_type;

  /* If the input range is empty then return the output iterator */
  if (first == last) return init;

  /* Retrieve the hardware concurrency of you system */
  unsigned int concurrency = std::thread::hardware_concurrency();

  /* Calculate the data size, the base chunk size and the remainder */
  diff_t dataSize = std::distance(first, last);
  diff_t baseChunkSize = dataSize / concurrency;
  diff_t remainder = dataSize % concurrency;

  /* Create a lambda function for processing a chunk */
  auto processChunk = [=](ForwardIt first, ForwardIt last, bool doTransform) {
    T partialReduce{0};
    while (first != last) {
      if (doTransform) {
        partialReduce = binary_op(partialReduce, unary_op(*first++));
      } else {
        partialReduce = binary_op(partialReduce, *first++);
      }
    }
    return partialReduce;
  };

  /* Reserve a vector of threads for concurrency - 1 */
  std::vector<std::thread> threads(0);
  threads.reserve(concurrency - 1);

  /* Create a vector of partial reduction values */
  std::vector<T> partialReductions(8);

  /* Loop over the hardware concurrency, starting at 1 */
  for (unsigned t = 1; t < concurrency; t++) {
    /* Calcualte the current chunk size as the base chunk size plus a potential
     * remaider */
    diff_t currentChunkSize =
        baseChunkSize + static_cast<diff_t>(t < remainder);

    /* Calculate the offset to the current chunk */
    diff_t currentOffset =
        (t * baseChunkSize) + std::min(static_cast<diff_t>(t), remainder);

    /* Create iterators to the beggining and end of the current chunk */
    auto chunkFirst = std::next(first, currentOffset);
    auto chunkLast = std::next(first, currentOffset + currentChunkSize);

    /* Launch a std::thread that will process a chunk with the iterators created
     * above*/
    threads.emplace_back([=, &partialReductions]() mutable {
      partialReductions[t] = processChunk(chunkFirst, chunkLast, true);
    });
  }

  /* Calculate the chunk size for the chunk that will execute on the calling
   * thread and create an iterator to the end of that chunk */
  diff_t currentChunkSize = baseChunkSize + static_cast<diff_t>(0 < remainder);
  auto chunkLast = std::next(first, currentChunkSize);

  /* Process the chunk of the calling thread  */
  partialReductions[0] = processChunk(first, chunkLast, true);

  /* Join all of the threads you created */
  for (auto &thread : threads) {
    thread.join();
  }

  /* Process all of the partial reducations as another chunk, pass the result
   * and the initial value to the binary operator and return the result */
  return binary_op(
      init, processChunk(partialReductions.begin(), partialReductions.end(),
      false));
}

}  // namespace cppcon

#endif  // __PARALLEL_TRANSFORM_REDUCE_H__
