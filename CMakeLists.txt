cmake_minimum_required(VERSION 3.20)
project(cqq LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# O3 optimizations are enabled by default in Release mode
add_compile_options(-Wall -Wextra -pedantic -march=native)

file(GLOB_RECURSE SRC_FILES "src/*.cc")
add_library(cqq_lib SHARED ${SRC_FILES})

target_include_directories(cqq_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(cqq main.cc)
target_link_libraries(cqq PRIVATE cqq_lib)

find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    file(GLOB_RECURSE FORMAT_FILES 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cc"
    )

    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${FORMAT_FILES}
        COMMENT "Formatting code with clang-format"
    )
endif()

# ==========================
# Google Benchmark/Testing
# ==========================
include(FetchContent)

FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.8.3
)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# Build them but disable their own internal testing
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest googlebenchmark)

enable_testing()
include(GoogleTest)

add_executable(cqq_e2e_tests tests/e2e/circuit_e2e.cc)
target_compile_definitions(cqq_e2e_tests PRIVATE
  CQQ_SAMPLE_CIRCUITS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/sample_circuits")
target_link_libraries(cqq_e2e_tests PRIVATE cqq_lib GTest::gtest_main)
gtest_discover_tests(cqq_e2e_tests)

add_executable(cqq_unit_tests
  tests/unit/operations_unit.cc)
target_link_libraries(cqq_unit_tests PRIVATE cqq_lib GTest::gtest_main)
gtest_discover_tests(cqq_unit_tests)

add_executable(cqq_benchmarks benchmarks/bm_simulation.cc)
target_include_directories(cqq_benchmarks PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks)
target_link_libraries(cqq_benchmarks PRIVATE cqq_lib benchmark::benchmark_main)
