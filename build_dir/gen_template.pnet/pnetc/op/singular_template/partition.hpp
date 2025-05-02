#ifndef _PNETC_OP_singular_template_partition
#define _PNETC_OP_singular_template_partition

#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void partition
        ( const std::list<pnet::type::value::value_type>& v
        , std::list<pnet::type::value::value_type>& vec
        );
    }
  }
}
#endif
