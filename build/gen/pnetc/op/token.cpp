#include <we/loader/macros.hpp>

#include <pnetc/op/token/count.hpp>

namespace pnetc
{
  namespace op
  {
    namespace token
    {
      static void count
        ( drts::worker::context *
        , expr::eval::context const&
        , expr::eval::context&
        , std::map<std::string, void*> const&
        )
      {
        ::pnetc::op::token::count ();
      }
    }
  }
}
#include <pnetc/op/token/plus.hpp>

namespace pnetc
{
  namespace op
  {
    namespace token
    {
      static void plus
        ( drts::worker::context *
        , expr::eval::context const&
        , expr::eval::context&
        , std::map<std::string, void*> const&
        )
      {
        ::pnetc::op::token::plus ();
      }
    }
  }
}
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::token::count,"count");
  WE_REGISTER_FUN_AS (::pnetc::op::token::plus,"plus");
}
WE_MOD_INITIALIZE_END()
