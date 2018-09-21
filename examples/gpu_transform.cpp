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

#include <algorithm>

#include <benchmark.h>
#include <sycl_execution>

constexpr int size = 4194304;
constexpr int iterations = 10;

int pow(int in, int power) {
  for (int i = 1; i < power; i++) {
    in *= i;
  }

  return in;
}

class transform;

TEST_CASE("cppcon::transform(sycl)", "gpu_transform") {
  std::vector<int> result(size);
  std::vector<int> expected(size);

  {
    auto input = std::vector<int>(size);

    init_data(input, [](int &value, unsigned index) { value = index % 16; });

    cppcon::sycl<transform> syclPolicy;

    // TODO Merge eval_performance & print_time

    auto time = eval_performance(
        [&]() {
          cppcon::transform(syclPolicy, input.begin(), input.end(),
                            result.begin(),
                            [](int in) { return cppcon::pow(in, 100); });
        },
        iterations);

    print_time<std::milli>("cppcon::transform(sycl) (" +
                               std::to_string(iterations) + " iterations)",
                           time);
  }

  {
    auto input = std::vector<int>(size);

    init_data(input, [](int &value, unsigned index) { value = index % 16; });

    auto time = eval_performance(
        [&]() {
          std::transform(input.begin(), input.end(), expected.begin(),
                         [](int &in) { return pow(in, 100); });
        },
        iterations);

    print_time<std::milli>(
        "std::transform (" + std::to_string(iterations) + " iterations)", time);
  }

  for (int i = 0; i < size; i++) {
    REQUIRE(result[i] == expected[i]);
  }
}
