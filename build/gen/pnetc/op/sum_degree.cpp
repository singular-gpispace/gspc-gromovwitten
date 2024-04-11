#include <we/loader/macros.hpp>

#include <pnetc/op/sum_degree/sum_deg.hpp>
#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace sum_degree
    {
      static void sum_deg
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const int & n (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "n"))));
        const int & d (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "d"))));
        std::list<pnet::type::value::value_type> v;
        ::pnetc::op::sum_degree::sum_deg (n, d, v);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "v"), v);
      }
    }
  }
}
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::sum_degree::sum_deg,"sum_deg");
}
WE_MOD_INITIALIZE_END()
