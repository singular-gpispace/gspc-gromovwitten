#include <feynman/Workflow.hpp>
#include "partition.cpp"
#include <iostream>

namespace feynman
{
  ParametersDescription Workflow::options()
  {
    namespace po = boost::program_options;

    ParametersDescription workflow_opts ("Workflow");
    workflow_opts.add_options()("N", po::value<int>()->required());
    workflow_opts.add_options()("degree", po::value<int>()->required());
    workflow_opts.add_options()("mycode", po::value<string>()->required());

    return workflow_opts;
  }

  Workflow::Workflow (Parameters const& args)
    : _N (args.at ("N").as<int>()) //number of edges.
     _degree (args.at ("d").as<int>()) // given degree
    , _mycode (args.at ("mycode").as<sdt::string>()) //Julia code. 
  {}

  ValuesOnPorts Workflow::inputs() const
  {
    ValuesOnPorts::Map values_on_ports;
   list p = partition(_degree, _N);
    for (vector<int> pa : p) {
        //vector<vector<int> > values_map;
        values_map.push_back("values", pa);
        values_on_ports.emplace("branchtype",values_map);
    }
    values_on_ports.emplace ("julia_code", _mycode);

    return values_on_ports;
  }

  int Workflow::process (WorkflowResult const& results) const
  {
    auto const& feynman_degree = results.get<std::string> ("feynman_degree");

    std::cout << "feynman: " << feynman << std::endl;

    return  EXIT_SUCCESS ;
  }
}
