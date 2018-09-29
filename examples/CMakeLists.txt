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

function(add_example example)
	add_executable("${example}" "${example}.cpp")
    target_include_directories("${example}" PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/external/catch2)
	target_link_libraries("${example}" PRIVATE Threads::Threads)
    set_target_properties("${example}" PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
    add_test(${example} ${example})
endfunction()

function(add_sycl_example example)
	add_executable("${example}" "${example}.cpp")
    target_include_directories("${example}" PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/external/catch2)
	target_link_libraries("${example}" PRIVATE Threads::Threads)
    set_target_properties("${example}" PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

    add_sycl_to_target(
      TARGET "${example}"
      SOURCES "${example}.cpp"
    )

    add_test(${example} ${example})
endfunction()

add_example(thread_joinable)
add_example(thread_join)
add_example(thread_detach)
add_example(thread_exception)
add_example(thread_guard)
add_example(thread_param_by_val)
add_example(thread_param_by_ref)
add_example(thread_lifetime)
add_example(thread_move)
add_example(thread_id)
add_example(mutex_lock_unlock)
add_example(mutex_lock_guard)
add_example(mutex_unprotected_data)
add_example(mutex_unprotected_data_access)
add_example(mutex_deadlock_single_lock)
add_example(mutex_deadlock_unique_lock)
add_example(async)
add_example(packaged_task)
add_example(promise)
add_example(promise_exception)

add_sycl_example(hello_world)

add_example(sequential_transform)
add_example(sequential_reduce)
add_example(sequential_transform_reduce)

add_example(parallel_transform)
add_example(parallel_reduce)
add_example(parallel_transform_reduce)

add_sycl_example(gpu_transform)
add_sycl_example(gpu_reduce)
add_sycl_example(gpu_transform_reduce)
