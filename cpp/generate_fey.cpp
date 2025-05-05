#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <utility>
#include <string>
#include <algorithm>

// Define PetriNet structure for Feynman workflow
struct PetriNet {
    std::vector<std::string> places;
    std::map<std::string, std::string> place_types;
    std::vector<std::string> transitions;
    std::map<std::string, std::vector<std::string>> transition_inputs;
    std::map<std::string, std::vector<std::string>> transition_outputs;
    std::map<std::string, std::vector<std::string>> transition_inouts;
    std::map<std::string, std::string> port_types;
    std::map<std::string, std::vector<std::string>> function_params;
    std::map<std::string, std::string> function_signatures;
    std::map<std::string, std::vector<std::string>> includes;
    std::map<std::string, std::string> code_blocks;
};

// Generate Petri net for Feynman workflow
std::string generate_feynman_petri_net(int d) {
    PetriNet net;

    // Define places
    net.places = {
        "graph", "degree", "N", "g", "loop", "genus", "degrees", "d_vectors", "branchtype", "fey_out", "deg"
    };

    // Define place types
    net.place_types = {
        {"graph", "list"},
        {"degree", "int"},
        {"N", "int"},
        {"g", "int"},
        {"loop", "string"},
        {"genus", "string"},
        {"degrees", "int"},
        {"d_vectors", "list"},
        {"branchtype", "list"},
        {"fey_out", "string"},
        {"deg", "int"}
    };

    // Define transitions
    for (int i = 1; i <= d; i++) {
        std::string is = std::to_string(i);
        net.transitions.push_back("partition_" + is);
        net.transitions.push_back("combination_" + is);
    }
    net.transitions.push_back("degrees_fey");
    net.transitions.push_back("combine_results");

    // Define transition inputs and outputs
    for (int i = 1; i <= d; i++) {
        std::string is = std::to_string(i);
        net.transition_inputs["partition_" + is] = {"i", "n"};
        net.transition_outputs["partition_" + is] = {"v"};
        net.transition_inputs["combination_" + is] = {"v"};
        net.transition_outputs["combination_" + is] = {"vec"};
    }
    net.transition_inputs["degrees_fey"] = {"weightmax", "d"};
    net.transition_outputs["degrees_fey"] = {"v"};
    net.transition_inputs["combine_results"] = {"a", "G", "gg", "l", "deg"};
    net.transition_outputs["combine_results"] = {"s"};

    // Define function parameters
    for (int i = 1; i <= d; i++) {
        std::string is = std::to_string(i);
        net.function_params["partition_" + is] = {"n", "i", "v"};
        net.function_params["combination_" + is] = {"v", "vec"};
    }
    net.function_params["degrees_fey"] = {"weightmax", "d", "v"};
    net.function_params["combine_results"] = {"s", "G", "a", "gg", "l", "deg"};

    // Define port types
    for (int i = 1; i <= d; i++) {
        std::string is = std::to_string(i);
        net.port_types["n"] = "int";
        net.port_types["i"] = "int";
        net.port_types["v"] = "list";
        net.port_types["vec"] = "list";
    }
    net.port_types["weightmax"] = "int";
    net.port_types["d"] = "int";
    net.port_types["a"] = "list";
    net.port_types["G"] = "list";
    net.port_types["gg"] = "string";
    net.port_types["l"] = "string";
    net.port_types["deg"] = "int";
    net.port_types["s"] = "string";

    // Define function signatures
    for (int i = 1; i <= d; i++) {
        std::string is = std::to_string(i);
        net.function_signatures["partition_" + is] = "gen_block_(n,i,v)";
        net.function_signatures["combination_" + is] = "partition(v, vec)";
    }

    net.function_signatures["degrees_fey"] = "init(weightmax,d,v)";
    std::string combine_sig = "feynman_integral_degree(s,G,a,gg,l,deg)";
    net.function_signatures["combine_results"] = combine_sig;

    // Define includes
    for (int i = 1; i <= d; i++) {
        std::string is = std::to_string(i);
        net.includes["partition_" + is] = {"feynman.h"};
        net.includes["combination_" + is] = {"feynman.h"};
    }
    net.includes["degrees_fey"] = {"feynman.h"};
    net.includes["combine_results"] = {"feynman.h"};

    // Define code blocks
    for (int i = 1; i <= d; i++) {
        std::string is = std::to_string(i);
        net.code_blocks["partition_" + is] = R"(
          vector2d gen=gen_block(n,i);
          for (std::vector<int> ge:gen){
              std::list<pnet::type::value::value_type> temp;
              for (int xi : ge) {
                  temp.push_back(pnet::type::value::value_type(xi));
              }
              v.push_back(temp);
          })";

        net.code_blocks["combination_" + is] = R"(
          std::vector<int> x;
          for (const auto& elem : v) {
            auto ptr = boost::get<int>(&elem);
            x.push_back(*ptr);
          }
          vector2d gen=iterate(x);
          using pnet_value = pnet::type::value::value_type;
          using pnet_list = std::list<pnet_value>;
          for (std::vector<int> &a : gen) {
              pnet_list temp_a;
              for (int xi : a) {
                  temp_a.push_back(pnet::type::value::value_type(xi));
              }
              vec.push_back(temp_a);
          })";
    }

    net.code_blocks["degrees_fey"] = R"(
          int nb = number_monomial(weightmax);
          if (d < nb) {
              throw std::runtime_error(
                  "d = " + std::to_string(d) + " should be equal to or greater than the number of monomials, which is " + std::to_string(nb));
          } else {
              for (int i = 1; i <= d; ++i) {
                  v.emplace_back(i);
              }
          })";

    net.code_blocks["combine_results"] = R"(
          std::vector<int> xxx;
          for (const auto &vii : G) {
              if (auto ptr = boost::get<int>(&vii)) {
                  xxx.push_back(*ptr);
              }
          }
          std::vector<std::pair<int, int>> Gv;
          for (size_t i = 0; i < xxx.size(); i += 2) {
              Gv.push_back(std::make_pair(xxx[i], xxx[i + 1]));
          }
          std::vector<int> av;
          int c=0;
          for (const auto &xi : a) {
              if (auto ptr = boost::get<int>(&xi)) {
                  av.push_back(*ptr);
                  c+=*ptr;
              }
          }
          std::vector<int> g=stringToVectorInt(gg);
          std::vector<int> ll= stringToVectorInt(l);
          fmpq_t result;
          fmpq_init(result);
          fmpq_t fey_branch_av;
          fmpq_init(fey_branch_av);
          feynman_integral_branch_type(fey_branch_av, Gv, av, g, ll);
          s = fmpqToString(fey_branch_av);
          fmpq_clear(fey_branch_av);)";

    // Generate XML
    std::stringstream xml;
    xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml << "<defun name=\"feynman\">\n";
    xml << "  <net>\n";

    // Add places
    for (const auto& place : net.places) {
        xml << "    <place name=\"" << place << "\" type=\"" << net.place_types.at(place) << "\"/>\n";
    }
    xml << "\n";

    // Generate transitions for each degree from 1 to d
    for (int i = 1; i <= d; i++) {
        std::string is = std::to_string(i);

        // Partition transition
        xml << "    <transition name=\"partition_" << is << "\">\n";
        xml << "      <defun>\n";
        xml << "        <in name=\"i\" type=\"int\"/>\n";
        xml << "        <in name=\"n\" type=\"int\"/>\n";
        xml << "        <out name=\"v\" type=\"list\"/>\n";
        xml << "        <module name=\"feynman_module\" function=\"" << net.function_signatures.at("partition_" + is) << "\">\n";
        for (const auto& inc : net.includes.at("partition_" + is)) {
            xml << "          <cinclude href=\"" << inc << "\"/>\n";
        }
        xml << "          <code><![CDATA[" << net.code_blocks.at("partition_" + is) << "]]></code>\n";
        xml << "        </module>\n";
        xml << "      </defun>\n";
        xml << "      <connect-read port=\"n\" place=\"N\"/>\n";
        xml << "      <connect-in port=\"i\" place=\"degrees\"/>\n";
        xml << "      <connect-out-many port=\"v\" place=\"d_vectors\"/>\n";
        xml << "    </transition>\n\n";

        // Combination transition
        xml << "    <transition name=\"combination_" << is << "\">\n";
        xml << "      <defun>\n";
        xml << "        <in name=\"v\" type=\"list\"/>\n";
        xml << "        <out name=\"vec\" type=\"list\"/>\n";
        xml << "        <module name=\"feynman_module\" function=\"" << net.function_signatures.at("combination_" + is) << "\">\n";
        for (const auto& inc : net.includes.at("combination_" + is)) {
            xml << "          <cinclude href=\"" << inc << "\"/>\n";
        }
        xml << "          <code><![CDATA[" << net.code_blocks.at("combination_" + is) << "]]></code>\n";
        xml << "        </module>\n";
        xml << "      </defun>\n";
        xml << "      <connect-in port=\"v\" place=\"d_vectors\"/>\n";
        xml << "      <connect-out-many port=\"vec\" place=\"branchtype\"/>\n";
        xml << "    </transition>\n\n";
    }

    // Degrees transition
    xml << "    <transition name=\"degrees_fey\">\n";
    xml << "      <defun>\n";
    xml << "        <in name=\"weightmax\" type=\"int\"/>\n";
    xml << "        <in name=\"d\" type=\"int\"/>\n";
    xml << "        <out name=\"v\" type=\"list\"/>\n";
    xml << "        <module name=\"feynman_module\" function=\"" << net.function_signatures.at("degrees_fey") << "\">\n";
    for (const auto& inc : net.includes.at("degrees_fey")) {
        xml << "          <cinclude href=\"" << inc << "\"/>\n";
    }
    xml << "          <code><![CDATA[" << net.code_blocks.at("degrees_fey") << "]]></code>\n";
    xml << "        </module>\n";
    xml << "      </defun>\n";
    xml << "      <connect-read port=\"weightmax\" place=\"g\"/>\n";
    xml << "      <connect-in port=\"d\" place=\"degree\"/>\n";
    xml << "      <connect-out port=\"v\" place=\"degrees\"/>\n";
    xml << "    </transition>\n\n";

    // Add final transition to combine results
    xml << "    <transition name=\"combine_results\">\n";
    xml << "      <defun>\n";
    xml << "        <in name=\"a\" type=\"list\"/>\n";
    xml << "        <in name=\"G\" type=\"list\"/>\n";
    xml << "        <in name=\"gg\" type=\"string\"/>\n";
    xml << "        <in name=\"l\" type=\"string\"/>\n";
    xml << "        <in name=\"deg\" type=\"int\"/>\n";
    xml << "        <out name=\"s\" type=\"string\"/>\n";
    xml << "        <module name=\"feynman_module\" function=\"" << net.function_signatures.at("combine_results") << "\">\n";
    for (const auto& inc : net.includes.at("combine_results")) {
        xml << "          <cinclude href=\"" << inc << "\"/>\n";
    }
    xml << "          <code><![CDATA[" << net.code_blocks.at("combine_results") << "]]></code>\n";
    xml << "        </module>\n";
    xml << "      </defun>\n";
    xml << "      <connect-read port=\"G\" place=\"graph\"/>\n";
    xml << "      <connect-in port=\"a\" place=\"branchtype\"/>\n";
    xml << "      <connect-read port=\"l\" place=\"loop\"/>\n";
    xml << "      <connect-out port=\"s\" place=\"fey_out\"/>\n";
    xml << "      <connect-read port=\"gg\" place=\"genus\"/>\n";
    xml << "      <connect-read port=\"deg\" place=\"deg\"/>\n";
    xml << "    </transition>\n\n";

    xml << "  </net>\n";
    xml << "</defun>\n";

    return xml.str();
}

