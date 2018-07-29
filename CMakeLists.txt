cmake_minimum_required (VERSION 3.7.2)
project (cppcon2018-parallelism-class)

# Dependencies

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(Threads REQUIRED)

# Headers

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/external/catch2)

file(GLOB PARALLELISM_INCLUDES ${CMAKE_SOURCE_DIR}/include/*.h)
source_group("Parallelism Headers" FILES ${PARALLELISM_INCLUDES})

# Examples

enable_testing()
file(GLOB examples "examples/*/*cpp")
foreach(examplePath ${examples})
  get_filename_component(exampleName ${examplePath} NAME_WE)
  add_executable(${exampleName} ${examplePath})
  target_include_directories(${exampleName} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
  set_property(TARGET ${exampleName} PROPERTY CXX_STANDARD 17)
  set_property(TARGET ${exampleName} PROPERTY CXX_STANDARD_REQUIRED ON)
  add_test(${exampleName} ${examplePath})
endforeach()
