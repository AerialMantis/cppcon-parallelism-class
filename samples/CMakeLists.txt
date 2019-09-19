#[[
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
]]

add_example(sample thread_joinable)
add_example(sample thread_join)
add_example(sample thread_detach)
add_example(sample thread_exception)
add_example(sample thread_guard)
add_example(sample thread_param_by_val)
add_example(sample thread_param_by_ref)
add_example(sample thread_lifetime)
add_example(sample thread_move)
add_example(sample thread_id)
add_example(sample mutex_lock_unlock)
add_example(sample mutex_lock_guard)
add_example(sample mutex_unprotected_data)
add_example(sample mutex_unprotected_data_access)
add_example(sample mutex_deadlock_single_lock)
add_example(sample mutex_deadlock_unique_lock)
add_example(sample mutex_lock_ordering_deadlock)
add_example(sample async)
add_example(sample packaged_task)
add_example(sample promise)
add_example(sample promise_exception)
add_example(sample sequential_transform)
add_example(sample sequential_reduce)
add_example(sample sequential_transform_reduce)
add_example(sample parallel_transform)
add_example(sample parallel_reduce)
add_example(sample parallel_transform_reduce)

if (${CPPCON_ENABLE_SYCL})
  add_sycl_example(sample verify_sycl)
  add_sycl_example(sample sycl_error_handling)
  add_sycl_example(sample sycl_debugging)
  add_sycl_example(sample sycl_logging)
  add_sycl_example(sample sycl_barrier)
  add_sycl_example(sample gpu_transform)
  add_sycl_example(sample gpu_reduce)
  add_sycl_example(sample gpu_transform_reduce)
endif()