#ifndef _PNETC_OP_feynman_module_init
#define _PNETC_OP_feynman_module_init

#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      void init
        ( const int& weightmax
        , const int& d
        , std::list<pnet::type::value::value_type>& v
        );
    }
  }
}
#endif
