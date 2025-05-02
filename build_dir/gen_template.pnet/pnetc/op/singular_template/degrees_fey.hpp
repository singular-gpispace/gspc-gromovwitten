#ifndef _PNETC_OP_singular_template_degrees_fey
#define _PNETC_OP_singular_template_degrees_fey

#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void degrees_fey
        ( const int& weightmax
        , const int& d
        , std::list<pnet::type::value::value_type>& v
        );
    }
  }
}
#endif
