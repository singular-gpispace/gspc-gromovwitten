#ifndef _PNETC_OP_feynman_module_feynman_integral_degree
#define _PNETC_OP_feynman_module_feynman_integral_degree

#include <list>
#include <we/type/value.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      void feynman_integral_degree
        ( const std::list<pnet::type::value::value_type>& G
        , const std::list<pnet::type::value::value_type>& a
        , const std::string& gg
        , const std::string& l
        , const int& deg
        , std::string& s
        );
    }
  }
}
#endif
