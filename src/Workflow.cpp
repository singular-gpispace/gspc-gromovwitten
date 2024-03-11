#include <feynman/Workflow.hpp>
#include "partition.cpp"
#include <iostream>

namespace feynman
{
  ParametersDescription Workflow::options()
  {
    namespace po = boost::program_options;

    ParametersDescription workflow_opts ("Workflow");
    workflow_opts.add_options()("N", po::value<int>()->required()); //edges
    workflow_opts.add_options()("degree", po::value<int>()->required());
    workflow_opts.add_options()("graph", po::value<string>()->required());

    return workflow_opts;
  }

  Workflow::Workflow (Parameters const& args)
    : _N (args.at ("N").as<int>()) //number of edges.
     _degree (args.at ("d").as<int>()) // given degree
    , _graph (args.at ("graph").as<sdt::string>()) // graph. 
  {}

  ValuesOnPorts Workflow::inputs() const
  {
    ValuesOnPorts::Map values_on_ports;
   list p = sum_degree(_N,_degree);
    for (vector<int> pa : p) {
        //vector<vector<int> > values_map;
        values_map.push_back("values", pa);
        values_on_ports.emplace("branchtype",values_map);
    }
    values_on_ports.emplace ("graph", _graph);

    return values_on_ports;
  }

  int Workflow::process (WorkflowResult const& results) const
  {
    auto const& feynman_degree = results.get<std::string> ("feynman_degree");

    std::cout << "feynman: " << feynman << std::endl;

    return  EXIT_SUCCESS ;
  }
}
