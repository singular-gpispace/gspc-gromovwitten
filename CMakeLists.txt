cmake_minimum_required(VERSION 3.16)
project(feynman LANGUAGES CXX)

# Set the C++ standard and requirements
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(GPISpace REQUIRED)
find_package(util-generic REQUIRED)
find_package(util-cmake REQUIRED)
find_package(Boost 1.63 REQUIRED COMPONENTS filesystem program_options)

# Include custom CMake macros
include(util-cmake/add_macros)

# Define a variable to hold the path to gspc-rifd
set(GSPC_RIFD_PATH "/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/bin/gspc-rifd" CACHE PATH "Path to gspc-rifd binary")

# Debug message to verify the path
message(STATUS "GSPC_RIFD_PATH: ${GSPC_RIFD_PATH}")

# Add your project configuration and targets here...
set(BUNDLE_ROOT "libexec/bundle")
bundle_GPISpace(DESTINATION "${BUNDLE_ROOT}/gpispace" COMPONENTS runtime)

# Locate FLINT include and library directories
set(FLINT_PREFIX "$ENV{HOME}/local") # Adjust this path as per your installation

find_path(FLINT_INCLUDE_DIR NAMES flint/flint.h HINTS "${FLINT_PREFIX}/include")
find_library(FLINT_LIB NAMES flint HINTS "${FLINT_PREFIX}/lib")

if(NOT FLINT_INCLUDE_DIR OR NOT FLINT_LIB)
  message(FATAL_ERROR "FLINT library not found. Please install FLINT or set FLINT_INCLUDE_DIR and FLINT_LIB.")
endif()

# Configure and add library target
add_library(feynman_implementation SHARED "src/feynman.cpp")
target_include_directories(feynman_implementation PUBLIC "${CMAKE_SOURCE_DIR}" "${FLINT_INCLUDE_DIR}")
target_link_libraries(feynman_implementation PRIVATE ${FLINT_LIB})

set_target_properties(feynman_implementation PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH TRUE
  INSTALL_RPATH "$ORIGIN/../lib:$ORIGIN/../../lib:$ENV{HOME}/local/lib"
)

set(PETRI_NET "${PROJECT_SOURCE_DIR}/workflow/${PROJECT_NAME}.xpnet")
set(COMPILED_PETRI_NET "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pnet")
set(WRAPPER_DIRECTORY "${PROJECT_BINARY_DIR}/gen")
set(WRAPPER_LIBRARY "${WRAPPER_DIRECTORY}/pnetc/op/lib${PROJECT_NAME}_module.so")
set(DRIVER_BINARY "${PROJECT_NAME}")

add_custom_command(
  COMMAND GPISpace::pnetc --input="${PETRI_NET}" --output="${COMPILED_PETRI_NET}"
  OUTPUT "${COMPILED_PETRI_NET}"
  DEPENDS "${PETRI_NET}"
)
add_custom_target(compiled_petri_net ALL DEPENDS "${COMPILED_PETRI_NET}")

add_custom_command(
  COMMAND GPISpace::pnetc
  --gen-cxxflags="-O3"
  --gen-cxxflags="-I${PROJECT_SOURCE_DIR}/include"
  --gen-cxxflags="-I${PROJECT_SOURCE_DIR}/include/feynman"
  --gen-ldflags="-L${PROJECT_BINARY_DIR}"
  --gen-ldflags="-lfeynman_implementation"
  --gen-ldflags="-Wl,-rpath=${CMAKE_INSTALL_PREFIX}/lib"
  --force-overwrite-file=true
  --input="${PETRI_NET}"
  --output="/dev/null"
  --path-to-cpp="${WRAPPER_DIRECTORY}"
  COMMAND +make -C "${WRAPPER_DIRECTORY}"
  OUTPUT "${WRAPPER_LIBRARY}"
  DEPENDS "${PETRI_NET}" feynman_implementation
)
add_custom_target(workflow_library ALL DEPENDS "${WRAPPER_LIBRARY}")

# Ensure correct build order
add_dependencies(workflow_library compiled_petri_net)

extended_add_executable(NAME "${DRIVER_BINARY}"
  DONT_APPEND_EXE_SUFFIX
  SOURCES
  src/parse_parameters_from_commandline.cpp
  src/execute.cpp
  src/main.cpp
  src/Workflow.cpp
  src/WorkflowResult.cpp
  src/ValuesOnPorts.cpp
  INCLUDE_DIRECTORIES PRIVATE include
  LIBRARIES PRIVATE Util::Generic GPISpace::execution Boost::headers Boost::filesystem Boost::program_options feynman_implementation
  INSTALL
  INSTALL_DESTINATION bin
)

bundle_GPISpace_add_rpath(TARGET "${DRIVER_BINARY}" INSTALL_DIRECTORY "bin")

install(FILES "${COMPILED_PETRI_NET}" DESTINATION pnet)
install(TARGETS feynman_implementation DESTINATION lib)
install(FILES "${WRAPPER_LIBRARY}" DESTINATION lib)
