#ifndef _PNETC_OP_singular_template_plus
#define _PNETC_OP_singular_template_plus

#include <string>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void plus
        ( const std::string& s
        , std::string& sum
        , we::type::literal::control& ns
        );
    }
  }
}
#endif
