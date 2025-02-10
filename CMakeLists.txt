cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")

project(Main1 VERSION 1.0)
# find_library(NUMERIC Numeric PATHS ${CMAKE_CURRENT_SOURCE_DIR}/numeric/lib/ REQUIRED)

add_executable(interactive interactive_state.cpp)
target_link_libraries(interactive PUBLIC gmp gmpxx)
target_include_directories(interactive PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/numeric/include)
target_include_directories(interactive PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/state/include)

add_executable(solver solver.cpp)
target_link_libraries(solver PUBLIC gmp gmpxx)
target_include_directories(solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/numeric/include)
target_include_directories(solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/state/include)

install(TARGETS interactive DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
install(TARGETS solver DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)