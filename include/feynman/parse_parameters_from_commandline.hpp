#pragma once

#include <feynman/Parameters.hpp>

namespace feynman
{
  Parameters parse_parameters_from_commandline
    ( ParametersDescription const& driver_opts
    , ParametersDescription const& workflow_opts
    , ParametersDescription const& workflow_type_opts
    , int argc
    , char** argv
    );

  // Overload for backward compatibility
  Parameters parse_parameters_from_commandline
    ( ParametersDescription const& driver_opts
    , ParametersDescription const& workflow_opts
    , int argc
    , char** argv
    );
}
