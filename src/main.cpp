#include <feynman/parse_parameters_from_commandline.hpp>
#include <feynman/execute.hpp>
#include <feynman/Parameters.hpp>
#include <feynman/Workflow.hpp>

#include <util-generic/print_exception.hpp>

#include <cstdlib>
#include <iostream>

int main (int argc, char** argv)
try
{
  // (1) loading configuration options
  auto const parameters = feynman::parse_parameters_from_commandline
    (feynman::execution::options(),
     feynman::Workflow::options(),
     argc,
     argv
    );

  // (2) initializing a workflow
  feynman::Workflow const workflow (parameters);

  // (3) executing the workflow
  auto const results = feynman::execute (parameters, workflow);

  // (4) evaluating the workflow result
  return workflow.process (results);
}
catch (...)
{
  std::cerr << "FAILURE: " << fhg::util::current_exception_printer() << std::endl;

  return EXIT_FAILURE;
}
