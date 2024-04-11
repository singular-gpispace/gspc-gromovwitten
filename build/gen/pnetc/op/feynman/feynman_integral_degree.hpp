#ifndef _PNETC_OP_feynman_feynman_integral_degree
#define _PNETC_OP_feynman_feynman_integral_degree

#include <list>
#include <we/type/value.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace feynman
    {
      int feynman_integral_degree
        ( const std::string& G
        , const std::list<pnet::type::value::value_type>& a
        );
    }
  }
}
#endif
