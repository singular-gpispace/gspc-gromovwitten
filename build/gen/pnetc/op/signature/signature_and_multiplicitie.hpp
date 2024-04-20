#ifndef _PNETC_OP_signature_signature_and_multiplicitie
#define _PNETC_OP_signature_signature_and_multiplicitie

#include <list>
#include <we/type/value.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace signature
    {
      void signature_and_multiplicitie
        ( const std::string& G
        , const std::list<pnet::type::value::value_type>& a
        , std::list<pnet::type::value::value_type>& s
        );
    }
  }
}
#endif
