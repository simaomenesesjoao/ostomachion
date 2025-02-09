cmake_minimum_required(VERSION 3.10)
project(Catch2WithGMP)
set(CMAKE_CXX_STANDARD 20)

find_package(Catch2 3 REQUIRED)

include_directories("..")
add_executable(test numbers_test.cpp)
target_link_libraries(test PRIVATE Catch2::Catch2WithMain "-lgmp -lgmpxx")
include_directories(${CMAKE_SOURCE_DIR}/catch2)  # Ensure Catch2 is in the include path

