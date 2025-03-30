cmake_minimum_required(VERSION 3.5)
project(truss_arch)
enable_language(CXX)

# Compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE STRING "" FORCE)

# Build type
set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
  "Choose the type of build: None Debug Release RelWithDebInfo MinSizeRel."
  FORCE)

# Include directories
include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/model"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/optimization"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/solvers"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/utils"
)

# Gather all source files recursively
file(GLOB_RECURSE TRUSS_SOURCES
  src/*.cpp
)

# Create executable from all sources
add_executable(trussopt ${TRUSS_SOURCES})

option(TRUSS_TESTS "Build tests" OFF)

# Add tests if the option is enabled
option(TRUSS_TESTS "Build tests" OFF)

if(TRUSS_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

# Optional: link libraries here (e.g., Eigen)
# find_package(Eigen3 REQUIRED)
# target_link_libraries(trussopt Eigen3::Eigen)
