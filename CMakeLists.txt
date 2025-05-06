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
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "\$ORIGIN/../lib:\$ORIGIN/../libexec/bundle/lib:\$ORIGIN/../libexec/bundle/gpispace/lib:\$ORIGIN/../libexec/bundle/gpispace/libexec/iml:/usr/local/lib")

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

# Find required libraries
find_library(FLINT_LIBRARY NAMES flint PATHS "/usr/local/lib" REQUIRED)
find_library(GMP_LIBRARY NAMES gmp REQUIRED)
find_library(MPFR_LIBRARY NAMES mpfr REQUIRED)
find_library(GF2X_LIBRARY NAMES gf2x)
find_library(NTL_LIBRARY NAMES ntl)

# Find FLINT headers
find_path(FLINT_INCLUDE_DIR NAMES flint/flint.h PATHS "/usr/local/include" REQUIRED)
get_filename_component(FLINT_INCLUDE_PARENT ${FLINT_INCLUDE_DIR} DIRECTORY)

# Configure and add implementation library target
if(WORKFLOW_TYPE STREQUAL "base")
    set(IMPLEMENTATION_SOURCE "src/feynman_base.cpp")
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
target_include_directories(feynman_implementation PUBLIC 
    "${CMAKE_SOURCE_DIR}/include"
    ${FLINT_INCLUDE_PARENT}
)
target_link_libraries(feynman_implementation PRIVATE 
    ${FLINT_LIBRARY}
    ${GMP_LIBRARY}
    ${MPFR_LIBRARY}
)
if(GF2X_LIBRARY)
    target_link_libraries(feynman_implementation PRIVATE ${GF2X_LIBRARY})
endif()
if(NTL_LIBRARY)
    target_link_libraries(feynman_implementation PRIVATE ${NTL_LIBRARY})
endif()
target_link_options(feynman_implementation PRIVATE
    "-Wl,--enable-new-dtags"
)

# Set RPATH for the implementation library
set_target_properties(feynman_implementation
  PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH TRUE
  INSTALL_RPATH "\$ORIGIN/../lib:\$ORIGIN/../libexec/bundle/lib:\$ORIGIN/../libexec/bundle/gpispace/lib:\$ORIGIN/../libexec/bundle/gpispace/libexec/iml"
)

# Set Petri net paths based on workflow type
if(WORKFLOW_TYPE STREQUAL "base")
    set(PETRI_NET "${PROJECT_SOURCE_DIR}/workflow/feynman_base.xpnet")
    set(COMPILED_PETRI_NET "${PROJECT_BINARY_DIR}/feynman_base.pnet")
    set(WORKFLOW_SOURCE "src/Workflow_base.cpp")
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
  --gen-cxxflags="-I${FLINT_INCLUDE_PARENT}"
  --gen-ldflags="-L${CMAKE_BINARY_DIR}"
  --gen-ldflags="-L${CMAKE_INSTALL_PREFIX}/lib"
  --gen-ldflags="-lfeynman_implementation"
  --gen-ldflags="-Wl,--enable-new-dtags"
  --gen-ldflags="-Wl,-rpath=\$ORIGIN/../lib"
  --gen-ldflags="-Wl,-rpath=${CMAKE_INSTALL_PREFIX}/lib"
  --gen-ldflags="-Wl,--no-as-needed"
  --gen-ldflags="-L/usr/local/lib"
  --gen-ldflags="-lflint"
  --gen-ldflags="-lgmp"
  --gen-ldflags="-lmpfr"
  --force-overwrite-file=true
  --input="${PETRI_NET}"
  --output="/dev/null"
  --path-to-cpp="${WRAPPER_DIRECTORY}"
  COMMAND
  +make -C "${WRAPPER_DIRECTORY}"
  OUTPUT "${WRAPPER_LIBRARY}"
  DEPENDS "${PETRI_NET}"
  feynman_implementation
)

add_custom_target(workflow_library
  ALL
  DEPENDS
  "${WRAPPER_LIBRARY}"
)

# Set RPATH for the module library
set_target_properties(workflow_library
  PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH TRUE
  INSTALL_RPATH "\$ORIGIN/../lib:\$ORIGIN/../libexec/bundle/lib:\$ORIGIN/../libexec/bundle/gpispace/lib:\$ORIGIN/../libexec/bundle/gpispace/libexec/iml"
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
target_link_options(feynman_workflow PRIVATE
    "-Wl,--enable-new-dtags"
)

# Make sure the module depends on the implementation
add_dependencies(workflow_library feynman_implementation)

# Install targets
install(FILES "${COMPILED_PETRI_NET}" DESTINATION pnet)

# Install implementation library first
install(TARGETS feynman_implementation 
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
)
install(TARGETS feynman_implementation 
  RUNTIME DESTINATION "${BUNDLE_ROOT}/lib"
  LIBRARY DESTINATION "${BUNDLE_ROOT}/lib"
)

# Then install module and workflow
install(FILES "${WRAPPER_LIBRARY}" DESTINATION lib)
install(TARGETS feynman_workflow DESTINATION lib)

# Copy system libraries to bundle
install(FILES 
    ${FLINT_LIBRARY}
    ${GMP_LIBRARY}
    ${MPFR_LIBRARY}
    ${GF2X_LIBRARY}
    ${NTL_LIBRARY}
    DESTINATION "${BUNDLE_ROOT}/lib"
    OPTIONAL
)

# Create a custom target to install implementation library before building module
add_custom_target(install_implementation
  COMMAND ${CMAKE_COMMAND} -DCOMPONENT=Runtime -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
  DEPENDS feynman_implementation
)
add_dependencies(workflow_library install_implementation)

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

# Set RPATH for libraries
foreach(target feynman_implementation feynman_workflow)
  set_target_properties(${target}
    PROPERTIES
    INSTALL_RPATH_USE_LINK_PATH TRUE
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "\$ORIGIN/../lib:\$ORIGIN/../libexec/bundle/lib:\$ORIGIN/../libexec/bundle/gpispace/lib:\$ORIGIN/../libexec/bundle/gpispace/libexec/iml:/usr/local/lib"
  )
endforeach()

# Set RPATH for executable
set_target_properties(feynman
  PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH TRUE
  BUILD_WITH_INSTALL_RPATH TRUE
  INSTALL_RPATH "\$ORIGIN/../lib:\$ORIGIN/../libexec/bundle/lib:\$ORIGIN/../libexec/bundle/gpispace/lib:\$ORIGIN/../libexec/bundle/gpispace/libexec/iml:/usr/local/lib"
)

bundle_GPISpace_add_rpath(TARGET "feynman" INSTALL_DIRECTORY "bin")
