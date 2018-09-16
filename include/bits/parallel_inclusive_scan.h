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

#ifndef __PARALLEL_INCLUSIVE_SCAN_H__
#define __PARALLEL_INCLUSIVE_SCAN_H__

#include <functional>
#include <iterator>
#include <thread>
#include <utility>
#include <vector>

#include <bits/std_policies.h>

namespace cppcon {

template <class ForwardIt1, class ForwardIt2, class BinaryOperation, class T>
ForwardIt2 inclusive_scan(par_execution_policy_t policy, ForwardIt1 first,
                          ForwardIt1 last, ForwardIt2 d_first,
                          BinaryOperation binary_op, T init) {
  using diff_t = typename std::iterator_traits<ForwardIt1>::difference_type;

  return d_first;

  /* implement me */
}

}  // namespace cppcon

#endif  // __PARALLEL_INCLUSIVE_SCAN_H__
