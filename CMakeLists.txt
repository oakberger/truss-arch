cmake_minimum_required(VERSION 3.16)
project(truss_arch)
enable_language(CXX)

# ================================
# Compiler settings
# ================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Wshadow -Wconversion -Wnull-dereference")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto")

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

# ================================
# Dependencies
# ================================
# Eigen (header-only)
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# nlohmann_json (with CMake support)
find_package(nlohmann_json CONFIG REQUIRED)
include_directories(/opt/homebrew/include)  # Optional fallback

# GLPK
include_directories(/opt/homebrew/Cellar/glpk/5.0/include)
link_directories(/opt/homebrew/Cellar/glpk/5.0/lib)

# Gurobi
# Gurobi (Manual configuration)
set(GUROBI_VERSION 120)  # Matches libgurobi120.dylib
set(GUROBI_ROOT /Library/gurobi1202/macos_universal2)

include_directories(${GUROBI_ROOT}/include)
link_directories(${GUROBI_ROOT}/lib)

# Use full paths to the actual libraries
set(GUROBI_LIB ${GUROBI_ROOT}/lib/libgurobi${GUROBI_VERSION}.dylib)
set(GUROBI_CXX_LIB ${GUROBI_ROOT}/lib/libgurobi_c++.a)

# ================================
# Include project source folders
# ================================
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry
  ${CMAKE_CURRENT_SOURCE_DIR}/src/model
  ${CMAKE_CURRENT_SOURCE_DIR}/src/optimization
  ${CMAKE_CURRENT_SOURCE_DIR}/src/solvers
  ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
)

# ================================
# Core library
# ================================
file(GLOB_RECURSE TRUSS_SOURCES src/*.cpp)

add_library(truss_core ${TRUSS_SOURCES})
target_link_libraries(truss_core PRIVATE glpk nlohmann_json::nlohmann_json ${GUROBI_CXX_LIB} ${GUROBI_LIB})
target_compile_options(truss_core PRIVATE -Wno-sign-conversion -Wno-unused-but-set-variable)

# ================================
# Example executables
# ================================
add_executable(arch_example_1 examples/arch_example_1.cpp)
target_link_libraries(arch_example_1 PRIVATE truss_core Eigen3::Eigen)

add_executable(arch_example_2 examples/arch_example_2.cpp)
target_link_libraries(arch_example_2 PRIVATE truss_core Eigen3::Eigen)

add_executable(arch_example_3 examples/arch_example_3.cpp)
target_link_libraries(arch_example_3 PRIVATE truss_core Eigen3::Eigen)

add_executable(arch_gurobi_1 examples/arch_gurobi_1.cpp)
target_link_libraries(arch_gurobi_1 PRIVATE truss_core Eigen3::Eigen)

add_executable(arch_gurobi_2 examples/arch_gurobi_2.cpp)
target_link_libraries(arch_gurobi_2 PRIVATE truss_core Eigen3::Eigen)

add_executable(elliptical_arch_1 examples/elliptical_arch_1.cpp)
target_link_libraries(elliptical_arch_1 PRIVATE truss_core Eigen3::Eigen)

add_executable(minimal_test examples/minimal_test.cpp)
target_link_libraries(minimal_test PRIVATE truss_core Eigen3::Eigen)

add_executable(rectangle_1 examples/rectangle_example_1.cpp)
target_link_libraries(rectangle_1 PRIVATE truss_core Eigen3::Eigen)

add_executable(rectangle_2 examples/rectangle_example_2.cpp)
target_link_libraries(rectangle_2 PRIVATE truss_core Eigen3::Eigen)

# ================================
# Unit testing
# ================================
option(TRUSS_TESTS "Build tests" ON)
if(TRUSS_TESTS)
  enable_testing()

  add_executable(main_test tests/main_test.cpp)
  target_link_libraries(main_test PRIVATE truss_core Eigen3::Eigen)
endif()

# ================================
# Doxygen documentation
# ================================
option(TRUSS_DOC "Build Doxygen documentation" ON)
if(TRUSS_DOC)
  find_package(Doxygen REQUIRED)
  add_subdirectory(docs)  # expects docs/CMakeLists.txt to handle Doxygen target with `ALL`
endif()

# ================================
# Sphinx documentation
# ================================
option(TRUSS_SPHINX_DOC "Build Sphinx documentation" ON)
if(TRUSS_SPHINX_DOC)
  # Define source and output directory for Sphinx
  set(SPHINX_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/docs/source)
  set(SPHINX_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs/sphinx)

  # Look for sphinx-build (CLI installed via Homebrew)
  find_program(SPHINX_BUILD_EXECUTABLE sphinx-build)

  if(SPHINX_BUILD_EXECUTABLE)
    add_custom_target(sphinx_docs ALL
      COMMAND ${SPHINX_BUILD_EXECUTABLE} -b html
              ${SPHINX_SOURCE_DIR} ${SPHINX_BUILD_DIR}
      COMMENT "Building HTML Sphinx documentation with sphinx-build"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      VERBATIM
    )
  else()
    message(WARNING "sphinx-build not found. 'sphinx_docs' target will not be available.")
  endif()
endif()