// Print Petri net details
void print_petri_net(const PetriNet& net) {
    std::cout << "\nGenerated Feynman PetriNet:\n";
    std::cout << "Places:\n";
    for (const auto& place : net.places) {
        std::cout << "  " << place << " (" << net.place_types.at(place) << ")\n";
    }
    std::cout << "\nTransitions:\n";
    for (const auto& t : net.transitions) {
        std::cout << "  " << t << "\n";
    }
    std::cout << "\nTransition Inputs:\n";
    for (const auto& [t, inputs] : net.transition_inputs) {
        std::cout << "  " << t << " <- { ";
        for (const auto& input : inputs) std::cout << input << " ";
        std::cout << "}\n";
    }
    std::cout << "\nTransition Outputs:\n";
    for (const auto& [t, outputs] : net.transition_outputs) {
        std::cout << "  " << t << " -> { ";
        for (const auto& output : outputs) std::cout << output << " ";
        std::cout << "}\n";
    }
    std::cout << "\nTransition Inouts:\n";
    for (const auto& [t, inouts] : net.transition_inouts) {
        std::cout << "  " << t << " <-> { ";
        for (const auto& inout : inouts) std::cout << inout << " ";
        std::cout << "}\n";
    }
}

// Main function
int main() {
    int d = 5;  // Example value for d
    std::string xml = generate_feynman_petri_net(d);
    std::ofstream file("feynman.xpnet");
    file << xml;
    file.close();

    return 0;
}