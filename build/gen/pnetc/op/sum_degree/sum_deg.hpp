#ifndef _PNETC_OP_sum_degree_sum_deg
#define _PNETC_OP_sum_degree_sum_deg

#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace sum_degree
    {
      void sum_deg
        ( const int& n
        , const int& d
        , std::list<pnet::type::value::value_type>& v
        );
    }
  }
}
#endif
