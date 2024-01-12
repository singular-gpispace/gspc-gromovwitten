#pragma once

#include <feynman/Parameters.hpp>

namespace feynman
{
  Parameters parse_parameters_from_commandline
    (ParametersDescription const& execution_options,
     ParametersDescription const& workflow_options,
     int argc,
     char** argv
    );
}
