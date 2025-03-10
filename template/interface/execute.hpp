#pragma once

#include <interface/Parameters.hpp>
#include <interface/WorkflowResult.hpp>

namespace template_module
{
  class Workflow;

  namespace execution
  {
    ParametersDescription options();
  }

  WorkflowResult execute (Parameters, Workflow const&);
}