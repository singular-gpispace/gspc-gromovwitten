configure_file("installation.hpp.in" "installation.hpp" @ONLY)

extended_add_library(NAME installation
  TYPE SHARED
  NAMESPACE SINGULAR-template
  SOURCES "installation.cpp"
  LIBRARIES Util::Generic GPISpace::execution ${FLINT_LIB}
  POSITION_INDEPENDENT
  INSTALL
)

set_target_properties(SINGULAR-template-installation
  PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH TRUE
)

extended_add_library(NAME module
  TYPE SHARED
  NAMESPACE SINGULAR-template
  SOURCES "bin/SINGULAR-template-module.cpp"
  "bin/execute.cpp"
  "bin/parse_parameters_from_singular.cpp"
  "bin/ValuesOnPorts.cpp"
  "bin/Workflow.cpp"
  "bin/WorkflowResult.cpp"
  "${CMAKE_SOURCE_DIR}/template/singular_functions.cpp"
  "${CMAKE_SOURCE_DIR}/template/feynman.cpp"

  # INCLUDE_DIRECTORIES PRIVATE "${CMAKE_SOURCE_DIR}" "${FLINT_INCLUDE_DIR}"
  COMPILE_DEFINITIONS SING_NDEBUG OM_NDEBUG
  LIBRARIES SINGULAR-template::installation Util::Generic GPISpace::execution Singular # ${FLINT_LIB}
  POSITION_INDEPENDENT
  INSTALL
  INSTALL_DESTINATION "${CMAKE_INSTALL_PREFIX}"
)
#[[
set_target_properties(SINGULAR-template-module
  PROPERTIES
  INSTALL_RPATH "$ORIGIN/lib:${FLINT_LIB_DIR}"
  INSTALL_RPATH_USE_LINK_PATH TRUE
)
]]