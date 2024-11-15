#pragma once

#define NO_NAME_MANGLING extern "C"

#include <string>
#include <config.hpp>

#define RESOLVE_INTERFACE_FUNCTION(function) \
      (fhg::util::scoped_dlhandle \
      (config::implementation(), \
      RTLD_GLOBAL | RTLD_NOW | RTLD_DEEPBIND) \
      .sym<decltype(function)> \
      (BOOST_PP_STRINGIZE(function)))



NO_NAME_MANGLING
std::string singular_template_compute_StdBasis(std::string const&
      , std::string const&
);
std::string singular_template_compute_parseInput(const std::string&, std::string const&);
