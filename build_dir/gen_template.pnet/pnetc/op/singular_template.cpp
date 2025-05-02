#include <we/loader/macros.hpp>

#include <pnetc/op/singular_template/degrees_fey.hpp>
#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void degrees_fey
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const int & weightmax (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "weightmax"))));
        const int & d (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "d"))));
        std::list<pnet::type::value::value_type> v;
        ::pnetc::op::singular_template::degrees_fey (weightmax, d, v);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "v"), v);
      }
    }
  }
}
#include <pnetc/op/singular_template/partition.hpp>
#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
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
        ::pnetc::op::singular_template::partition (v, vec);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "vec"), vec);
      }
    }
  }
}
#include <pnetc/op/singular_template/feynman_integral_degree.hpp>
#include <list>
#include <we/type/value.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void feynman_integral_degree
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::string & genus (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "genus"))));
        const std::string & loop (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "loop"))));
        const int & deg (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "deg"))));
        const std::string & graph (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "graph"))));
        const std::list<pnet::type::value::value_type> & a (::boost::get< std::list<pnet::type::value::value_type> > (_pnetc_input.value (std::list<std::string> (1, "a"))));
        std::string s;
        ::pnetc::op::singular_template::feynman_integral_degree (genus, loop, deg, graph, a, s);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), s);
      }
    }
  }
}
#include <pnetc/op/singular_template/plus.hpp>
#include <string>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
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
        ::pnetc::op::singular_template::plus (s, sum, ns);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "sum"), sum);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "ns"), ns);
      }
    }
  }
}
#include <pnetc/op/singular_template/control1.hpp>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
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
        ::pnetc::op::singular_template::control1 (ns, counter_total);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "counter_total"), counter_total);
      }
    }
  }
}
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
        const std::string & base_filename (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "base_filename"))));
        const std::string & input (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "input"))));
        const std::string & library_name (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "library_name"))));
        std::string output;
        ::pnetc::op::singular_template::compute (base_filename, input, library_name, output);
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
        std::string graph;
        int degree;
        int edges;
        int g;
        int deg;
        unsigned long total;
        std::string genus;
        std::string loop;
        ::pnetc::op::singular_template::compute1 (input, graph, degree, edges, g, deg, total, genus, loop);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "graph"), graph);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "degree"), degree);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "edges"), edges);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "g"), g);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "deg"), deg);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "total"), total);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "genus"), genus);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "loop"), loop);
      }
    }
  }
}
#include <pnetc/op/singular_template/final.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
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
        ::pnetc::op::singular_template::final (total, counter_total, answer);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "answer"), answer);
      }
    }
  }
}
#include <pnetc/op/singular_template/gen_block_.hpp>
#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
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
        ::pnetc::op::singular_template::gen_block_ (n, i, v);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "v"), v);
      }
    }
  }
}
#include <pnetc/op/singular_template/Quasi.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
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
        ::pnetc::op::singular_template::Quasi (weightmax, answer);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "answer"), answer);
      }
    }
  }
}
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::degrees_fey,"degrees_fey");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::partition,"partition");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::feynman_integral_degree,"feynman_integral_degree");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::plus,"plus");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::control1,"control1");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::compute,"compute");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::compute1,"compute1");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::final,"final");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::gen_block_,"gen_block_");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Quasi,"Quasi");
}
WE_MOD_INITIALIZE_END()
