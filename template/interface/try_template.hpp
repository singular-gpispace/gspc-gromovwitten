#pragma once

#define NO_NAME_MANGLING extern "C"
#include <vector>
#include <string>
#include <config.hpp>

#define RESOLVE_INTERFACE_FUNCTION(function) \
      (fhg::util::scoped_dlhandle \
      (config::implementation(), \
      RTLD_GLOBAL | RTLD_NOW | RTLD_DEEPBIND) \
      .sym<decltype(function)> \
      (BOOST_PP_STRINGIZE(function)))





NO_NAME_MANGLING
std::string singular_template_compute_StdBasis ( std::string const&
                                      , std::string const&
									  , std::string const&
                                      );




NO_NAME_MANGLING
std::pair<std::vector<std::string>, unsigned long> singular_template_leadSyzGPI( std::string const&
                                                    
                                                         , std::string const&
									  , std::string const&
                                      );

NO_NAME_MANGLING
std::pair<std::vector<std::string>, unsigned long> singular_template_LIFT( std::string const&
                                     , std::string const&
                                      , std::string const&
						  , std::string const&
                                      );

NO_NAME_MANGLING
std::pair<std::vector<std::string>, unsigned long> singular_template_SUBLIFT( std::string const&
                                     , std::string const&
                                      , std::string const&
						  , std::string const&
                                      );

NO_NAME_MANGLING
std::string singular_template_reduce( std::string const&
                                     , std::string const&
                                      , std::string const&
						  , std::string const&
                                      );