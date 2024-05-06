#include <../include/feynman/Workflow.hpp>
#include <../include/feynman/feynman.hpp>
// #include <../feynman/funct.cpp>
// #include <../feynman/state.cpp>

#include <feynman/Workflow.hpp>
#include <iostream>
using pnet_value = pnet::type::value::value_type;
using pnet_list = std::list<pnet_value>;
using pnet_list2d = std::list<std::list<pnet_value, pnet_value>>;
namespace feynman
{
    ParametersDescription Workflow::options()
    {
        namespace po = boost::program_options;

        ParametersDescription workflow_opts("Workflow");
        workflow_opts.add_options()("N", po::value<int>()->required()); // edges
        workflow_opts.add_options()("degree", po::value<int>()->required());
        workflow_opts.add_options()("graph", po::value<std::string>()->required());

        return workflow_opts;
    }

    Workflow::Workflow(Parameters const &args)
        : _N(args.at("N").as<int>()) // number of edges.
          ,
          _degree(args.at("degree").as<int>()) // given degree
          ,
          _graph(args.at("graph").as<std::string>()) // graph.
    {
    }

    ValuesOnPorts Workflow::inputs() const
    {
        ValuesOnPorts::Map values_on_ports;
        values_on_ports.emplace("N", _N);
        values_on_ports.emplace("degree", _degree);
        values_on_ports.emplace("graph", _graph);

        return values_on_ports;
    }

    int Workflow::process(WorkflowResult const &results) const
    {
        // auto const &signature = results.get<int>("s");

        //  std::cout << " signature is : " << signature << std::endl;

        // auto const &feynm = results.get<int>("L");
        auto const &feynm = results.get<unsigned long>("sum");

        std::cout << "feynman_degree: " << feynm << std::endl;

        std::cout << std::endl;
        return EXIT_SUCCESS;
    }
}
