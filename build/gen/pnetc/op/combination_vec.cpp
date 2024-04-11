#include <we/loader/macros.hpp>

#include <pnetc/op/combination_vec/partition.hpp>
#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace combination_vec
    {
      static void partition
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::list<pnet::type::value::value_type> & v (::boost::get< std::list<pnet::type::value::value_type> > (_pnetc_input.value (std::list<std::string> (1, "v"))));
        std::list<pnet::type::value::value_type> vec;
        ::pnetc::op::combination_vec::partition (v, vec);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "vec"), vec);
      }
    }
  }
}
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::combination_vec::partition,"partition");
}
WE_MOD_INITIALIZE_END()
