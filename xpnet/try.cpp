#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <utility>
#include <string>

// Define DAG structure
struct DAG {
    std::vector<std::string> vertices;
    std::vector<std::pair<std::string, std::string>> edges;
};

// Define PetriNet structure
struct PetriNet {
    std::vector<std::string> places;
    std::map<std::string, std::string> place_types;
    std::vector<std::string> transitions;
    std::map<std::string, std::vector<std::string>> transition_inputs;
    std::map<std::string, std::vector<std::string>> transition_outputs;
};

// Function to generate DAG with "T_" prefix
DAG generate_dag(const std::vector<std::string>& raw_vertices, const std::vector<std::pair<std::string, std::string>>& raw_edges) {
    DAG dag;
    for (const auto& v : raw_vertices) {
        dag.vertices.push_back("T_" + v);
    }
    for (const auto& edge : raw_edges) {
        dag.edges.emplace_back("T_" + edge.first, "T_" + edge.second);
    }
    return dag;
}

// Function to find the root node(s)
std::string find_root_node(const DAG& dag) {
    std::set<std::string> root_nodes(dag.vertices.begin(), dag.vertices.end());
    for (const auto& edge : dag.edges) {
        root_nodes.erase(edge.second);
    }
    if (!root_nodes.empty()) {
        return *root_nodes.begin();
    } else {
        return "";
    }
}

