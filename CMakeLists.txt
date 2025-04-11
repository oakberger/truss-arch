cmake_minimum_required(VERSION 3.30)
project(truss_arch)
enable_language(CXX)

# Compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE STRING "" FORCE)

# Build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Choose the type of build: None Debug Release RelWithDebInfo MinSizeRel.")
endif()

# Include directories
include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/geometry"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/model"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/optimization"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/solvers"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/utils"
  "/opt/homebrew/include"
  "/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3"
)

# Gather all source files recursively
file(GLOB_RECURSE TRUSS_SOURCES
  src/*.cpp
)

# Create executable from all sources
add_executable(trussopt ${TRUSS_SOURCES})
add_executable(trussopt_test tests/main_test.cpp ${TRUSS_SOURCES})

option(TRUSS_TESTS "Build tests" OFF)

# Add tests if the option is enabled
option(TRUSS_TESTS "Build tests" OFF)

if(TRUSS_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

# Find and link Eigen3
find_package(Eigen3 REQUIRED)
target_link_libraries(trussopt Eigen3::Eigen)

# Find and link Doxygen
option(TRUSS_DOC "Build Doxygen documentation" OFF)

if(TRUSS_DOC)
  find_package(Doxygen REQUIRED)
  add_subdirectory(docs)
endif()
