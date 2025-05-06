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
    unsigned long partialBinomialSum(int n, int d) {
        unsigned long sum = 0;

        for (int i = 1; i <= d; ++i) {
            sum += binomial(i + n - 1, i); // Calculate and add C(i+n-1, i)
        }

        return sum;
    }
    std::vector<int> stringToVectorInt(const std::string& str) {
        std::vector<int> result;
        std::stringstream ss(str);
        int num;
        while (ss >> num) {
            result.push_back(num);
        }
        return result;
    }
    // Function to sum a vector of integers
    int sumVector(const std::vector<int>& vec) {
        int sum = 0;
        for (int num : vec) {
            sum += num;
        }
        return sum;
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
        workflow_opts.add_options()("g", po::value<int>()->required());
        workflow_opts.add_options()("loop", po::value<std::string>()->required());
        workflow_opts.add_options()("genus", po::value<std::string>()->required());
        workflow_opts.add_options()("deg", po::value<int>()->default_value(0));

        return workflow_opts;
    }

    Workflow::Workflow(Parameters const& args)
        : _N(args.at("N").as<int>()) // number of edges.
        , _degree(args.at("degree").as<int>()) // given degree
        , _graph(args.at("graph").as<std::string>()) // graph.
        , _g(args.at("g").as<int>()) // graph.
        , _deg(args.at("deg").as<int>()) // given degree
        , _genus(args.at("genus").as<std::string>()) // given degree
        , _loop(args.at("loop").as<std::string>()) // graph.

    {
        std::vector<int> genusVector = stringToVectorInt(std::get<std::string>(_genus));

        // Sum the integers in the vector
        int gs = sumVector(genusVector);
        G = extractIntegers(_graph);
        for (int xi : G)
        {
            graph_int.emplace_back(pnet_value(xi));
        }
        deg_int = _degree;
        genus_int = 2 * (_N + gs);
        total_int = partialBinomialSum(_N, _degree);

    }

    ValuesOnPorts Workflow::inputs() const
    {
        ValuesOnPorts::Map values_on_ports;
        values_on_ports.emplace("N", _N);
        values_on_ports.emplace("degree", _degree);
        values_on_ports.emplace("graph", graph_int);
        values_on_ports.emplace("g", genus_int);
        values_on_ports.emplace("loop", _loop);
        values_on_ports.emplace("deg", deg_int);
        values_on_ports.emplace("genus", _genus);
        values_on_ports.emplace("total", total_int);

        return values_on_ports;
    }

    int Workflow::process(WorkflowResult const& results) const
    {
        // auto const &signature = results.get<int>("s");

        //  std::cout << " signature is : " << signature << std::endl;

        // auto const &feynm = results.get<int>("L");

        auto const& feynm = results.get<std::string>("answer");
        std::cout << "feynman_degree: " << feynm << std::endl;

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