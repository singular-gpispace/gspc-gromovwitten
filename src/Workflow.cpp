#include <../include/feynman/Workflow.hpp>
#include <iostream>

namespace feynman
{
    std::vector<int> G;
    std::vector<int> extractIntegers(const std::string& graph)
    {
        std::vector<int> G;
        std::string num;
        for (char c : graph)
        {
            if (isdigit(c))
            {
                num += c;
            }
            else if (!num.empty())
            {
                G.push_back(std::stoi(num));
                num.clear();
            }
        }
        return G;
    }

    using pnet_value = pnet::type::value::value_type;
    using pnet_list = std::list<pnet_value>;
    using pnet_list2d = std::list<std::list<pnet_value>>;
    pnet_list graph_int;

    ParametersDescription Workflow::options()
    {
        namespace po = boost::program_options;

        ParametersDescription workflow_opts("Workflow");
        workflow_opts.add_options()("N", po::value<int>()->required()); // edges
        workflow_opts.add_options()("degree", po::value<int>()->required());
        workflow_opts.add_options()("graph", po::value<std::string>()->required());
        workflow_opts.add_options()("genus", po::value<std::string>()->required());
        workflow_opts.add_options()("loop", po::value<std::string>()->required());

        return workflow_opts;
    }

    Workflow::Workflow(Parameters const& args)
        : _N(args.at("N").as<int>()) // number of edges.
        , _degree(args.at("degree").as<int>()) // given degree
        , _graph(args.at("graph").as<std::string>()) // graph.
        , _genus(args.at("genus").as<std::string>()) // graph.
        , _loop(args.at("loop").as<std::string>()) // graph.

    {
        G = extractIntegers(_graph);
        for (int xi : G)
        {
            graph_int.emplace_back(pnet_value(xi));
        }
    }

    ValuesOnPorts Workflow::inputs() const
    {
        ValuesOnPorts::Map values_on_ports;
        values_on_ports.emplace("N", _N);
        values_on_ports.emplace("degree", _degree);
        values_on_ports.emplace("graph", graph_int);
        values_on_ports.emplace("genus", _genus);
        values_on_ports.emplace("loop", _loop);

        return values_on_ports;
    }

    int Workflow::process(WorkflowResult const& results) const
    {
        // auto const &signature = results.get<int>("s");

        //  std::cout << " signature is : " << signature << std::endl;

        // auto const &feynm = results.get<int>("L");
        auto const& feyn_sum = results.get<std::string>("sum");

        std::cout << "feynman_degree: " << feyn_sum << std::endl;

        std::cout << std::endl;
        /*  auto const& vectors = results.get_all<std::string>("fey_out", 40); // Assuming _N is the expected count

         for (const auto& vec : vectors)
         {
             std::cout << "vectors: " << vec << std::endl;
         }
  */
        return EXIT_SUCCESS;
    }
}