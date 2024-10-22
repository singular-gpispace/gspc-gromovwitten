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
    unsigned long  binomial(const int n, const int k) {
        std::vector<unsigned long > vec(k);
        vec[0] = n - k + 1;

        for (int i = 1; i < k; ++i) {
            vec[i] = vec[i - 1] * (n - k + 1 + i) / (i + 1);
        }

        return vec[k - 1];
    }
    // Function to calculate the partial sum of binomial coefficients S(d, n)
    unsigned long partialBinomialSum(int n, int d) {
        unsigned long sum = 0;

        for (int i = 2; i <= d; ++i) {
            sum += binomial(i + n - 1, i); // Calculate and add C(i+n-1, i)
        }

        return sum;
    }
    using pnet_value = pnet::type::value::value_type;
    using pnet_list = std::list<pnet_value>;
    using pnet_list2d = std::list<std::list<pnet_value>>;

    ParametersDescription Workflow::options()
    {
        namespace po = boost::program_options;

        ParametersDescription workflow_opts("Workflow");
        workflow_opts.add_options()("N", po::value<int>()->required()); // edges
        workflow_opts.add_options()("degree", po::value<int>()->required());
        workflow_opts.add_options()("graph", po::value<std::string>()->required());
        workflow_opts.add_options()("deg", po::value<int>()->default_value(0));

        return workflow_opts;
    }

    Workflow::Workflow(Parameters const& args)
        : _N(args.at("N").as<int>()),         // number of edges
        _degree(args.at("degree").as<int>()), // given degree
        _deg(args.at("deg").as<int>()), // given degree
        _graph(args.at("graph").as<std::string>())   // graph

    {
        total_int = partialBinomialSum(_N, _degree);
        G = extractIntegers(_graph);
        for (int xi : G)
        {
            graph_int.emplace_back(pnet_value(xi));
        }
        deg_int = _degree;
    }

    ValuesOnPorts Workflow::inputs() const
    {
        ValuesOnPorts::Map values_on_ports;
        values_on_ports.emplace("N", _N);
        values_on_ports.emplace("degree", _degree);
        values_on_ports.emplace("deg", deg_int);
        values_on_ports.emplace("graph", graph_int);
        values_on_ports.emplace("total", total_int);

        return values_on_ports;
    }

    int Workflow::process(WorkflowResult const& results) const
    {

        /*   auto const& fey = results.get<std::string>("state");
          std::cout << "count: " << fey << std::endl; */

        auto const& feynm = results.get<std::string>("answer");
        std::cout << "feynman_degree: " << feynm << std::endl;

        /*
               auto const& feysum = results.get<unsigned long>("fey_sum");
               std::cout << "feynman_degree_sum: " << feysum << std::endl;
               // auto const &feynm = results.get<int>("L");

               auto const& vectors = results.get_all<std::string>("state", 1); // Assuming _N is the expected count

               for (const auto& vec : vectors)
               {
                   std::cout << "state: " << vec << "   ";
               }
               std::cout << std::endl;
        */
        return EXIT_SUCCESS;
    }
}