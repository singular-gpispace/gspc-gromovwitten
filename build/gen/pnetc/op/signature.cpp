#include <we/loader/macros.hpp>

#include <pnetc/op/signature/signature_and_multiplicitie.hpp>
#include <list>
#include <we/type/value.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace signature
    {
      static void signature_and_multiplicitie
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::string & G (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "G"))));
        const std::list<pnet::type::value::value_type> & a (::boost::get< std::list<pnet::type::value::value_type> > (_pnetc_input.value (std::list<std::string> (1, "a"))));
        std::list<pnet::type::value::value_type> s;
        ::pnetc::op::signature::signature_and_multiplicitie (G, a, s);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), s);
      }
    }
  }
}
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::signature::signature_and_multiplicitie,"signature_and_multiplicitie");
}
WE_MOD_INITIALIZE_END()
