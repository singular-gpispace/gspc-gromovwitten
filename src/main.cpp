#include <feynman/parse_parameters_from_commandline.hpp>
#include <feynman/execute.hpp>
#include <feynman/Parameters.hpp>
#include <feynman/Workflow.hpp>
#include <../include/feynman/feynman.hpp> 

#include <util-generic/print_exception.hpp>

#include <cstdlib>
#include <iostream>
#include <string>
#include <filesystem>
#include <boost/filesystem/path.hpp>
#include <boost/program_options/variables_map.hpp>

enum class WorkflowType {
    BASE,
    COMPOSITION,
    DOUBLES_EDGES,
    QUASI_PSI,
    QUASI,
    SUM_PSI,
    SUM
};

WorkflowType parseWorkflowType(const std::string& type) {
    if (type == "composition") return WorkflowType::COMPOSITION;
    if (type == "doubles") return WorkflowType::DOUBLES_EDGES;
    if (type == "quasi_psi") return WorkflowType::QUASI_PSI;
    if (type == "quasi") return WorkflowType::QUASI;
    if (type == "sum_psi") return WorkflowType::SUM_PSI;
    if (type == "sum") return WorkflowType::SUM;
    return WorkflowType::BASE;
}

std::string getWorkflowPath(WorkflowType type, const std::filesystem::path& base_path) {
    std::string filename;
    switch (type) {
        case WorkflowType::COMPOSITION:
            filename = "feynman_composition.pnet";
            break;
        case WorkflowType::DOUBLES_EDGES:
            filename = "feynman_doubles_edges.pnet";
            break;
        case WorkflowType::QUASI_PSI:
            filename = "feynman_quasi_psi.pnet";
            break;
        case WorkflowType::QUASI:
            filename = "feynman_quasi.pnet";
            break;
        case WorkflowType::SUM_PSI:
            filename = "feynman_sum_psi.pnet";
            break;
        case WorkflowType::SUM:
            filename = "feynman_sum.pnet";
            break;
        case WorkflowType::BASE:
            filename = "feynman.pnet";
            break;
    }
    return (base_path / "pnet" / filename).string();
}

int main(int argc, char** argv) try {
    // Add workflow type option
    namespace po = boost::program_options;
    po::options_description workflow_type_opts("Workflow Type");
    workflow_type_opts.add_options()
        ("type", po::value<std::string>()->default_value("base"), 
         "Workflow type (base|composition|doubles|quasi_psi|quasi|sum_psi|sum)")
        ("workflow_path", po::value<std::string>(), "Path to workflow file");

    // Load configuration options
    auto parameters = feynman::parse_parameters_from_commandline(
        feynman::execution::options(),
     feynman::Workflow::options(),
        workflow_type_opts,
     argc,
     argv
    );

    // Get workflow type
    std::string type_str = parameters.at("type").as<std::string>();
    WorkflowType workflow_type = parseWorkflowType(type_str);

    // Initialize workflow
    feynman::Workflow workflow(parameters);

    // Get installation path and workflow path
    std::filesystem::path installation_path = std::filesystem::path(argv[0]).parent_path().parent_path();
    std::string workflow_path = getWorkflowPath(workflow_type, installation_path);

    // Create a new variable value for the workflow path
    boost::program_options::variable_value v(boost::any(workflow_path), false);
    parameters.insert(std::make_pair(std::string("workflow_path"), v));

    // Execute workflow and process results
    auto results = feynman::execute(parameters, workflow);
    return workflow.process(results);
}
catch (...) {
  std::cerr << "FAILURE: " << fhg::util::current_exception_printer() << std::endl;
  return EXIT_FAILURE;
}
