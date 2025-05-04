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
PetriNet generate_feynman_petri_net() {
    PetriNet net;

    // Define places
    net.places = {
        "degree", "deg", "N", "graph", "total", "fey_out",
        "d_vectors", "branchtype", "sum", "ns", "counter_total", "fey_sum", "answer"
    };

    // Define place types
    net.place_types = {
        {"degree", "int"},
        {"deg", "int"},
        {"N", "int"},
        {"graph", "list"},
        {"total", "unsigned long"},
        {"fey_out", "string"},
        {"d_vectors", "list"},
        {"branchtype", "list"},
        {"sum", "string"},
        {"ns", "control"},
        {"counter_total", "unsigned long"},
        {"fey_sum", "string"},
        {"answer", "string"}
    };

    // Define port types
    net.port_types = {
        {"d", "int"},
        {"v", "list"},
        {"i", "int"},
        {"n", "int"},
        {"vec", "list"},
        {"deg", "int"},
        {"a", "list"},
        {"G", "list"},
        {"s", "string"},
        {"sum", "string"},
        {"ns", "control"},
        {"counter_total", "unsigned long"},
        {"total", "unsigned long"},
        {"answer", "string"},
        {"fey_sum", "string"}
    };

    // Define function parameters
    net.function_params = {
        {"partition", {"i", "n", "v"}},
        {"Combination", {"v", "vec"}},
        {"feynman_degree", {"deg", "a", "G", "s"}},
        {"Reduce", {"s", "sum", "ns"}},
        {"sum_number", {"ns", "counter_total"}},
        {"equality", {"total", "counter_total", "fey_sum"}},
        {"Quasimodular", {"fey_sum", "answer"}}
    };

    // Define function signatures
    net.function_signatures = {
        {"partition", "gen_block_(n,i,v)"},
        {"Combination", "partition(v, vec)"},
        {"feynman_degree", "feynman_integral_degree(deg,s,G,a)"},
        {"Reduce", "plus (ns,s, sum)"},
        {"sum_number", "control1 (ns,counter_total)"},
        {"equality", "final (total,counter_total,fey_sum)"},
        {"Quasimodular", "Quasi (fey_sum,answer)"}
    };

    // Define includes for each transition
    net.includes = {
        {"partition", {"iostream", "vector", "sstream", "feynman.hpp"}},
        {"Combination", {"iostream", "vector", "numeric", "sstream", "../include/feynman/feynman.hpp"}},
        {"feynman_degree", {"iostream", "vector", "numeric", "tuple", "algorithm", "unordered_map", "functional", "cmath", "sstream", "stack", "unordered_set", "feynman.hpp"}},
        {"Reduce", {"iostream", "vector", "tuple", "algorithm", "unordered_map", "functional", "cmath", "sstream", "stack", "unordered_set", "feynman.hpp"}},
        {"sum_number", {"iostream", "vector", "sstream"}},
        {"equality", {"iostream", "vector", "sstream"}},
        {"Quasimodular", {"iostream", "vector", "sstream", "Quasi.hpp", "feynman.hpp"}}
    };

    // Define code blocks for each transition
    net.code_blocks = {
        {"partition", R"(
          vector2d gen=gen_block(n,i);
          for (std::vector<int> ge:gen){
              std::list<pnet::type::value::value_type> temp;
              for (int xi : ge) {
                  temp.push_back(pnet::type::value::value_type(xi));
              }
              v.push_back(temp);
          }
        )"},
        {"Combination", R"(
          std::vector<int> x;
          for (const auto& elem : v) {
            auto ptr = boost::get<int>(&elem) ;
              x.push_back(*ptr);
          }
          vector2d gen=iterate( x);
          using pnet_value = pnet::type::value::value_type;
          using pnet_list = std::list<pnet_value>;
          for (std::vector<int> &a : gen)
          {
              pnet_list temp_a;
              for (int xi : a)
              {
                  temp_a.push_back(pnet::type::value::value_type(xi));
              }
              vec.push_back(temp_a);
          }
        )"},
        {"feynman_degree", R"(
          std::vector<int> xxx;
          for (const auto &vii : G)
          {
              if (auto ptr = boost::get<int>(&vii))
              {
                  xxx.push_back( *ptr );
              }
          }
          std::vector<std::pair<int, int>> Gv;
          for (size_t i = 0; i < xxx.size(); i += 2)
          {
              Gv.push_back(std::make_pair(xxx[i], xxx[i + 1]));
          }
          std::vector<int> av;
          int c=0;
          for (const auto &xi : a)
          {
              if (auto ptr = boost::get<int>(&xi))
              {
                  av.push_back( *ptr);
                  c+=*ptr;
              }
          }
          unsigned long fe=feynman_integral_branch_type(Gv, av);
          std::vector<unsigned long> v(deg, 0);
          v[c-1] = fe;
          s=vectorToStringULong(v);
        )"},
        {"Reduce", R"(
          std::vector<unsigned long> v = stringToVectorUlong(sum);
          std::vector<unsigned long> w = stringToVectorUlong(s);
          std::vector<unsigned long> sum_vector = sumOfVectors(v, w);
          sum = vectorToStringULong(sum_vector);
        )"},
        {"sum_number", R"(
          counter_total +=1;
        )"},
        {"equality", ""},
        {"Quasimodular", ""}
    };

    // Define transitions
    net.transitions = {
        "partition", "Combination", "feynman_degree", "Reduce",
        "sum_number", "equality", "Quasimodular"
    };

    // Define transition inputs
    net.transition_inputs = {
        {"partition", {"i", "n"}},
        {"Combination", {"v"}},
        {"feynman_degree", {"deg", "a", "G"}},
        {"Reduce", {"s"}},
        {"sum_number", {"ns"}},
        {"equality", {"total", "counter_total"}},
        {"Quasimodular", {"fey_sum"}}
    };

    // Define transition outputs
    net.transition_outputs = {
        {"partition", {"v"}},
        {"Combination", {"vec"}},
        {"feynman_degree", {"s"}},
        {"Reduce", {"ns"}},
        {"sum_number", {}},
        {"equality", {"fey_sum"}},
        {"Quasimodular", {"answer"}}
    };

    // Define transition inouts
    net.transition_inouts = {
        {"Reduce", {"sum"}},
        {"sum_number", {"counter_total"}},
        {"equality", {}}
    };

    return net;
}

