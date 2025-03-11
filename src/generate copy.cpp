#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <utility> // for std::pair
#include <string>
#include <ostream>
#include <iostream>
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
    std::vector<std::string> vertices;
    std::vector<std::pair<std::string, std::string>> edges;

    // Add "T_" prefix to vertices
    for (const auto& v : raw_vertices) {
        vertices.push_back("T_" + v);
    }

    // Add "T_" prefix to edges
    for (const auto& edge : raw_edges) {
        edges.emplace_back("T_" + edge.first, "T_" + edge.second);
    }

    return {vertices, edges};
}

// Function to generate PetriNet from DAG
PetriNet generate_petri_net(const DAG& dag) {
    PetriNet net;
    net.transitions = dag.vertices;

    std::map<std::string, std::vector<std::string>> transition_inputs;
    std::map<std::string, std::string> transition_output;

    std::set<std::string> root_nodes(dag.vertices.begin(), dag.vertices.end());
    for (const auto& edge : dag.edges) {
        root_nodes.erase(edge.second);
    }

    // Add root place P0
    net.places.push_back("P0");
    net.place_types["P0"] = "string";

    // Add input place
    net.places.push_back("input");
    net.place_types["input"] = "string";

    // Create control places and define transitions
    for (const auto& transition : dag.vertices) {
        std::string control_place = "control_" + transition.substr(2); // T11 -> control_11
        net.places.push_back(control_place);
        net.place_types[control_place] = "control";

        if (transition_inputs.find(transition) == transition_inputs.end()) {
            transition_inputs[transition] = {};
        }
        transition_inputs[transition].push_back(control_place);
    }

    // Connect root nodes to P0 and input
    for (const auto& root : root_nodes) {
        transition_inputs[root].push_back("P0");
        transition_inputs[root].push_back("input");
    }

    // Assign outputs: Txx -> P_xx
    for (const auto& transition : dag.vertices) {
        std::string place_name = "P_" + transition.substr(2); // T11 -> P_11
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

    // Ensure all transitions have entries in transition_inputs and outputs
    for (const auto& transition : dag.vertices) {
        if (transition_inputs.find(transition) == transition_inputs.end()) {
            transition_inputs[transition] = {};
        }
        if (transition_output.find(transition) != transition_output.end()) {
            net.transition_outputs[transition] = { transition_output[transition] };
        }
    }

    net.transition_inputs = transition_inputs;
    return net;
}


std::string generateXPNetXML(const PetriNet& net) {
    std::ostringstream xml;

    xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml << "<defun name=\"feynman\">\n";
    xml << "    <in name=\"input\" type=\"string\" place=\"input\"/>\n";
    xml << "    <in name=\"P0\" type=\"string\" place=\"P0\"/>\n";
    // Find the last output place
    std::string last_output_place = "P0";
    for (const auto& [transition, outputs] : net.transition_outputs) {
        if (!outputs.empty()) {
            last_output_place = outputs[0];
        }
    }

    xml << "    <out name=\"result\" type=\"string\" place=\"" << last_output_place << "\"/>\n";
    xml << "    <net>\n";

    // Count how many times each place is used as an input
    std::map<std::string, int> place_usage_count;
    for (const auto& [transition, inputs] : net.transition_inputs) {
        for (const auto& in_place : inputs) {
            place_usage_count[in_place]++;
        }
    }

    // Declare places
    std::set<std::string> declared_places;
    for (const auto& place : net.places) {
        if (declared_places.find(place) == declared_places.end()) {
            std::string type = net.place_types.at(place);
            if (type == "string") {
                xml << "        <place name=\"" << place << "\" type=\"" << type << "\"/>\n";
            }
            else if (type == "control") {
                xml << "        <place name=\"" << place << "\" type=\"" << type << "\">\n";
                xml << "            <token>\n                <value>[]</value>\n            </token>\n";
                xml << "        </place>\n";
            }
            declared_places.insert(place);
        }
    }

    // Declare transitions
    for (const auto& transition : net.transitions) {
        xml << "        <transition name=\"" << transition << "\">\n";
        xml << "            <defun>\n";

        std::vector<std::string> function_args;
        std::set<std::string> declared_inputs;

        if (net.transition_inputs.find(transition) != net.transition_inputs.end()) {
            for (const auto& in_place : net.transition_inputs.at(transition)) {
                std::string type = (in_place.find("control_") == 0) ? "control" : "string";
                if (declared_inputs.find(in_place) == declared_inputs.end()) {
                    xml << "                <in name=\"" << in_place << "\" type=\"" << type << "\"/>\n";
                    function_args.push_back(in_place);
                    declared_inputs.insert(in_place);
                }
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

        if (transition == "T_11") {
            xml << "              <cinclude href=\"feynman.hpp\"/> \n";
            xml << "                    <code><![CDATA[\n";
            xml << "                        std::cout << \"DEBUG: Transition " << transition << " starting.\" << std::endl;\n";

            xml << "                        int res = h(4, 5);\n";
            xml << "                        std::cout << \"T11: Result = \" << res << std::endl;\n";

            // **Dynamically assign the output place**
            if (!function_args.empty()) {
                std::string out_place = function_args.back(); // Last argument is the output place
                xml << "                        " << out_place << " = std::to_string(res);\n";
                xml << "                        std::cout << \"DEBUG: " << transition << " output assigned to " << out_place << " with value \" << res << std::endl;\n";
            }
            else {
                xml << "                        std::cerr << \"ERROR: No output place defined for transition " << transition << "!\" << std::endl;\n";
            }

            xml << "                        std::cout << \"DEBUG: Transition " << transition << " finished.\" << std::endl;\n";
            xml << "                    ]]></code>\n";
        }
        else {
            xml << "              <cinclude href=\"feynman.hpp\"/> \n";
            xml << "                    <code><![CDATA[\n";
            xml << "                        std::cout << \"DEBUG: Transition " << transition << " starting.\" << std::endl;\n";

            // Debugging input values
            xml << "                        std::cout << \"DEBUG: function_args[1] = '\" << " << function_args[1] << " << \"'\" << std::endl;\n";

            xml << "                       int input=std::stoi(" << function_args[1] << ");\n";
            xml << "                        std::string output_value;\n";  // Dynamically assign output place
            xml << "                        int result = g(input, output_value);\n";
            xml << "                        std::cout << \"DEBUG: Transition " << transition << " result = \" << result << std::endl;\n";

            // **Dynamically assign the output place**
            if (!function_args.empty()) {
                std::string out_place = function_args.back(); // Last argument is the output place
                xml << "                        " << out_place << " = std::to_string(result);\n";
                xml << "                        std::cout << \"DEBUG: " << transition << " output assigned to " << out_place << " with value \" << result << std::endl;\n";
            }
            else {
                xml << "                        std::cerr << \"ERROR: No output place defined for transition " << transition << "!\" << std::endl;\n";
            }

            xml << "                    ]]></code>\n";
        }


        xml << "                </module>\n";
        xml << "            </defun>\n";

        // Connect inputs and outputs
        std::set<std::string> connected_inputs;
        std::set<std::string> connected_outputs;

        if (net.transition_inputs.find(transition) != net.transition_inputs.end()) {
            for (const auto& in_place : net.transition_inputs.at(transition)) {
                if (connected_inputs.find(in_place) == connected_inputs.end()) {
                    if (place_usage_count[in_place] > 1) {
                        xml << "            <connect-read port=\"" << in_place << "\" place=\"" << in_place << "\"/>\n";
                    }
                    else {
                        xml << "            <connect-in port=\"" << in_place << "\" place=\"" << in_place << "\"/>\n";
                    }
                    connected_inputs.insert(in_place);
                }
            }
        }

        if (net.transition_outputs.find(transition) != net.transition_outputs.end() && !net.transition_outputs.at(transition).empty()) {
            std::string out_place = net.transition_outputs.at(transition)[0];
            if (connected_outputs.find(out_place) == connected_outputs.end()) {
                xml << "            <connect-out port=\"" << out_place << "\" place=\"" << out_place << "\"/>\n";
                connected_outputs.insert(out_place);
            }
        }

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
        {"11", "21"},
        {"11", "22"},
        {"11", "23"},
        {"21", "31"},
        {"22", "31"},
        {"22", "32"},
        {"23", "32"},
        {"31", "41"},
        {"32", "41"}
    };

    DAG myDAG = generate_dag(vertices, edges);
    PetriNet myPetriNet = generate_petri_net(myDAG);

    std::string xml = generateXPNetXML(myPetriNet);

    std::ofstream file("../workflow/feynman.xpnet");
    file << xml;
    file.close();

    std::cout << "XPNet file 'feynman.xpnet' generated successfully!\n";
    return 0;
}