#ifndef _PNETC_OP_feynman_module_partition
#define _PNETC_OP_feynman_module_partition

#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      void partition
        ( const std::list<pnet::type::value::value_type>& v
        , std::list<pnet::type::value::value_type>& vec
        );
    }
  }
}
#endif
