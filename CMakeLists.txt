cmake_minimum_required(VERSION 3.16)
project(feynman LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(GPISpace REQUIRED)
find_package(Boost 1.63 REQUIRED COMPONENTS filesystem program_options)
find_package(util-generic REQUIRED)
find_package(util-cmake REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include(util-cmake/add_macros)

# Define paths
set(GENERATED_XPNET "${CMAKE_BINARY_DIR}/workflow/feynman.xpnet")
set(GENERATED_XPNET "${CMAKE_SOURCE_DIR}/workflow/feynman.xpnet")

set(COMPILED_PETRI_NET "${CMAKE_BINARY_DIR}/feynman.pnet")
set(WRAPPER_DIRECTORY "${CMAKE_BINARY_DIR}/gen")
#set(WRAPPER_LIBRARY "${WRAPPER_DIRECTORY}/pnetc/op/libfeynman_module.so")
set(WRAPPER_LIBRARY "${CMAKE_SOURCE_DIR}/bin/lib/libfeynman_module.so")

# -------------------- Generate Executable --------------------
add_executable(generate src/generate.cpp)
target_include_directories(generate PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(generate PRIVATE ${FLINT_LIB})

#  feynman.xpnet first then  Petri net
add_custom_command(
  OUTPUT "${GENERATED_XPNET}"
  COMMAND generate
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
  COMMENT "Generating feynman.xpnet"
)

add_custom_target(generate_xpnet ALL DEPENDS "${GENERATED_XPNET}")

# -------------------- Feynman Implementation Library --------------------
add_library(feynman_implementation SHARED src/feynman.cpp)
target_include_directories(feynman_implementation PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(feynman_implementation PRIVATE flint)

set_target_properties(feynman_implementation PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

# -------------------- Feynman Executable (Using GPISpace) --------------------
extended_add_executable(NAME "${PROJECT_NAME}"
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
set_target_properties(feynman PROPERTIES INSTALL_RPATH 
  "${CMAKE_INSTALL_PREFIX}/lib;${CMAKE_INSTALL_PREFIX}/libexec/bundle/gpispace/lib")

#  feynman.pnet is only built after feynman.xpnet is created
add_custom_command(
  OUTPUT "${COMPILED_PETRI_NET}"
  COMMAND GPISpace::pnetc --input="${GENERATED_XPNET}" --output="${COMPILED_PETRI_NET}"
  DEPENDS generate_xpnet
  COMMENT "Compiling Petri Net: feynman.pnet"
)

add_custom_target(compiled_petri_net ALL DEPENDS "${COMPILED_PETRI_NET}")

# -------------------- WRAPPER & WORKFLOW CONFIGURATION --------------------
add_custom_command(
  OUTPUT "${WRAPPER_LIBRARY}"
  COMMAND GPISpace::pnetc
  --gen-cxxflags="-O3"
  --gen-cxxflags="-I${PROJECT_SOURCE_DIR}/include"
  --gen-cxxflags="-I${PROJECT_SOURCE_DIR}/include/feynman"
  --gen-ldflags="-L${PROJECT_BINARY_DIR}"
  --gen-ldflags="-lfeynman_implementation"
  --gen-ldflags="-Wl,-rpath=${CMAKE_INSTALL_PREFIX}/lib"
  --force-overwrite-file=true
  --input="${GENERATED_XPNET}"
  --output="/dev/null"
  --path-to-cpp="${WRAPPER_DIRECTORY}"
  COMMAND make -C "${WRAPPER_DIRECTORY}"
  DEPENDS compiled_petri_net feynman_implementation
 # COMMENT "Building wrapper library: libfeynman_module.so"
)

add_custom_target(workflow_library ALL DEPENDS "${WRAPPER_LIBRARY}")

# -------------------- Installation --------------------
install(TARGETS generate DESTINATION bin)
install(TARGETS feynman_implementation DESTINATION lib)
install(FILES "${GENERATED_XPNET}" DESTINATION workflow)
install(FILES "${COMPILED_PETRI_NET}" DESTINATION pnet)
#install(FILES "${CMAKE_SOURCE_DIR}/bin/lib/libfeynman_module.so" DESTINATION lib)
