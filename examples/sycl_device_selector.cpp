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

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <CL/sycl.hpp>

struct gpu_selector : public cl::sycl::device_selector {
  int operator()(const cl::sycl::device& d) const override {
    if (d.is_gpu()) {
      return 1;
    } else {
      return -1;
    }
  }
};

TEST_CASE("device_selector", "sycl_device_selector") {
  try {
    cl::sycl::queue q(gpu_selector{});

    std::cout << "Device found: "
              << q.get_device().get_info<cl::sycl::info::device::name>()
              << "\n";

  } catch (...) {
    std::cout << "No GPU device could be found\n";
  }

  REQUIRE(true);
}
