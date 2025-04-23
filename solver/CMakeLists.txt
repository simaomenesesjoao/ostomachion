cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-O3 -g -Wall -Wextra -fno-inline")

# set(CMAKE_CXX_FLAGS "-O0 -g -rdynamic -Wall -Wextra -fno-inline")

project(Main1 VERSION 1.0)
# find_library(NUMERIC Numeric PATHS ${CMAKE_CURRENT_SOURCE_DIR}/numeric/lib/ REQUIRED)

# add_executable(interactive src/interactive_state.cpp)
# target_link_libraries(interactive PUBLIC gmp gmpxx)
# target_include_directories(interactive PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../numeric/include)
# target_include_directories(interactive PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../state/include)

add_executable(main src/main.cpp)
target_link_libraries(main PUBLIC gmp gmpxx pthread SDL2 SDL2_gfx)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../numeric/include)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../polygon/include)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../state/include)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../solver/include)

# install(TARGETS interactive DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
# install(TARGETS solver DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
