#pragma once
#include <string>
#include <feynman/Parameters.hpp>
#include <feynman/ValuesOnPorts.hpp>
#include <feynman/WorkflowResult.hpp>

namespace feynman
{
  class Workflow
  {
  public:
    static ParametersDescription options();

    Workflow(Parameters const& parameters);

    ValuesOnPorts inputs() const;

    int process(WorkflowResult const& result) const;
    using pnet_value = pnet::type::value::value_type;
    using pnet_list = std::list<pnet_value>;
    using pnet_list2d = std::list<std::list<pnet_value>>;

  private:
    int _N;
    int _degree;
    int _m;
    std::string _graph;
    std::vector<int> G; // Declare Gv as a member variable
    pnet_list graph_int;
  };
}