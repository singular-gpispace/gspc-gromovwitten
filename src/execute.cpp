#include <feynman/execute.hpp>
#include <feynman/Workflow.hpp>

#include <drts/client.hpp>
#include <drts/drts.hpp>
#include <drts/scoped_rifd.hpp>

#include <util-generic/executable_path.hpp>

#include <filesystem>
#include <boost/filesystem/path.hpp>

#include <string>
#include <iostream>

namespace feynman
{
  namespace execution
  {
    ParametersDescription options()
    {
      namespace po = boost::program_options;

      ParametersDescription driver_opts ("Worker Topology");
      driver_opts.add_options()("topology", po::value<std::string>()->required());
      driver_opts.add_options()("workflow_path", po::value<std::string>());
      driver_opts.add (gspc::options::installation());
      driver_opts.add (gspc::options::drts());
      driver_opts.add (gspc::options::logging());
      driver_opts.add (gspc::options::scoped_rifd());

      return driver_opts;
    }
  }

  WorkflowResult execute (Parameters parameters, Workflow const& workflow)
  {
    // Get the executable path and use its parent directory as installation path
    auto const executable_path = fhg::util::executable_path();
    auto const installation_path = std::filesystem::path(executable_path.parent_path().parent_path().string());
    auto const library_path = installation_path / "lib";
    
    // Get workflow path from parameters
    auto const workflow_path = std::filesystem::path(parameters.at("workflow_path").as<std::string>());

    std::cout << "Debug - executable_path: " << executable_path << std::endl;
    std::cout << "Debug - installation_path: " << installation_path << std::endl;
    std::cout << "Debug - library_path: " << library_path << std::endl;
    std::cout << "Debug - workflow_path: " << workflow_path << std::endl;
    std::cout << "Debug - workflow_path exists: " << std::filesystem::exists(workflow_path) << std::endl;

    gspc::installation installation (parameters);
    gspc::scoped_rifds rifds(gspc::rifd::strategy {parameters},
                             gspc::rifd::hostnames {parameters},
                             gspc::rifd::port {parameters},
                             installation);

    gspc::set_application_search_path(parameters, library_path);

    gspc::scoped_runtime_system drts (parameters,
                                      installation,
                                      parameters.at ("topology").as<std::string>(),
                                      rifds.entry_points());

    gspc::workflow const workflow_obj(workflow_path);

    return gspc::client {drts}.put_and_run (workflow_obj, workflow.inputs().map());
  }
}