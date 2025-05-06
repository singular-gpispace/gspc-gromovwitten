cmake_minimum_required(VERSION 3.16)
project(feynman
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add option to select workflow type
set(WORKFLOW_TYPE "base" CACHE STRING "Type of workflow to build (base, composition, doubles, quasi_psi, quasi, sum_psi, sum)")

# Set RPATH settings
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "\$ORIGIN/../lib:\$ORIGIN/../libexec/bundle/lib:\$ORIGIN/../libexec/bundle/gpispace/lib:\$ORIGIN/../libexec/bundle/gpispace/libexec/iml")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Debug messages
message("--------------------------------")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_INSTALL_RPATH: ${CMAKE_INSTALL_RPATH}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

find_package(GPISpace REQUIRED)
find_package(util-generic REQUIRED)
find_package(util-cmake REQUIRED)
find_package(Boost 1.63 REQUIRED
  COMPONENTS
  filesystem
  program_options
)
include(util-cmake/add_macros)

# Set installation paths
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")
set(BUNDLE_ROOT "libexec/bundle")
bundle_GPISpace(DESTINATION "${BUNDLE_ROOT}/gpispace"
  COMPONENTS runtime
)

# FLINT include and library directories
set(FLINT_INCLUDE_DIR "/usr/local/include/flint/")
set(FLINT_LIB_DIR "/usr/local/lib/")

# FLINT include directory
include_directories(${FLINT_INCLUDE_DIR})

# FLINT library directory
link_directories(${FLINT_LIB_DIR})

# Configure and add implementation library target
if(WORKFLOW_TYPE STREQUAL "base")
    set(IMPLEMENTATION_SOURCE "src/feynman.cpp")
elseif(WORKFLOW_TYPE STREQUAL "doubles")
    set(IMPLEMENTATION_SOURCE "src/feynman_double_edges.cpp")
elseif(WORKFLOW_TYPE STREQUAL "composition")
    set(IMPLEMENTATION_SOURCE "src/feynman_composition.cpp")
elseif(WORKFLOW_TYPE STREQUAL "quasi_psi")
    set(IMPLEMENTATION_SOURCE "src/feynman_quasi_psi.cpp")
elseif(WORKFLOW_TYPE STREQUAL "quasi")
    set(IMPLEMENTATION_SOURCE "src/feynman_quasi.cpp")
elseif(WORKFLOW_TYPE STREQUAL "sum_psi")
    set(IMPLEMENTATION_SOURCE "src/feynman_sum_psi.cpp")
elseif(WORKFLOW_TYPE STREQUAL "sum")
    set(IMPLEMENTATION_SOURCE "src/feynman_sum.cpp")
endif()

