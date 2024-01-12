#pragma once

#include <feynman/Parameters.hpp>
#include <feynman/ValuesOnPorts.hpp>
#include <feynman/WorkflowResult.hpp>

namespace feynman
{
  class Workflow
  {
  public:
    static ParametersDescription options();

    Workflow (Parameters const& parameters);

    ValuesOnPorts inputs() const;

    int process (WorkflowResult const& result) const;

  private:
    int _N;
  };
}
