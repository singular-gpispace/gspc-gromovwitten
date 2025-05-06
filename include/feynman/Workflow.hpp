#pragma once
#include <string>
#include <vector>
#include <list>
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
    std::vector<int> G;
    pnet_list graph_int;
    
    // Additional member variables
    int _m;          // for composition
    int _g;          // for quasi_psi
    int _deg;        // for quasi, sum, sum_psi
    std::string _genus;  // for quasi_psi, sum_psi
    std::string _loop;   // for quasi_psi, sum_psi
    
    // Helper variables
    int total_int;
    int deg_int;
    int genus_int;
  };
}
