# https://www.youtube.com/watch?v=mKZ-i-UfGgQ
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(HelloWorld VERSION 1.0)

find_package(SFML 2 REQUIRED network audio graphics window system)
add_subdirectory(blah)
# find_library(ZLIB libz.a REQUIRED PATHS /usr/lib/libz.a)
find_library(ZLIB z REQUIRED) # automatically exposes the zlib include directory

add_executable(hello main.cpp)
#target_include_directories(hello PUBLIC ${SFML_INCLUDE_DIR})
# target_link_libraries(hello PUBLIC blah ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

target_include_directories(hello PUBLIC ${SFML_INCLUDE_DIR} ${ZLIB_INCULDE_DIR})
target_link_libraries(hello PUBLIC blah ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${ZLIB})
