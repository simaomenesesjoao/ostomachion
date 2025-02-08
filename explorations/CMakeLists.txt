cmake_minimum_required(VERSION 3.10)
project(Catch2WithGMP)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
find_package(Catch2 3 REQUIRED)

# Add Catch2 as a header-only library (if you don't use package manager for Catch2)
# You can also use FetchContent to download Catch2 automatically:
# include_directories(${CMAKE_SOURCE_DIR}/path/to/catch2) 

# Alternatively, if installed via pacman or a package manager, Catch2 is usually found
# automatically, but you might need to set the include path manually
# include_directories("/usr/include/catch2")

# Create your test executable
add_executable(test catch2_test.cpp)

# target_link_libraries(test "-lgmp -lgmpxx")
target_link_libraries(test PRIVATE Catch2::Catch2WithMain "-lgmp -lgmpxx")

# Enable Catch2's automatic test discovery and linking
include_directories(${CMAKE_SOURCE_DIR}/catch2)  # Ensure Catch2 is in the include path

