cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(NumberTest VERSION 1.0)

add_subdirectory(numeric)

add_executable(NumTest main_number.cpp)
target_link_libraries(NumTest PUBLIC Numeric ${GMP_LIB})
