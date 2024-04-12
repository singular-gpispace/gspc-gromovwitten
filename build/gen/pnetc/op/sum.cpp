#include <we/loader/macros.hpp>

#include <pnetc/op/sum/add.hpp>

namespace pnetc
{
  namespace op
  {
    namespace sum
    {
      static void add
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const int & l (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "l"))));
        const int & r (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "r"))));
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), 
        ::pnetc::op::sum::add (l, r));
      }
    }
  }
}
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::sum::add,"add");
}
WE_MOD_INITIALIZE_END()
