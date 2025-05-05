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
PetriNet generate_feynman_petri_net(int d) {
    PetriNet net;

    // Define places
    net.places = {
        "degree", "deg", "N", "graph", "total", "answer",
        "ns", "counter_total", "fey_sum"
    };
    for (int i = 2; i <= d; i++) {
        net.places.push_back("control_" + std::to_string(i));
        net.places.push_back("d_vectors_" + std::to_string(i));
        net.places.push_back("branchtype_" + std::to_string(i));
        net.places.push_back("fey_out_" + std::to_string(i));
        net.places.push_back("sum_" + std::to_string(i));
    }

    // Define place types
    net.place_types = {
        {"degree", "int"},
        {"deg", "int"},
        {"N", "int"},
        {"graph", "list"},
        {"total", "unsigned long"},
        {"answer", "string"},
        {"ns", "control"},
        {"counter_total", "unsigned long"},
        {"fey_sum", "string"}
    };
    for (int i = 2; i <= d; i++) {
        net.place_types["control_" + std::to_string(i)] = "control";
        net.place_types["d_vectors_" + std::to_string(i)] = "list";
        net.place_types["branchtype_" + std::to_string(i)] = "list";
        net.place_types["fey_out_" + std::to_string(i)] = "string";
        net.place_types["sum_" + std::to_string(i)] = "string";
    }

    // Define port types
    net.port_types = {
        {"i", "int"},
        {"n", "int"},
        {"v", "list"},
        {"vec", "list"},
        {"deg", "int"},
        {"a", "list"},
        {"G", "list"},
        {"s", "string"},
        {"sum", "string"},
        {"ns", "control"},
        {"counter_total", "unsigned long"},
        {"total", "unsigned long"},
        {"fey_sum", "string"},
        {"answer", "string"}
    };
    for (int i = 2; i <= d; i++) {
        net.port_types["control_" + std::to_string(i)] = "control";
        net.port_types["sum_" + std::to_string(i)] = "string";
    }

    // Define transitions
    net.transitions = {"sum_number", "equality", "Quasimodular"};
    for (int i = 2; i <= d; i++) {
        net.transitions.push_back("partition_" + std::to_string(i));
        net.transitions.push_back("Combination_" + std::to_string(i));
        net.transitions.push_back("feynman_degree_" + std::to_string(i));
        net.transitions.push_back("Reduce_" + std::to_string(i));
    }

    // Define transition inputs
    net.transition_inputs = {
        {"sum_number", {"ns"}},
        {"Quasimodular", {"fey_sum"}}
    };
    std::vector<std::string> equality_inputs = {"total", "counter_total"};
    for (int i = 2; i <= d; i++) {
        equality_inputs.push_back("sum_" + std::to_string(i));
    }
    net.transition_inputs["equality"] = equality_inputs;
    for (int i = 2; i <= d; i++) {
        std::string is = std::to_string(i);
        net.transition_inputs["partition_" + is] = {"control_" + is, "i", "n"};
        net.transition_inputs["Combination_" + is] = {"v"};
        net.transition_inputs["feynman_degree_" + is] = {"deg", "a", "G"};
        net.transition_inputs["Reduce_" + is] = {"s"};
    }

    // Define transition outputs
    net.transition_outputs = {
        {"sum_number", {}},
        {"equality", {"fey_sum"}},
        {"Quasimodular", {"answer"}}
    };
    for (int i = 2; i <= d; i++) {
        std::string is = std::to_string(i);
        net.transition_outputs["partition_" + is] = {"v"};
        net.transition_outputs["Combination_" + is] = {"vec"};
        net.transition_outputs["feynman_degree_" + is] = {"s"};
        net.transition_outputs["Reduce_" + is] = {"ns"};
    }

    // Define transition inouts
    net.transition_inouts = {
        {"sum_number", {"counter_total"}},
        {"equality", {}}
    };
    for (int i = 2; i <= d; i++) {
        net.transition_inouts["Reduce_" + std::to_string(i)] = {"sum"};
    }

    // Define function parameters
    net.function_params = {
        {"sum_number", {"ns", "counter_total"}},
        {"Quasimodular", {"fey_sum", "answer"}}
    };
    std::vector<std::string> equality_params = {"total", "counter_total", "fey_sum"};
    for (int i = 2; i <= d; i++) {
        equality_params.push_back("sum_" + std::to_string(i));
    }
    net.function_params["equality"] = equality_params;
    for (int i = 2; i <= d; i++) {
        std::string is = std::to_string(i);
        net.function_params["partition_" + is] = {"control_" + is, "i", "n", "v"};
        net.function_params["Combination_" + is] = {"v", "vec"};
        net.function_params["feynman_degree_" + is] = {"deg", "a", "G", "s"};
        net.function_params["Reduce_" + is] = {"s", "sum", "ns"};
    }

    // Define function signatures
    net.function_signatures = {
        {"sum_number", "control1 (ns,counter_total)"},
        {"equality", "final (total,counter_total,fey_sum" + [&]() {
            std::string s;
            for (int i = 2; i <= d; i++) s += ",sum_" + std::to_string(i);
            return s;
        }() + ")"},
        {"Quasimodular", "Quasi (fey_sum,answer)"}
    };
    for (int i = 2; i <= d; i++) {
        std::string is = std::to_string(i);
        net.function_signatures["partition_" + is] = "gen_block_" + is + "(n,i,v,control_" + is + ")";
        net.function_signatures["Combination_" + is] = "partition_" + is + "(v, vec)";
        net.function_signatures["feynman_degree_" + is] = "feynman_integral_degree_" + is + "(deg,s,G,a)";
        net.function_signatures["Reduce_" + is] = "plus_" + is + " (ns,s, sum)";
    }

    // Define includes for each transition
    net.includes = {
        {"sum_number", {"iostream", "vector", "sstream"}},
        {"equality", {"iostream", "vector", "sstream", "feynman/feynman.hpp"}},
        {"Quasimodular", {"iostream", "vector", "sstream", "Quasi.hpp", "feynman.hpp"}}
    };
    for (int i = 2; i <= d; i++) {
        std::string is = std::to_string(i);
        net.includes["partition_" + is] = {"iostream", "vector", "sstream", "feynman.hpp"};
        net.includes["Combination_" + is] = {"iostream", "vector", "numeric", "sstream", "../include/feynman/feynman.hpp"};
        net.includes["feynman_degree_" + is] = {
            "iostream", "vector", "numeric", "tuple", "algorithm",
            "unordered_map", "functional", "cmath", "sstream",
            "stack", "unordered_set", "feynman.hpp"
        };
        net.includes["Reduce_" + is] = {
            "iostream", "vector", "tuple", "algorithm",
            "unordered_map", "functional", "cmath", "sstream",
            "stack", "unordered_set", "feynman.hpp"
        };
    }

    // Define code blocks for each transition
    net.code_blocks = {
        {"sum_number", R"(
          counter_total +=1;
          //std::cout << " sum_number counter_total: " << counter_total << std::endl;
        )"},
        {"equality", R"(
          //std::cout << " equality total: " << total << std::endl;
          std::vector<std::string> sum_vec;
          )" + [&]() {
            std::string s;
            for (int i = 2; i <= d; i++) {
                s += "sum_vec.push_back(sum_" + std::to_string(i) + ");\n          ";
            }
            return s;
        }() + R"(
          std::string sum_vec_str = vectorToString(sum_vec," ");
          //std::cout << " equality sum_vec_str: " << sum_vec_str << std::endl;
          fey_sum = sum_vec_str;
        )"},
        {"Quasimodular", R"(
          answer = fey_sum;
          //std::cout << " Quasimodular answer: " << answer << std::endl;
        )"}
    };
    for (int i = 2; i <= d; i++) {
        std::string is = std::to_string(i);
        net.code_blocks["partition_" + is] = R"(
          vector2d gen=gen_block(n,)" + is + R"();
          for (std::vector<int> ge:gen){
              std::list<pnet::type::value::value_type> temp;
              for (int xi : ge) {
                  temp.push_back(pnet::type::value::value_type(xi));
              }
              v.push_back(temp);
          }
        )";
        net.code_blocks["Combination_" + is] = R"(
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
        )";
        net.code_blocks["feynman_degree_" + is] = R"(
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
          s=std::to_string(fe);
          //std::cout << " feynman_degree_)" + is + R"( s: " << s << std::endl;
        )";
        net.code_blocks["Reduce_" + is] = R"(
          unsigned long s1 = std::stoul(s);
          //std::cout << " Reduce_)" + is + R"( s1: " << s1 << std::endl;
          unsigned long s2 = std::stoul(sum);
          //std::cout << " Reduce_)" + is + R"( s2: " << s2 << std::endl;
          unsigned long fe = s1 + s2;
          sum = std::to_string(fe);
          //std::cout << " Reduce_)" + is + R"( sum: " << sum << std::endl;
        )";
    }

    return net;
}

