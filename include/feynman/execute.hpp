#pragma once

#include <feynman/Parameters.hpp>
#include <feynman/WorkflowResult.hpp>

namespace feynman
{
  class Workflow;

  namespace execution
  {
    ParametersDescription options();
  }

  WorkflowResult execute (Parameters, Workflow const&);
}