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
        auto const& vectors = results.get_all<std::string>("vectors", _degree); // Assuming _N is the expected count

        for (const auto& vec : vectors)
        {
            std::cout << "vectors: " << vec << std::endl;
        }

        return EXIT_SUCCESS;
    }
}