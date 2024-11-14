#include <we/loader/macros.hpp>

#include <pnetc/op/singular_template/compute.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void compute
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::string & input (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "input"))));
        const std::string & library_name (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "library_name"))));
        std::string output;
        ::pnetc::op::singular_template::compute (input, library_name, output);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "output"), output);
      }
    }
  }
}
#include <pnetc/op/singular_template/compute1.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void compute1
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::string & input (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "input"))));
        const std::string & base_filename (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "base_filename"))));
        const std::string & library_name (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "library_name"))));
        std::string output;
        ::pnetc::op::singular_template::compute1 (input, base_filename, library_name, output);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "output"), output);
      }
    }
  }
}
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::compute,"compute");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::compute1,"compute1");
}
WE_MOD_INITIALIZE_END()
