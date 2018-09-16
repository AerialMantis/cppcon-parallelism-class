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
#include <sycl_execution>

constexpr int size = 2097152;
constexpr int iterations = 10;

class transform_reduce;

TEST_CASE("cppcon::transform_reduce(par)", "gpu_transform_reduce") {
  int result{0};
  int expected{0};

  {
    auto input = std::vector<int>(size);

    init_data(input, [](int &value, unsigned index) { value = index % 16; });

    cppcon::sycl<transform_reduce> syclPolicy;

    auto cppconTime = eval_performance(
        [&]() {
          result = cppcon::transform_reduce(syclPolicy, input.begin(),
                                            input.end(), 42, std::plus<>(),
                                            [](int &in) { return in * 2; });
        },
        iterations);

    print_time<std::milli>("cppcon::transform_reduce(sycl) (" +
                               std::to_string(iterations) + " iterations)",
                           cppconTime);
  }

  {
    auto input = std::vector<int>(size);
    auto temp = std::vector<int>(size);

    init_data(input, [](int &value, unsigned index) { value = index % 16; });

    auto time = eval_performance(
        [&]() {
          std::transform(input.begin(), input.end(), temp.begin(),
                         [](int &in) { return in * 2; });

          expected = std::accumulate(temp.begin(), temp.end(), 42);
        },
        iterations);

    print_time<std::milli>("std::transform & std::accumulate (" +
                               std::to_string(iterations) + " iterations)",
                           time);
  }

  REQUIRE(result == expected);
}
