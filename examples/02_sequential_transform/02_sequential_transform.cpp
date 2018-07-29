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
#include <sequential_transform.h>

constexpr int size = 4194304;
constexpr int iterations = 10;

TEST_CASE("std::transform", "sequential_transform") {
  auto input = std::vector<int>(size);
  auto output = std::vector<int>(size);

  init_data(input);

  auto time = eval_performance(
      [&]() {
        std::transform(input.begin(), input.end(), output.begin(),
                       [](int &in) { return in * 2; });
      },
      iterations);

  print_time<std::milli>(
      "std::transform (" + std::to_string(iterations) + " iterations)", time);

  REQUIRE(true);
}

TEST_CASE("cppcon::sequential::transform", "sequential_transform") {
  auto input = std::vector<int>(size);
  auto output = std::vector<int>(size);

  init_data(input);

  auto time = eval_performance(
      [&]() {
        cppcon::sequential::transform(input.begin(), input.end(),
                                      output.begin(),
                                      [](int &in) { return in * 2; });
      },
      iterations);

  print_time<std::milli>(
      "cppcon::sequential::transform (" + std::to_string(iterations) + " iterations)", time);

  REQUIRE(true);
}
