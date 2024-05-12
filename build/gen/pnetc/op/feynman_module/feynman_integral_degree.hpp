#ifndef _PNETC_OP_feynman_module_feynman_integral_degree
#define _PNETC_OP_feynman_module_feynman_integral_degree

#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      unsigned long feynman_integral_degree
        ( const std::list<pnet::type::value::value_type>& G
        , const std::list<pnet::type::value::value_type>& a
        );
    }
  }
}
#endif