// Generate XML for Feynman workflow
std::string generateFeynmanXPNetXML(const PetriNet& net, int d) {
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

    // Declare base places
    for (const auto& place : net.places) {
        xml << "    <place name=\"" << place << "\" type=\"" << net.place_types.at(place) << "\"";
        if (place == "counter_total" || place.find("sum_") == 0 || place.find("control_") == 0) {
            xml << ">\n";
            xml << "      <token>\n";
            if (place == "counter_total") {
                xml << "        <value>0UL</value>\n";
            } else if (place.find("sum_") == 0) {
                xml << "        <value>\"0\"</value>\n";
            } else {
                xml << "        <value>[]</value>\n";
            }
            xml << "      </token>\n";
            xml << "    </place>\n";
        } else {
            xml << "/>\n";
        }
    }
    xml << "\n";

    // Generate transitions for each degree from 2 to d
    for (int i = 2; i <= d; i++) {
        std::string is = std::to_string(i);

        // Partition transition
        xml << "    <transition name=\"partition_" << is << "\">\n";
        xml << "      <defun>\n";
        for (const auto& param : net.function_params.at("partition_" + is)) {
            std::string ptype = net.port_types.at(param);
            if (param == "v") {
                xml << "        <out name=\"" << param << "\" type=\"" << ptype << "\"/>\n";
            } else {
                xml << "        <in name=\"" << param << "\" type=\"" << ptype << "\"/>\n";
            }
        }
        xml << "        <module name=\"feynman_module\" function=\"" << net.function_signatures.at("partition_" + is) << "\">\n";
        for (const auto& inc : net.includes.at("partition_" + is)) {
            xml << "          <cinclude href=\"" << inc << "\"/>\n";
        }
        xml << "          <code><![CDATA[" << net.code_blocks.at("partition_" + is);
        xml << "          ]]>\n";
        xml << "          </code>\n";
        xml << "        </module>\n";
        xml << "      </defun>\n";
        xml << "      <connect-read port=\"i\" place=\"degree\"/>\n";
        xml << "      <connect-read port=\"n\" place=\"N\"/>\n";
        xml << "      <connect-in port=\"control_" << is << "\" place=\"control_" << is << "\"/>\n";
        xml << "      <connect-out-many port=\"v\" place=\"d_vectors_" << is << "\"/>\n";
        xml << "    </transition>\n\n";

        // Combination transition
        xml << "    <transition name=\"Combination_" << is << "\">\n";
        xml << "      <defun>\n";
        for (const auto& param : net.function_params.at("Combination_" + is)) {
            std::string ptype = net.port_types.at(param);
            if (param == "vec") {
                xml << "        <out name=\"" << param << "\" type=\"" << ptype << "\"/>\n";
            } else {
                xml << "        <in name=\"" << param << "\" type=\"" << ptype << "\"/>\n";
            }
        }
        xml << "        <module name=\"feynman_module\" function=\"" << net.function_signatures.at("Combination_" + is) << "\">\n";
        for (const auto& inc : net.includes.at("Combination_" + is)) {
            xml << "          <cinclude href=\"" << inc << "\"/>\n";
        }
        xml << "          <code><![CDATA[" << net.code_blocks.at("Combination_" + is);
        xml << "         ]]>\n";
        xml << "          </code>\n";
        xml << "        </module>\n";
        xml << "      </defun>\n";
        xml << "      <connect-in port=\"v\" place=\"d_vectors_" << is << "\"/>\n";
        xml << "      <connect-out-many port=\"vec\" place=\"branchtype_" << is << "\"/>\n";
        xml << "    </transition>\n\n";

        // Feynman degree transition
        xml << "    <transition name=\"feynman_degree_" << is << "\">\n";
        xml << "      <defun>\n";
        for (const auto& param : net.function_params.at("feynman_degree_" + is)) {
            std::string ptype = net.port_types.at(param);
            if (param == "s") {
                xml << "        <out name=\"" << param << "\" type=\"" << ptype << "\"/>\n";
            } else {
                xml << "        <in name=\"" << param << "\" type=\"" << ptype << "\"/>\n";
            }
        }
        xml << "        <module name=\"feynman_module\" function=\"" << net.function_signatures.at("feynman_degree_" + is) << "\">\n";
        for (const auto& inc : net.includes.at("feynman_degree_" + is)) {
            xml << "          <cinclude href=\"" << inc << "\"/>\n";
        }
        xml << "          <code><![CDATA[" << net.code_blocks.at("feynman_degree_" + is);
        xml << "         ]]>\n";
        xml << "          </code>\n";
        xml << "        </module>\n";
        xml << "      </defun>\n";
        xml << "      <connect-read port=\"deg\" place=\"deg\"/>\n";
        xml << "      <connect-in port=\"a\" place=\"branchtype_" << is << "\"/>\n";
        xml << "      <connect-read port=\"G\" place=\"graph\"/>\n";
        xml << "      <connect-out port=\"s\" place=\"fey_out_" << is << "\"/>\n";
        xml << "    </transition>\n\n";

        // Reduce transition
        xml << "    <transition name=\"Reduce_" << is << "\">\n";
        xml << "      <defun>\n";
        for (const auto& param : net.function_params.at("Reduce_" + is)) {
            std::string ptype = net.port_types.at(param);
            if (param == "sum") {
                xml << "        <inout name=\"" << param << "\" type=\"" << ptype << "\"/>\n";
            } else if (param == "ns") {
                xml << "        <out name=\"" << param << "\" type=\"" << ptype << "\"/>\n";
            } else {
                xml << "        <in name=\"" << param << "\" type=\"" << ptype << "\"/>\n";
            }
        }
        xml << "        <module name=\"feynman_module\" function=\"" << net.function_signatures.at("Reduce_" + is) << "\">\n";
        for (const auto& inc : net.includes.at("Reduce_" + is)) {
            xml << "          <cinclude href=\"" << inc << "\"/>\n";
        }
        xml << "          <code><![CDATA[" << net.code_blocks.at("Reduce_" + is);
        xml << "         ]]>\n";
        xml << "          </code>\n";
        xml << "        </module>\n";
        xml << "      </defun>\n";
        xml << "      <connect-in port=\"s\" place=\"fey_out_" << is << "\"/>\n";
        xml << "      <connect-inout port=\"sum\" place=\"sum_" << is << "\"/>\n";
        xml << "      <connect-out port=\"ns\" place=\"ns\"/>\n";
        xml << "    </transition>\n\n";
    }

    // Add sum_number transition
    xml << "    <transition name=\"sum_number\">\n";
    xml << "      <defun>\n";
    for (const auto& param : net.function_params.at("sum_number")) {
        std::string ptype = net.port_types.at(param);
        if (param == "counter_total") {
            xml << "        <inout name=\"" << param << "\" type=\"" << ptype << "\"/>\n";
        } else {
            xml << "        <in name=\"" << param << "\" type=\"" << ptype << "\"/>\n";
        }
    }
    xml << "        <module name=\"feynman_module\" function=\"" << net.function_signatures.at("sum_number") << "\">\n";
    for (const auto& inc : net.includes.at("sum_number")) {
        xml << "          <cinclude href=\"" << inc << "\"/>\n";
    }
    xml << "          <code><![CDATA[" << net.code_blocks.at("sum_number");
    xml << "         ]]>\n";
    xml << "          </code>\n";
    xml << "        </module>\n";
    xml << "      </defun>\n";
    xml << "      <connect-in port=\"ns\" place=\"ns\"/>\n";
    xml << "      <connect-inout port=\"counter_total\" place=\"counter_total\"/>\n";
    xml << "    </transition>\n\n";

    // Add equality transition
    xml << "    <transition name=\"equality\">\n";
    xml << "      <defun>\n";
    for (const auto& param : net.function_params.at("equality")) {
        std::string ptype = net.port_types.at(param);
        if (param == "fey_sum") {
            xml << "        <out name=\"" << param << "\" type=\"" << ptype << "\"/>\n";
        } else {
            xml << "        <in name=\"" << param << "\" type=\"" << ptype << "\"/>\n";
        }
    }
    xml << "        <module name=\"feynman_module\" function=\"" << net.function_signatures.at("equality") << "\">\n";
    for (const auto& inc : net.includes.at("equality")) {
        xml << "          <cinclude href=\"" << inc << "\"/>\n";
    }
    xml << "          <code><![CDATA[" << net.code_blocks.at("equality");
    xml << "         ]]>\n";
    xml << "          </code>\n";
    xml << "        </module>\n";
    xml << "      </defun>\n";
    xml << "      <connect-read port=\"total\" place=\"total\"/>\n";
    xml << "      <connect-read port=\"counter_total\" place=\"counter_total\"/>\n";
    xml << "      <connect-out port=\"fey_sum\" place=\"fey_sum\"/>\n";
    for (int i = 2; i <= d; i++) {
        xml << "      <connect-in port=\"sum_" << i << "\" place=\"sum_" << i << "\"/>\n";
    }
    xml << "      <condition>\n";
    xml << "      ${counter_total}:eq: ${total}\n";
    xml << "      </condition>\n";
    xml << "    </transition>\n\n";

    // Add Quasimodular transition
    xml << "    <transition name=\"Quasimodular\">\n";
    xml << "      <defun>\n";
    for (const auto& param : net.function_params.at("Quasimodular")) {
        std::string ptype = net.port_types.at(param);
        if (param == "answer") {
            xml << "        <out name=\"" << param << "\" type=\"" << ptype << "\"/>\n";
        } else {
            xml << "        <in name=\"" << param << "\" type=\"" << ptype << "\"/>\n";
        }
    }
    xml << "        <module name=\"feynman_module\" function=\"" << net.function_signatures.at("Quasimodular") << "\">\n";
    for (const auto& inc : net.includes.at("Quasimodular")) {
        xml << "          <cinclude href=\"" << inc << "\"/>\n";
    }
    xml << "          <code><![CDATA[" << net.code_blocks.at("Quasimodular");
    xml << "         ]]>\n";
    xml << "          </code>\n";
    xml << "        </module>\n";
    xml << "      </defun>\n";
    xml << "      <connect-in port=\"fey_sum\" place=\"fey_sum\"/>\n";
    xml << "      <connect-out port=\"answer\" place=\"answer\"/>\n";
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
    int d = 22; // Can be any positive integer >= 2
    if (d < 2) {
        std::cerr << "Error: d must be at least 2\n";
        return 1;
    }
    PetriNet myPetriNet = generate_feynman_petri_net(d);
    //print_petri_net(myPetriNet);

    std::string xml = generateFeynmanXPNetXML(myPetriNet, d);
    std::ofstream file("feynman.xpnet");
    file << xml;
    file.close();

    return 0;
}