#pragma once
#include <string>
#include <feynman/Parameters.hpp>
#include <feynman/ValuesOnPorts.hpp>
#include <feynman/WorkflowResult.hpp>

using pnet_value = pnet::type::value::value_type;
using pnet_list = std::list<pnet_value>;
using pnet_list2d = std::list<std::list<pnet_value>>;
namespace feynman
{
  class Workflow
  {
  public:
    static ParametersDescription options();

    Workflow(Parameters const& parameters);

    ValuesOnPorts inputs() const;

    int process(WorkflowResult const& result) const;

  private:
    int _N;
    int _degree;
    std::string _graph;
    std::string _genus;
    std::string _loop;
    std::vector<int> G;
    pnet_list graph_int;
  };
}
