cmake_minimum_required(VERSION 3.16)
project(feynman
  LANGUAGES CXX
)

# Add option to select workflow type
set(WORKFLOW_TYPE "base" CACHE STRING "Type of workflow to build (base, composition, doubles, quasi_psi, quasi, sum_psi, sum)")

# Set basic project settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set RPATH settings
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "\$ORIGIN/../lib:\$ORIGIN/../libexec/bundle/lib:\$ORIGIN/../libexec/bundle/gpispace/lib:\$ORIGIN/../libexec/bundle/gpispace/libexec/iml:/usr/local/lib")

# Set installation paths
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")

# Set library paths
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/local/lib")

# Set runtime library paths
set(CMAKE_RUNTIME_PATH ${CMAKE_RUNTIME_PATH} "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_PATH ${CMAKE_RUNTIME_PATH} "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_RUNTIME_PATH ${CMAKE_RUNTIME_PATH} "/usr/local/lib")

# Debug messages
message("--------------------------------")
message(STATUS "Selected workflow type: ${WORKFLOW_TYPE}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_INSTALL_RPATH: ${CMAKE_INSTALL_RPATH}")
message(STATUS "CMAKE_LIBRARY_PATH: ${CMAKE_LIBRARY_PATH}")
message(STATUS "CMAKE_RUNTIME_PATH: ${CMAKE_RUNTIME_PATH}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")

# Include the appropriate workflow-specific CMakeLists file
if(WORKFLOW_TYPE STREQUAL "base")
    include(${CMAKE_SOURCE_DIR}/cmake/CMakeLists_base.txt)
elseif(WORKFLOW_TYPE STREQUAL "composition")
    include(${CMAKE_SOURCE_DIR}/cmake/CMakeLists_composition.txt)
elseif(WORKFLOW_TYPE STREQUAL "doubles")
    include(${CMAKE_SOURCE_DIR}/cmake/CMakeLists_doubles_edges.txt)
elseif(WORKFLOW_TYPE STREQUAL "quasi_psi")
    include(${CMAKE_SOURCE_DIR}/cmake/CMakeLists_quasi_psi.txt)
elseif(WORKFLOW_TYPE STREQUAL "quasi")
    include(${CMAKE_SOURCE_DIR}/cmake/CMakeLists_quasi.txt)
elseif(WORKFLOW_TYPE STREQUAL "sum_psi")
    include(${CMAKE_SOURCE_DIR}/cmake/CMakeLists_sum_psi.txt)
elseif(WORKFLOW_TYPE STREQUAL "sum")
    include(${CMAKE_SOURCE_DIR}/cmake/CMakeLists_sum.txt)
else()
    message(FATAL_ERROR "Invalid workflow type: ${WORKFLOW_TYPE}. Valid types are: base, composition, doubles, quasi_psi, quasi, sum_psi, sum")
endif()
