#include <feynman/execute.hpp>
#include <feynman/Workflow.hpp>

#include <drts/client.hpp>
#include <drts/drts.hpp>
#include <drts/scoped_rifd.hpp>

#include <util-generic/executable_path.hpp>

#include <boost/filesystem/path.hpp>

#include <string>

namespace feynman
{
  namespace execution
  {
    ParametersDescription options()
    {
      ParametersDescription driver_opts {"Worker Topology"};
      driver_opts.add_options()
        ( "topology"
        , ::boost::program_options::value<std::string>()->required()
        );
      driver_opts.add (gspc::options::installation());
      driver_opts.add (gspc::options::drts());
      driver_opts.add (gspc::options::logging());
      driver_opts.add (gspc::options::scoped_rifd());

      return driver_opts;
    }
  }

  WorkflowResult execute (Parameters parameters, Workflow const& workflow)
  {
    auto const feynman_installation_path
      {fhg::util::executable_path().parent_path().parent_path()};

    gspc::installation installation {parameters};
    gspc::scoped_rifds rifds
      { gspc::rifd::strategy {parameters}
      , gspc::rifd::hostnames {parameters}
      , gspc::rifd::port {parameters}
      , installation
      };


      gspc::set_application_search_path(parameters, std::filesystem::path((feynman_installation_path / "lib").string()));

    gspc::scoped_runtime_system drts
      { parameters
      , installation
      , parameters.at ("topology").as<std::string>()
      , rifds.entry_points()
      };

      return gspc::client {drts}
      . put_and_run
        ( std::filesystem::path((feynman_installation_path / "pnet" / "feynman.pnet").string())
        , workflow.inputs().map()
        );

  }
}