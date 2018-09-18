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

#include <bits/std_policies.h>

namespace cppcon {

template <class ForwardIt, class OutputIt, class UnaryOperation>
OutputIt transform(par_execution_policy_t policy, ForwardIt first,
                   ForwardIt last, OutputIt d_first, UnaryOperation unary_op) {

  /* implement me */

  return d_first;
}

}  // namespace cppcon

#endif  // __PARALLEL_TRANSFORM_H__
