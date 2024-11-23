#ifndef _PNETC_OP_feynman_module_plus
#define _PNETC_OP_feynman_module_plus

#include <string>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
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
