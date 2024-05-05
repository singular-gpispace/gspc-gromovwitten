#include <we/loader/macros.hpp>

#include <pnetc/op/feynman_module/sum_deg.hpp>
#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
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
        ::pnetc::op::feynman_module::sum_deg (n, d, v);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "v"), v);
      }
    }
  }
}
#include <pnetc/op/feynman_module/partition.hpp>
#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
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
        ::pnetc::op::feynman_module::partition (v, vec);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "vec"), vec);
      }
    }
  }
}
#include <pnetc/op/feynman_module/count.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      static void count
        ( drts::worker::context *
        , expr::eval::context const&
        , expr::eval::context&
        , std::map<std::string, void*> const&
        )
      {
        ::pnetc::op::feynman_module::count ();
      }
    }
  }
}
#include <pnetc/op/feynman_module/signature_and_multiplicities.hpp>
#include <list>
#include <we/type/value.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      static void signature_and_multiplicities
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::string & G (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "G"))));
        const std::list<pnet::type::value::value_type> & a (::boost::get< std::list<pnet::type::value::value_type> > (_pnetc_input.value (std::list<std::string> (1, "a"))));
        std::list<pnet::type::value::value_type> s;
        ::pnetc::op::feynman_module::signature_and_multiplicities (G, a, s);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), s);
      }
    }
  }
}
#include <pnetc/op/feynman_module/feynman_integral_degree.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
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
        ::pnetc::op::feynman_module::feynman_integral_degree (G, a));
      }
    }
  }
}
#include <pnetc/op/feynman_module/plus.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      static void plus
        ( drts::worker::context *
        , expr::eval::context const&
        , expr::eval::context&
        , std::map<std::string, void*> const&
        )
      {
        ::pnetc::op::feynman_module::plus ();
      }
    }
  }
}
#include <pnetc/op/feynman_module/add.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      static void add
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const unsigned long & l (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "l"))));
        const unsigned long & r (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "r"))));
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), 
        ::pnetc::op::feynman_module::add (l, r));
      }
    }
  }
}
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::feynman_module::sum_deg,"sum_deg");
  WE_REGISTER_FUN_AS (::pnetc::op::feynman_module::partition,"partition");
  WE_REGISTER_FUN_AS (::pnetc::op::feynman_module::count,"count");
  WE_REGISTER_FUN_AS (::pnetc::op::feynman_module::signature_and_multiplicities,"signature_and_multiplicities");
  WE_REGISTER_FUN_AS (::pnetc::op::feynman_module::feynman_integral_degree,"feynman_integral_degree");
  WE_REGISTER_FUN_AS (::pnetc::op::feynman_module::plus,"plus");
  WE_REGISTER_FUN_AS (::pnetc::op::feynman_module::add,"add");
}
WE_MOD_INITIALIZE_END()
