cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(temp_test)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")


# find_package(Catch2 REQUIRED)

file(GLOB TEST_FILES temp_test.cpp)
#file(GLOB TEST_FILES intersection_algorithms_benchmark.cpp)
add_executable(temp_test ${TEST_FILES})
target_link_libraries(temp_test PUBLIC  gmp gmpxx pthread SDL2 SDL2_gfx)
# target_link_libraries(temp_test PUBLIC  gmp gmpxx Catch2::Catch2WithMain)
target_include_directories(temp_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include/)
target_include_directories(temp_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../numeric/include/)
