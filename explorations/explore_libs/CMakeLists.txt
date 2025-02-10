cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(MyTalker)

file(GLOB SRC src/*.cpp)
#file(GLOB SRC src/functions.cpp)
add_library(Talker STATIC ${SRC})
target_include_directories(Talker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
