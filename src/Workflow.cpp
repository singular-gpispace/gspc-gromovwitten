#include <../include/feynman/Workflow.hpp>
#include <iostream>

namespace feynman
{

    using pnet_value = pnet::type::value::value_type;
    using pnet_list = std::list<pnet_value>;
    using pnet_list2d = std::list<std::list<pnet_value>>;

    ParametersDescription Workflow::options()
    {
        namespace po = boost::program_options;

        ParametersDescription workflow_opts("Workflow");
        workflow_opts.add_options()("N", po::value<int>()->required()); // edges
        workflow_opts.add_options()("degree", po::value<int>()->required());

        return workflow_opts;
    }

    Workflow::Workflow(Parameters const& args)
        : _N(args.at("N").as<int>()) // number of edges.
        ,
        _degree(args.at("degree").as<int>()) // given degree
    {
    }

    ValuesOnPorts Workflow::inputs() const
    {
        ValuesOnPorts::Map values_on_ports;
        values_on_ports.emplace("N", _N);
        values_on_ports.emplace("degree", _degree);
        return values_on_ports;
    }

    int Workflow::process(WorkflowResult const& results) const
    {

        auto const& resul = results.get<std::string>("vectors");

        std::cout << "vectors " << resul << std::endl;
        std::cout << std::endl;


        return EXIT_SUCCESS;
    }
}