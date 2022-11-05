cmake_minimum_required(VERSION 3.21)
project(lru_cache)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(google_tests)

add_executable(lru_cache main.cpp include/lru_cache.h google_tests/cache_tests.cpp)