// Generate XML for Feynman workflow
std::string generateFeynmanXPNetXML(const PetriNet& net) {
    std::ostringstream xml;

    // XML header
    xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml << "<defun name=\"feynman_deg\">\n\n";

    // Top-level inputs and output
    xml << "  <in name=\"degree\" type=\"int\" place=\"degree\"/>\n";
    xml << "  <in name=\"deg\" type=\"int\" place=\"deg\"/>\n";
    xml << "  <in name=\"N\" type=\"int\" place=\"N\"/>\n";
    xml << "  <in name=\"graph\" type=\"list\" place=\"graph\"/>\n";
    xml << "  <in name=\"total\" type=\"unsigned long\" place=\"total\"/>\n";
    xml << "  <out name=\"answer\" type=\"string\" place=\"answer\"/>\n\n";
    xml << "  <net>\n";

    // Declare places
    for (const auto& place : net.places) {
        xml << "    <place name=\"" << place << "\" type=\"" << net.place_types.at(place) << "\"";
        if (place == "sum" || place == "counter_total") {
            xml << ">\n";
            xml << "      <token>\n";
            xml << "        <value>" << (place == "sum" ? "\" \"" : "0UL") << "</value>\n";
            xml << "      </token>\n";
            xml << "    </place>\n";
        } else {
            xml << "/>\n";
        }
    }
    xml << "\n";

    // Generate transitions
    for (const auto& transition : net.transitions) {
        xml << "    <transition name=\"" << transition << "\">\n";
        xml << "      <defun>\n";

        // Port declarations
        const auto& params = net.function_params.at(transition);
        for (const auto& param : params) {
            if (std::find(net.transition_inputs.at(transition).begin(),
                          net.transition_inputs.at(transition).end(),
                          param) != net.transition_inputs.at(transition).end()) {
                xml << "        <in name=\"" << param << "\" type=\"" << net.port_types.at(param) << "\"/>\n";
            }
            if (std::find(net.transition_outputs.at(transition).begin(),
                          net.transition_outputs.at(transition).end(),
                          param) != net.transition_outputs.at(transition).end()) {
                xml << "        <out name=\"" << param << "\" type=\"" << net.port_types.at(param) << "\"/>\n";
            }
            if (net.transition_inouts.count(transition) &&
                std::find(net.transition_inouts.at(transition).begin(),
                          net.transition_inouts.at(transition).end(),
                          param) != net.transition_inouts.at(transition).end()) {
                xml << "        <inout name=\"" << param << "\" type=\"" << net.port_types.at(param) << "\"/>\n";
            }
        }

        xml << "        <module name=\"feynman_module\" function=\"" << net.function_signatures.at(transition) << "\">\n";

        // Includes
        for (const auto& inc : net.includes.at(transition)) {
            xml << "          <cinclude href=\"" << inc << "\"/>\n";
        }

        // Code block
        xml << "          <code><![CDATA[" << net.code_blocks.at(transition);
        xml << "         ]]>\n";
        xml << "          </code>\n";
        xml << "        </module>\n";
        xml << "      </defun>\n";

        // Connections
        for (const auto& input : net.transition_inputs.at(transition)) {
            if (input == "d") {
                xml << "      <connect-in port=\"" << input << "\" place=\"degree\"/>\n";
            } else if (input == "i") {
                xml << "      <connect-in port=\"" << input << "\" place=\"d_vectors\"/>\n";
            } else if (input == "n") {
                xml << "      <connect-read port=\"" << input << "\" place=\"N\"/>\n";
            } else if (input == "v") {
                xml << "      <connect-in port=\"" << input << "\" place=\"d_vectors\"/>\n";
            } else if (input == "deg") {
                xml << "      <connect-read port=\"" << input << "\" place=\"deg\"/>\n";
            } else if (input == "a") {
                xml << "      <connect-in port=\"" << input << "\" place=\"branchtype\"/>\n";
            } else if (input == "G") {
                xml << "      <connect-read port=\"" << input << "\" place=\"graph\"/>\n";
            } else if (input == "s") {
                xml << "      <connect-in port=\"" << input << "\" place=\"fey_out\"/>\n";
            } else if (input == "ns") {
                xml << "      <connect-in port=\"" << input << "\" place=\"ns\"/>\n";
            } else if (input == "total") {
                xml << "      <connect-read port=\"" << input << "\" place=\"total\"/>\n";
            } else if (input == "counter_total") {
                xml << "      <connect-in port=\"" << input << "\" place=\"counter_total\"/>\n";
            } else if (input == "fey_sum") {
                xml << "      <connect-in port=\"" << input << "\" place=\"fey_sum\"/>\n";
            }
        }
        for (const auto& output : net.transition_outputs.at(transition)) {
            if (output == "v" && transition == "partition") {
                xml << "      <connect-out-many port=\"" << output << "\" place=\"d_vectors\"/>\n";
            } else if (output == "vec") {
                xml << "      <connect-out-many port=\"" << output << "\" place=\"branchtype\"/>\n";
            } else if (output == "s") {
                xml << "      <connect-out port=\"" << output << "\" place=\"fey_out\"/>\n";
            } else if (output == "ns") {
                xml << "      <connect-out port=\"" << output << "\" place=\"ns\"/>\n";
            } else if (output == "fey_sum") {
                xml << "      <connect-out port=\"" << output << "\" place=\"fey_sum\"/>\n";
            } else if (output == "answer") {
                xml << "      <connect-out port=\"" << output << "\" place=\"answer\"/>\n";
            }
        }
        if (net.transition_inouts.count(transition)) {
            for (const auto& inout : net.transition_inouts.at(transition)) {
                if (inout == "sum") {
                    xml << "      <connect-inout port=\"" << inout << "\" place=\"sum\"/>\n";
                } else if (inout == "counter_total") {
                    xml << "      <connect-inout port=\"" << inout << "\" place=\"counter_total\"/>\n";
                }
            }
        }

        // Condition for equality
        if (transition == "equality") {
            xml << "      <condition>\n";
            xml << "      ${counter_total}:eq: ${total}\n";
            xml << "      </condition>\n";
        }

        xml << "    </transition>\n\n";
    }

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
    PetriNet myPetriNet = generate_feynman_petri_net();
    print_petri_net(myPetriNet);

    std::string xml = generateFeynmanXPNetXML(myPetriNet);
    std::ofstream file("generated_feynman.xpnet");
    file << xml;
    file.close();

    return 0;
}