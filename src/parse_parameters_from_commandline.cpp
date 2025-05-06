#include <feynman/parse_parameters_from_commandline.hpp>

#include <boost/program_options.hpp>

#include <cstdlib>
#include <iostream>

namespace feynman
{
  Parameters parse_parameters_from_commandline
    ( ParametersDescription const& driver_opts
    , ParametersDescription const& workflow_opts
    , ParametersDescription const& workflow_type_opts
    , int argc
    , char** argv
    )
  {
    namespace po = boost::program_options;

    po::options_description visible_options;
    visible_options.add(driver_opts);
    visible_options.add(workflow_opts);
    visible_options.add(workflow_type_opts);

    po::variables_map vm;
    po::store (po::parse_command_line (argc, argv, visible_options), vm);

    if (vm.count ("help"))
    {
      std::cout << visible_options << std::endl;
      std::exit (EXIT_SUCCESS);
    }

    po::notify (vm);

    return Parameters (vm);
  }

  Parameters parse_parameters_from_commandline
    ( ParametersDescription const& driver_opts
    , ParametersDescription const& workflow_opts
    , int argc
    , char** argv
    )
  {
    namespace po = boost::program_options;
    po::options_description empty_opts;
    return parse_parameters_from_commandline(driver_opts, workflow_opts, empty_opts, argc, argv);
  }
}
