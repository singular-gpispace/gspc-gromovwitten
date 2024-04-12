#include <we/loader/macros.hpp>

#include <pnetc/op/feynman/sum_deg.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace feynman
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
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "v"), 
        ::pnetc::op::feynman::sum_deg (n, d));
      }
    }
  }
}
#include <pnetc/op/feynman/partition.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace feynman
    {
      static void partition
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::string & v (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "v"))));
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "vec"), 
        ::pnetc::op::feynman::partition (v));
      }
    }
  }
}
#include <pnetc/op/feynman/feynman_integral_degree.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace feynman
    {
      static void feynman_integral_degree
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::string & G (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "G"))));
        const std::string & a (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "a"))));
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), 
        ::pnetc::op::feynman::feynman_integral_degree (G, a));
      }
    }
  }
}
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::feynman::sum_deg,"sum_deg");
  WE_REGISTER_FUN_AS (::pnetc::op::feynman::partition,"partition");
  WE_REGISTER_FUN_AS (::pnetc::op::feynman::feynman_integral_degree,"feynman_integral_degree");
}
WE_MOD_INITIALIZE_END()
