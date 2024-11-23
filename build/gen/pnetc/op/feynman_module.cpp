#include <we/loader/macros.hpp>

#include <pnetc/op/feynman_module/init.hpp>
#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      static void init
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const int & weightmax (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "weightmax"))));
        const int & d (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "d"))));
        std::list<pnet::type::value::value_type> v;
        ::pnetc::op::feynman_module::init (weightmax, d, v);
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
#include <pnetc/op/feynman_module/feynman_integral_degree.hpp>
#include <list>
#include <we/type/value.hpp>
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
        const std::list<pnet::type::value::value_type> & G (::boost::get< std::list<pnet::type::value::value_type> > (_pnetc_input.value (std::list<std::string> (1, "G"))));
        const std::list<pnet::type::value::value_type> & a (::boost::get< std::list<pnet::type::value::value_type> > (_pnetc_input.value (std::list<std::string> (1, "a"))));
        const std::string & gg (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "gg"))));
        const std::string & l (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "l"))));
        const int & deg (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "deg"))));
        std::string s;
        ::pnetc::op::feynman_module::feynman_integral_degree (G, a, gg, l, deg, s);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), s);
      }
    }
  }
}
#include <pnetc/op/feynman_module/plus.hpp>
#include <string>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      static void plus
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::string & s (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "s"))));
        std::string sum (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "sum"))));
        we::type::literal::control ns;
        ::pnetc::op::feynman_module::plus (s, sum, ns);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "sum"), sum);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "ns"), ns);
      }
    }
  }
}
#include <pnetc/op/feynman_module/control1.hpp>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      static void control1
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const we::type::literal::control & ns (::boost::get< we::type::literal::control > (_pnetc_input.value (std::list<std::string> (1, "ns"))));
        unsigned long counter_total (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "counter_total"))));
        ::pnetc::op::feynman_module::control1 (ns, counter_total);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "counter_total"), counter_total);
      }
    }
  }
}
#include <pnetc/op/feynman_module/final.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      static void final
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const unsigned long & total (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "total"))));
        const unsigned long & counter_total (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "counter_total"))));
        std::string answer (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "answer"))));
        ::pnetc::op::feynman_module::final (total, counter_total, answer);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "answer"), answer);
      }
    }
  }
}
#include <pnetc/op/feynman_module/gen_block_.hpp>
#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      static void gen_block_
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const int & n (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "n"))));
        const int & i (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "i"))));
        std::list<pnet::type::value::value_type> v;
        ::pnetc::op::feynman_module::gen_block_ (n, i, v);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "v"), v);
      }
    }
  }
}
#include <pnetc/op/feynman_module/Quasi.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      static void Quasi
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const int & weightmax (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "weightmax"))));
        std::string answer (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "answer"))));
        ::pnetc::op::feynman_module::Quasi (weightmax, answer);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "answer"), answer);
      }
    }
  }
}
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::feynman_module::init,"init");
  WE_REGISTER_FUN_AS (::pnetc::op::feynman_module::partition,"partition");
  WE_REGISTER_FUN_AS (::pnetc::op::feynman_module::feynman_integral_degree,"feynman_integral_degree");
  WE_REGISTER_FUN_AS (::pnetc::op::feynman_module::plus,"plus");
  WE_REGISTER_FUN_AS (::pnetc::op::feynman_module::control1,"control1");
  WE_REGISTER_FUN_AS (::pnetc::op::feynman_module::final,"final");
  WE_REGISTER_FUN_AS (::pnetc::op::feynman_module::gen_block_,"gen_block_");
  WE_REGISTER_FUN_AS (::pnetc::op::feynman_module::Quasi,"Quasi");
}
WE_MOD_INITIALIZE_END()
