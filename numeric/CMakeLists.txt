cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(NumericLIB)

find_library(GMP_LIB gmp)
add_library(Numeric STATIC instantiator.cpp)
target_link_libraries(Numeric PUBLIC ${GMP_LIB})
install(TARGETS Numeric DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib)