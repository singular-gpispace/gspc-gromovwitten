cmake_minimum_required(VERSION 3.16)
project(feynman
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(GPISpace REQUIRED)

find_package(util-generic REQUIRED)
find_package(util-cmake REQUIRED)
find_package(Boost 1.63 REQUIRED
  COMPONENTS
  filesystem
  program_options
)
include(util-cmake/add_macros)

set(BUNDLE_ROOT "libexec/bundle")
bundle_GPISpace(DESTINATION "${BUNDLE_ROOT}/gpispace"
  COMPONENTS runtime
)

# extended_add_library (NAME feynman_implementation
# TYPE SHARED
# SOURCES "src/test.cpp"
# INCLUDE_DIRECTORIES PUBLIC "${CMAKE_SOURCE_DIR}"
# COMPILE_DEFINITIONS SING_NDEBUG OM_NDEBUG
# INSTALL INSTALL_DESTINATION "lib"
# )
add_library(feynman_implementation SHARED "src/test.cpp" "src/feynman.cpp" "src/state.cpp")
target_include_directories(feynman_implementation PUBLIC "${CMAKE_SOURCE_DIR}")

set_target_properties(feynman_implementation
  PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH TRUE
)

set(PETRI_NET "${PROJECT_SOURCE_DIR}/workflow/${PROJECT_NAME}.xpnet")
set(COMPILED_PETRI_NET "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pnet")
set(WRAPPER_DIRECTORY "${PROJECT_BINARY_DIR}/gen")
set(WRAPPER_LIBRARY "${WRAPPER_DIRECTORY}/pnetc/op/lib${PROJECT_NAME}_module.so")
set(DRIVER_BINARY "${PROJECT_NAME}")

add_custom_command(
  COMMAND
  GPISpace::pnetc --input="${PETRI_NET}" --output="${COMPILED_PETRI_NET}"
  OUTPUT "${COMPILED_PETRI_NET}"
  DEPENDS "${PETRI_NET}"
)
add_custom_target(compiled_petri_net
  ALL
  DEPENDS
  "${COMPILED_PETRI_NET}"
)

add_custom_command(
  COMMAND
  GPISpace::pnetc
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

# target_link_libraries("${WRAPPER_LIBRARY}" PRIVATE feynman_implementation)
extended_add_executable(NAME "${DRIVER_BINARY}"
  DONT_APPEND_EXE_SUFFIX
  SOURCES
  src/parse_parameters_from_commandline.cpp
  src/execute.cpp
  src/main.cpp
  src/Workflow.cpp
  src/WorkflowResult.cpp
  src/ValuesOnPorts.cpp
  src/state.cpp

  INCLUDE_DIRECTORIES PRIVATE include
  LIBRARIES PRIVATE Util::Generic
  GPISpace::execution
  Boost::headers
  Boost::filesystem
  Boost::program_options
  feynman_implementation

  INSTALL
  INSTALL_DESTINATION bin
)

bundle_GPISpace_add_rpath(TARGET "${DRIVER_BINARY}" INSTALL_DIRECTORY "bin")

install(FILES
  "${COMPILED_PETRI_NET}"
  DESTINATION pnet
)
install(TARGETS feynman_implementation
  DESTINATION lib)
install(FILES
  "${WRAPPER_LIBRARY}"
  DESTINATION lib
)