cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Main1 VERSION 1.0)
find_library(NUMERIC Numeric PATHS ${CMAKE_CURRENT_SOURCE_DIR}/numeric/lib/ REQUIRED)

add_executable(Main main_number.cpp)
target_link_libraries(Main PUBLIC ${NUMERIC} gmp gmpxx)
target_include_directories(Main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/numeric/src)
