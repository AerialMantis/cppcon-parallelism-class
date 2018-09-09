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

#include <benchmark.h>
#include <cppcon_solution>

constexpr int size = 4194304;
constexpr int iterations = 10;

TEST_CASE("cppcon::inclusive_scan(par)", "parallel_inclusive_scan") {
  std::vector<int> result(size);
  std::vector<int> expected(size);

  {
    auto input = std::vector<int>(size);

    init_data(input, [](int &value, unsigned index) { value = index % 16; });

    auto time = eval_performance(
        [&]() {
          cppcon::inclusive_scan(cppcon::par, input.begin(), input.end(),
                                 result.begin(), std::plus<>(), 0);
        },
        iterations);

    print_time<std::milli>("cppcon::inclusive_scan(par) (" +
                               std::to_string(iterations) + " iterations)",
                           time);
  }

  {
    auto input = std::vector<int>(size);

    init_data(input, [](int &value, unsigned index) { value = index % 16; });

    auto time = eval_performance(
        [&]() {
          std::partial_sum(input.begin(), input.end(), expected.begin());
        },
        iterations);

    print_time<std::milli>(
        "std::partial_sum (" + std::to_string(iterations) + " iterations)",
        time);
  }

  for (int i = 0; i < size; i++) {
    REQUIRE(result[i] == expected[i]);
  }
}
