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
        "degree", "deg", "N", "graph", "total", "answer",
        "ns", "counter_total", "fey_sum"
    };

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

    // Generate places and transitions for each degree from 2 to d
    for (int i = 2; i <= d; i++) {
        // Declare control place for this degree
        xml << "    <place name=\"control_" << i << "\" type=\"control\">\n";
        xml << "      <token><value>[]</value></token>\n";
        xml << "    </place>\n\n";

        // Declare degree-specific places
        xml << "    <place name=\"d_vectors_" << i << "\" type=\"list\"/>\n";
        xml << "    <place name=\"branchtype_" << i << "\" type=\"list\"/>\n";
        xml << "    <place name=\"fey_out_" << i << "\" type=\"string\"/>\n";
        xml << "    <place name=\"sum_" << i << "\" type=\"string\">\n";
        xml << "      <token>\n";
        xml << "        <value>\"0\"</value>\n";
        xml << "      </token>\n";
        xml << "    </place>\n\n";

        // Partition transition for this degree
        xml << "    <transition name=\"partition_" << i << "\">\n";
        xml << "      <defun>\n";
        xml << "        <in name=\"control_" << i << "\" type=\"control\"/>\n";
        xml << "        <in name=\"i\" type=\"int\"/>\n";
        xml << "        <in name=\"n\" type=\"int\"/>\n";
        xml << "        <out name=\"v\" type=\"list\"/>\n";
        xml << "        <module name=\"feynman_module\" function=\"gen_block_" << i << "(n,i,v,control_" << i << ")\">\n";
        for (const auto& inc : net.includes.at("partition")) {
            xml << "          <cinclude href=\"" << inc << "\"/>\n";
        }
        xml << "          <code><![CDATA[\n";
        xml << "          vector2d gen=gen_block(n," << i << ");\n";
        xml << "          for (std::vector<int> ge:gen){\n";
        xml << "              std::list<pnet::type::value::value_type> temp;\n";
        xml << "              for (int xi : ge) {\n";
        xml << "                  temp.push_back(pnet::type::value::value_type(xi));\n";
        xml << "              }\n";
        xml << "              v.push_back(temp);\n";
        xml << "          }\n";
        xml << "          ]]>\n";
        xml << "          </code>\n";
        xml << "        </module>\n";
        xml << "      </defun>\n";
        xml << "      <connect-read port=\"i\" place=\"degree\"/>\n";
        xml << "      <connect-read port=\"n\" place=\"N\"/>\n";
        xml << "      <connect-in port=\"control_" << i << "\" place=\"control_" << i << "\"/>\n";
        xml << "      <connect-out-many port=\"v\" place=\"d_vectors_" << i << "\"/>\n";
        xml << "    </transition>\n\n";

        // Combination transition for this degree
        xml << "    <transition name=\"Combination_" << i << "\">\n";
        xml << "      <defun>\n";
        xml << "        <in name=\"v\" type=\"list\"/>\n";
        xml << "        <out name=\"vec\" type=\"list\"/>\n";
        xml << "        <module name=\"feynman_module\" function=\"partition_" << i << "(v, vec)\">\n";
        for (const auto& inc : net.includes.at("Combination")) {
            xml << "          <cinclude href=\"" << inc << "\"/>\n";
        }
        xml << "          <code><![CDATA[" << net.code_blocks.at("Combination");
        xml << "         ]]>\n";
        xml << "          </code>\n";
        xml << "        </module>\n";
        xml << "      </defun>\n";
        xml << "      <connect-in port=\"v\" place=\"d_vectors_" << i << "\"/>\n";
        xml << "      <connect-out-many port=\"vec\" place=\"branchtype_" << i << "\"/>\n";
        xml << "    </transition>\n\n";

        // Feynman degree transition for this degree
        xml << "    <transition name=\"feynman_degree_" << i << "\">\n";
        xml << "      <defun>\n";
        xml << "        <in name=\"deg\" type=\"int\"/>\n";
        xml << "        <in name=\"a\" type=\"list\"/>\n";
        xml << "        <in name=\"G\" type=\"list\"/>\n";
        xml << "        <out name=\"s\" type=\"string\"/>\n";
        xml << "        <module name=\"feynman_module\" function=\"feynman_integral_degree_" << i << "(deg,s,G,a)\">\n";
        for (const auto& inc : net.includes.at("feynman_degree")) {
            xml << "          <cinclude href=\"" << inc << "\"/>\n";
        }
        xml << "          <code><![CDATA[" << net.code_blocks.at("feynman_degree");
        xml << "         ]]>\n";
        xml << "          </code>\n";
        xml << "        </module>\n";
        xml << "      </defun>\n";
        xml << "      <connect-read port=\"deg\" place=\"deg\"/>\n";
        xml << "      <connect-in port=\"a\" place=\"branchtype_" << i << "\"/>\n";
        xml << "      <connect-read port=\"G\" place=\"graph\"/>\n";
        xml << "      <connect-out port=\"s\" place=\"fey_out_" << i << "\"/>\n";
        xml << "    </transition>\n\n";

        // Reduce transition for this degree
        xml << "    <transition name=\"Reduce_" << i << "\">\n";
        xml << "      <defun>\n";
        xml << "        <in name=\"s\" type=\"string\"/>\n";
        xml << "        <inout name=\"sum\" type=\"string\"/>\n";
        xml << "        <out name=\"ns\" type=\"control\"/>\n";
        xml << "        <module name=\"feynman_module\" function=\"plus_" << i << " (ns,s, sum)\">\n";
        for (const auto& inc : net.includes.at("Reduce")) {
            xml << "          <cinclude href=\"" << inc << "\"/>\n";
        }
        xml << "          <code><![CDATA[" << net.code_blocks.at("Reduce");
        xml << "         ]]>\n";
        xml << "          </code>\n";
        xml << "        </module>\n";
        xml << "      </defun>\n";
        xml << "      <connect-in port=\"s\" place=\"fey_out_" << i << "\"/>\n";
        xml << "      <connect-inout port=\"sum\" place=\"sum_" << i << "\"/>\n";
        xml << "      <connect-out port=\"ns\" place=\"ns\"/>\n";
        xml << "    </transition>\n\n";
    }

    // Add sum_number transition
    xml << "    <transition name=\"sum_number\">\n";
    xml << "      <defun>\n";
    xml << "        <in name=\"ns\" type=\"control\"/>\n";
    xml << "        <inout name=\"counter_total\" type=\"unsigned long\"/>\n";
    xml << "        <module name=\"feynman_module\" function=\"control1 (ns,counter_total)\">\n";
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
    xml << "        <in name=\"total\" type=\"unsigned long\"/>\n";
    xml << "        <in name=\"counter_total\" type=\"unsigned long\"/>\n";
    xml << "        <out name=\"fey_sum\" type=\"string\"/>\n";
    for (int i = 2; i <= d; i++) {
        xml << "        <in name=\"sum_" << i << "\" type=\"string\"/>\n";
    }
    xml << "        <module name=\"feynman_module\" function=\"final (total,counter_total,fey_sum";
    for (int i = 2; i <= d; i++) {
        xml << ",sum_" << i;
    }
    xml << ")\">\n";
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
    xml << "        <in name=\"fey_sum\" type=\"string\"/>\n";
    xml << "        <out name=\"answer\" type=\"string\"/>\n";
    xml << "        <module name=\"feynman_module\" function=\"Quasi (fey_sum,answer)\">\n";
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
    int d=5;
    PetriNet myPetriNet = generate_feynman_petri_net();
    print_petri_net(myPetriNet);

    std::string xml = generateFeynmanXPNetXML(myPetriNet,d);
    std::ofstream file("generated_feynman.xpnet");
    file << xml;
    file.close();

    return 0;
}