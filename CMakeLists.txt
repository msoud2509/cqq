cmake_minimum_required(VERSION 3.20)
project(cqq LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# O3 optimizations are enabled by default in Release mode
add_compile_options(-Wall -Wextra -pedantic -march=native)

file(GLOB_RECURSE SRC_FILES "src/*.cc")
add_library(cqq_lib ${SRC_FILES})

target_include_directories(cqq_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(cqq main.cc)
target_link_libraries(cqq PRIVATE cqq_lib)

find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    file(GLOB_RECURSE FORMAT_FILES 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp"
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
