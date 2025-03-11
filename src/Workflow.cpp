#include <../include/feynman/Workflow.hpp>
#include <iostream>

namespace feynman
{
   
    using pnet_value = pnet::type::value::value_type;
    using pnet_list = std::list<pnet_value>;
    using pnet_list2d = std::list<std::list<pnet_value>>;
    pnet_list graph_int;

    ParametersDescription Workflow::options()
    {
        namespace po = boost::program_options;

        ParametersDescription workflow_opts("Workflow");
        workflow_opts.add_options()("input", po::value<std::string>()->required());
        workflow_opts.add_options()("P0", po::value<std::string>()->required());

        return workflow_opts;
    }

    Workflow::Workflow(const Parameters& args) 
    : _input(args.at("input").as<std::string>())  // Initialize _input correctly
    , _P0(args.at("P0").as<std::string>())  // Initialize _input correctly

{
}
    ValuesOnPorts Workflow::inputs() const
    {
        ValuesOnPorts::Map values_on_ports;
        values_on_ports.emplace("input", _input);
        values_on_ports.emplace("P0", _P0);

        return values_on_ports;
    }

    int Workflow::process(WorkflowResult const& results) const
    {
        // auto const &signature = results.get<int>("s");

        //  std::cout << " signature is : " << signature << std::endl;

        // auto const &feynm = results.get<int>("L");
        auto const& feynm = results.get<std::string>("result");
        std::cout << "feynman_degree: " << feynm << std::endl;


        std::cout << std::endl;
        return EXIT_SUCCESS;
    }
}