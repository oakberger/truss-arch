cmake_minimum_required(VERSION 3.16)  
project(truss_arch)
enable_language(CXX)

# Compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Wshadow -Wconversion -Wnull-dereference")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto")

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

# Find and link Eigen3 first
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

find_package(nlohmann_json CONFIG REQUIRED)
include_directories(/opt/homebrew/include)

# Find and link GLPK
include_directories(/opt/homebrew/Cellar/glpk/5.0/include)
link_directories(/opt/homebrew/Cellar/glpk/5.0/lib)

# Project source includes
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry
  ${CMAKE_CURRENT_SOURCE_DIR}/src/model
  ${CMAKE_CURRENT_SOURCE_DIR}/src/optimization
  ${CMAKE_CURRENT_SOURCE_DIR}/src/solvers
  ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
)

# Source files
file(GLOB_RECURSE TRUSS_SOURCES src/*.cpp)

# Core library
add_library(truss_core ${TRUSS_SOURCES})
target_link_libraries(truss_core PRIVATE ${GLPK_LIBRARIES})
target_link_libraries(truss_core PRIVATE glpk)
target_link_libraries(truss_core PRIVATE nlohmann_json::nlohmann_json)

# Main example executable
add_executable(trussopt examples/main.cpp)
target_link_libraries(trussopt PRIVATE truss_core Eigen3::Eigen)

target_compile_options(truss_core PRIVATE -Wno-sign-conversion -Wno-unused-but-set-variable)


# Testing support
option(TRUSS_TESTS "Build tests" ON)
if(TRUSS_TESTS)
  enable_testing()
  add_executable(basic_gsm_example tests/basic_gsm_example.cpp)
  target_link_libraries(basic_gsm_example PRIVATE truss_core Eigen3::Eigen)

  add_executable(main_test tests/main_test.cpp)
  target_link_libraries(main_test PRIVATE truss_core Eigen3::Eigen)

  add_executable(arch_geometry_test tests/arch_geometry_test.cpp)
  target_link_libraries(arch_geometry_test PRIVATE truss_core Eigen3::Eigen)
  
endif()

# Optional Doxygen docs
option(TRUSS_DOC "Build Doxygen documentation" ON)
if(TRUSS_DOC)
  find_package(Doxygen REQUIRED)
  add_subdirectory(docs)
endif()