// Function to generate and print PetriNet from DAG
PetriNet generate_petri_net(const DAG& dag) {
    PetriNet net;
    net.transitions = dag.vertices;

    std::map<std::string, std::vector<std::string>> transition_inputs;
    std::map<std::string, std::string> transition_output;

    std::string root_node = find_root_node(dag);

    // Add root place P0
    net.places.push_back("P0");
    net.place_types["P0"] = "string";

    // Add input place
    net.places.push_back("input");
    net.place_types["input"] = "string";

    // Create control places and define transitions
    for (const auto& transition : dag.vertices) {
        std::string control_place = "control_" + transition.substr(2);
        net.places.push_back(control_place);
        net.place_types[control_place] = "control";
        transition_inputs[transition].push_back(control_place);
    }

    // Connect root node to P0 and input
    if (!root_node.empty()) {
        transition_inputs[root_node].push_back("P0");
        transition_inputs[root_node].push_back("input");
    }

    // Assign outputs: Txx -> P_xx
    for (const auto& transition : dag.vertices) {
        std::string place_name = "P_" + transition.substr(2);
        net.places.push_back(place_name);
        net.place_types[place_name] = "string";
        transition_output[transition] = place_name;
    }

    // Connect transitions based on edges
    for (const auto& edge : dag.edges) {
        if (transition_output.find(edge.first) != transition_output.end()) {
            transition_inputs[edge.second].push_back(transition_output[edge.first]);
        }
    }

    for (const auto& transition : dag.vertices) {
        if (transition_output.find(transition) != transition_output.end()) {
            net.transition_outputs[transition] = { transition_output[transition] };
        }
    }

    net.transition_inputs = transition_inputs;

    // Print PetriNet for debugging
    std::cout << "\nGenerated PetriNet:\n";
    std::cout << "Places:\n";
    for (const auto& place : net.places) {
        std::cout << "  " << place << " (" << net.place_types[place] << ")\n";
    }

    std::cout << "Transitions:\n";
    for (const auto& transition : net.transitions) {
        std::cout << "  " << transition << "\n";
    }

    std::cout << "Transition Inputs:\n";
    for (const auto& [t, inputs] : net.transition_inputs) {
        std::cout << "  " << t << " <- { ";
        for (const auto& i : inputs) std::cout << i << " ";
        std::cout << "}\n";
    }

    std::cout << "Transition Outputs:\n";
    for (const auto& [t, outputs] : net.transition_outputs) {
        std::cout << "  " << t << " -> { ";
        for (const auto& o : outputs) std::cout << o << " ";
        std::cout << "}\n";
    }

    return net;
}
std::string generateXPNetXML(const PetriNet& net, const std::string& root_node) {
    std::ostringstream xml;

    xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml << "<defun name=\"feynman\">\n";
    xml << "    <in name=\"input\" type=\"string\" place=\"input\"/>\n";
    xml << "    <in name=\"P0\" type=\"string\" place=\"P0\"/>\n";

    std::string last_output_place = "P0";
    for (const auto& [transition, outputs] : net.transition_outputs) {
        if (!outputs.empty()) {
            last_output_place = outputs[0];
        }
    }

    xml << "    <out name=\"result\" type=\"string\" place=\"" << last_output_place << "\"/>\n";
    xml << "    <net>\n";

    std::map<std::string, int> place_usage_count;
    for (const auto& [transition, inputs] : net.transition_inputs) {
        for (const auto& in_place : inputs) {
            place_usage_count[in_place]++;
        }
    }

    std::set<std::string> declared_places;
    for (const auto& place : net.places) {
        if (declared_places.find(place) == declared_places.end()) {
            std::string type = net.place_types.at(place);
            xml << "        <place name=\"" << place << "\" type=\"" << type << "\"/>\n";
            declared_places.insert(place);
        }
    }

    for (const auto& transition : net.transitions) {
        xml << "        <transition name=\"" << transition << "\">\n";
        xml << "            <defun>\n";

        std::vector<std::string> function_args;
        std::set<std::string> declared_inputs;

        if (net.transition_inputs.find(transition) != net.transition_inputs.end()) {
            for (const auto& in_place : net.transition_inputs.at(transition)) {
                std::string type = (in_place.find("control_") == 0) ? "control" : "string";
                xml << "                <in name=\"" << in_place << "\" type=\"" << type << "\"/>\n";
                function_args.push_back(in_place);
                declared_inputs.insert(in_place);
            }
        }

        if (net.transition_outputs.find(transition) != net.transition_outputs.end() && !net.transition_outputs.at(transition).empty()) {
            std::string out_place = net.transition_outputs.at(transition)[0];
            xml << "                <out name=\"" << out_place << "\" type=\"string\"/>\n";
            function_args.push_back(out_place);
        }

        xml << "                <module name=\"feynman\" function=\"f_" << transition << " (";
        for (size_t i = 0; i < function_args.size(); ++i) {
            xml << function_args[i];
            if (i < function_args.size() - 1) {
                xml << ", ";
            }
        }
        xml << ")\">\n";

        xml << "              <cinclude href=\"feynman.hpp\"/> \n";
        xml << "                    <code><![CDATA[\n";
        xml << "                        std::cout << \"DEBUG: Transition " << transition << " starting.\" << std::endl;\n";

        if (transition == root_node) {
            xml << "                        int res = h(4, 5);\n";
        } else {
            xml << "                        int input=std::stoi(" << function_args[1] << ");\n";
            xml << "                        std::string output_value;\n";
            xml << "                        int result = g(input, output_value);\n";
        }

        if (!function_args.empty()) {
            std::string out_place = function_args.back();
            xml << "                        " << out_place << " = std::to_string(res);\n";
        }

        xml << "                    ]]></code>\n";
        xml << "                </module>\n";
        xml << "            </defun>\n";
        xml << "        </transition>\n";
    }

    xml << "    </net>\n";
    xml << "</defun>\n";

    return xml.str();
}

// Main function
int main() {
    std::vector<std::string> vertices = {"11", "21", "22", "23", "31", "32", "41"};
    std::vector<std::pair<std::string, std::string>> edges = {
        {"11", "21"}, {"11", "22"}, {"11", "23"},
        {"21", "31"}, {"22", "31"}, {"22", "32"},
        {"23", "32"}, {"31", "41"}, {"32", "41"}
    };

    DAG myDAG = generate_dag(vertices, edges);
    std::string root_node = find_root_node(myDAG);
    
    std::cout << "Root Node: " << root_node << "\n";

    PetriNet myPetriNet = generate_petri_net(myDAG);

    std::string xml = generateXPNetXML(myPetriNet, root_node);

    std::ofstream file("../workflow/feynman.xpnet");
    file << xml;
    file.close();

    return 0;
}