add_library(feynman_implementation SHARED ${IMPLEMENTATION_SOURCE})
target_include_directories(feynman_implementation PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(feynman_implementation PRIVATE flint)

# Set RPATH for the implementation library
set_target_properties(feynman_implementation
  PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH TRUE
  INSTALL_RPATH "\$ORIGIN/../lib:\$ORIGIN/../libexec/bundle/lib:\$ORIGIN/../libexec/bundle/gpispace/lib:\$ORIGIN/../libexec/bundle/gpispace/libexec/iml"
)

# Set Petri net paths based on workflow type
if(WORKFLOW_TYPE STREQUAL "base")
    set(PETRI_NET "${PROJECT_SOURCE_DIR}/workflow/feynman.xpnet")
    set(COMPILED_PETRI_NET "${PROJECT_BINARY_DIR}/feynman.pnet")
    set(WORKFLOW_SOURCE "src/Workflow.cpp")
elseif(WORKFLOW_TYPE STREQUAL "composition")
    set(PETRI_NET "${PROJECT_SOURCE_DIR}/workflow/feynman_composition.xpnet")
    set(COMPILED_PETRI_NET "${PROJECT_BINARY_DIR}/feynman_composition.pnet")
    set(WORKFLOW_SOURCE "src/Workflow_composition.cpp")
elseif(WORKFLOW_TYPE STREQUAL "doubles")
    set(PETRI_NET "${PROJECT_SOURCE_DIR}/workflow/feynman_doubles_edges.xpnet")
    set(COMPILED_PETRI_NET "${PROJECT_BINARY_DIR}/feynman_doubles_edges.pnet")
    set(WORKFLOW_SOURCE "src/Workflow_doubles_edges.cpp")
elseif(WORKFLOW_TYPE STREQUAL "quasi_psi")
    set(PETRI_NET "${PROJECT_SOURCE_DIR}/workflow/feynman_quasi_psi.xpnet")
    set(COMPILED_PETRI_NET "${PROJECT_BINARY_DIR}/feynman_quasi_psi.pnet")
    set(WORKFLOW_SOURCE "src/Workflow_quasi_psi.cpp")
elseif(WORKFLOW_TYPE STREQUAL "quasi")
    set(PETRI_NET "${PROJECT_SOURCE_DIR}/workflow/feynman_quasi.xpnet")
    set(COMPILED_PETRI_NET "${PROJECT_BINARY_DIR}/feynman_quasi.pnet")
    set(WORKFLOW_SOURCE "src/Workflow_quasi.cpp")
elseif(WORKFLOW_TYPE STREQUAL "sum_psi")
    set(PETRI_NET "${PROJECT_SOURCE_DIR}/workflow/feynman_sum_psi.xpnet")
    set(COMPILED_PETRI_NET "${PROJECT_BINARY_DIR}/feynman_sum_psi.pnet")
    set(WORKFLOW_SOURCE "src/Workflow_sum_psi.cpp")
elseif(WORKFLOW_TYPE STREQUAL "sum")
    set(PETRI_NET "${PROJECT_SOURCE_DIR}/workflow/feynman_sum.xpnet")
    set(COMPILED_PETRI_NET "${PROJECT_BINARY_DIR}/feynman_sum.pnet")
    set(WORKFLOW_SOURCE "src/Workflow_sum.cpp")
else()
    message(FATAL_ERROR "Invalid workflow type: ${WORKFLOW_TYPE}")
endif()

# Set up wrapper generation
set(WRAPPER_DIRECTORY "${PROJECT_BINARY_DIR}/gen")
set(WRAPPER_LIBRARY "${WRAPPER_DIRECTORY}/pnetc/op/lib${PROJECT_NAME}_module.so")

# Compile selected Petri net
add_custom_command(
  OUTPUT "${COMPILED_PETRI_NET}"
  COMMAND GPISpace::pnetc --input="${PETRI_NET}" --output="${COMPILED_PETRI_NET}"
  DEPENDS "${PETRI_NET}"
  COMMENT "Compiling ${PETRI_NET}"
)

# Create a custom target for Petri net compilation
add_custom_target(compiled_petri_net ALL
  DEPENDS "${COMPILED_PETRI_NET}"
)

# Generate module wrapper with proper linking
add_custom_command(
  COMMAND
  GPISpace::pnetc
  --gen-cxxflags="-O3"
  --gen-cxxflags="-std=c++17"
  --gen-cxxflags="-I${PROJECT_SOURCE_DIR}/include"
  --gen-cxxflags="-I${PROJECT_SOURCE_DIR}/include/feynman"
  --gen-ldflags="-L${CMAKE_BINARY_DIR}"
  --gen-ldflags="-lfeynman_implementation"
  --gen-ldflags="-Wl,-rpath='\$\$ORIGIN/../lib'"
  --gen-ldflags="-Wl,--no-as-needed"
  --force-overwrite-file=true
  --input="${PETRI_NET}"
  --output="/dev/null"
  --path-to-cpp="${WRAPPER_DIRECTORY}"
  COMMAND
  +make -C "${WRAPPER_DIRECTORY}"
  OUTPUT "${WRAPPER_LIBRARY}"
  DEPENDS "${PETRI_NET}"
  feynman_implementation
  compiled_petri_net
)

add_custom_target(workflow_library
  ALL
  DEPENDS
  "${WRAPPER_LIBRARY}"
)

# Add workflow library based on selected type
add_library(feynman_workflow SHARED
  ${WORKFLOW_SOURCE}
  src/WorkflowResult.cpp
  src/ValuesOnPorts.cpp
)

target_include_directories(feynman_workflow PUBLIC include)
target_link_libraries(feynman_workflow PRIVATE
  feynman_implementation
  Boost::filesystem
  GPISpace::execution
)

set_target_properties(feynman_workflow
  PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH TRUE
  INSTALL_RPATH "\$ORIGIN/../lib:\$ORIGIN/../libexec/bundle/lib:\$ORIGIN/../libexec/bundle/gpispace/lib:\$ORIGIN/../libexec/bundle/gpispace/libexec/iml"
)

# Make sure the module depends on the implementation
add_dependencies(workflow_library feynman_implementation)

# Install targets
install(FILES "${COMPILED_PETRI_NET}" DESTINATION pnet)
install(TARGETS feynman_implementation DESTINATION lib)
install(FILES "${WRAPPER_LIBRARY}" DESTINATION lib)
install(TARGETS feynman_workflow DESTINATION lib)

# Add executable
extended_add_executable(NAME "feynman"
  DONT_APPEND_EXE_SUFFIX
  SOURCES
  src/parse_parameters_from_commandline.cpp
  src/execute.cpp
  src/main.cpp
  INCLUDE_DIRECTORIES PRIVATE include
  LIBRARIES PRIVATE 
    Util::Generic
    GPISpace::execution
    Boost::headers
    Boost::filesystem
    Boost::program_options
    feynman_implementation
    feynman_workflow
  INSTALL
  INSTALL_DESTINATION bin
)

# Make sure the executable depends on the implementation and workflow
add_dependencies(feynman feynman_implementation feynman_workflow)

# Set executable RPATH
set_target_properties(feynman
  PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH TRUE
  INSTALL_RPATH "\$ORIGIN/../lib:\$ORIGIN/../libexec/bundle/lib:\$ORIGIN/../libexec/bundle/gpispace/lib:\$ORIGIN/../libexec/bundle/gpispace/libexec/iml"
)
