cmake_minimum_required(VERSION 3.16)
project(singular-template LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

set(INSTALL_DO_NOT_BUNDLE ON CACHE BOOL "")

find_package(GPISpace REQUIRED)

find_package(util-generic REQUIRED)
find_package(util-cmake REQUIRED)

include(util-cmake/add_macros)

include(util-cmake/beautify_find_boost)
include(util-cmake/beautify_find_GPISpace)

find_boost(1.61 REQUIRED QUIET FROM_GPISPACE_INSTALLATION COMPONENTS
  date_time
  filesystem
  program_options
  regex
  serialization
  system
  test_exec_monitor
  thread
  unit_test_framework
  timer
  iostreams
  chrono
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
find_package(Singular REQUIRED)

# Find FLINT
find_library(FLINT_LIBRARY
  NAMES flint
  HINTS ${FLINT_LIB_DIR}
  PATH_SUFFIXES lib
)

if(NOT FLINT_LIBRARY)
  message(FATAL_ERROR "FLINT library not found!")
endif()

# Include directories for project
include_directories("src")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/src")
include_directories("${FLINT_INCLUDE_DIR}")

# Add subdirectories
add_subdirectory(src)
add_subdirectory(template)
add_subdirectory(workflow)

# Configure config.hpp file
configure_file("template/config.hpp.in"
  "${CMAKE_BINARY_DIR}/template/config.hpp"
  @ONLY
)

# Add custom target to ensure config.hpp is generated
add_custom_target(Config
  ALL
  DEPENDS "${CMAKE_BINARY_DIR}/template/config.hpp"
)

# Install the examples directory
# install(DIRECTORY examples
# DESTINATION "${CMAKE_INSTALL_PREFIX}/share"
# )

# Link the FLINT library to the  target  template_implementation
# target_link_libraries(template_implementation PRIVATE ${FLINT_LIB})
