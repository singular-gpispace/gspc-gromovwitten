#ifndef _PNETC_OP_feynman_module_signature_and_multiplicities
#define _PNETC_OP_feynman_module_signature_and_multiplicities

#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      void signature_and_multiplicities
        ( const std::list<pnet::type::value::value_type>& G
        , const std::list<pnet::type::value::value_type>& a
        , std::list<pnet::type::value::value_type>& s
        );
    }
  }
}
#endif
