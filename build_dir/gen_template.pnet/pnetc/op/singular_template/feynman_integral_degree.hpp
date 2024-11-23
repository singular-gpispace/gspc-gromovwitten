#ifndef _PNETC_OP_singular_template_feynman_integral_degree
#define _PNETC_OP_singular_template_feynman_integral_degree

#include <list>
#include <we/type/value.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void feynman_integral_degree
        ( const std::string& genus
        , const std::string& loop
        , const int& deg
        , const std::string& graph
        , const std::list<pnet::type::value::value_type>& a
        , std::string& s
        );
    }
  }
}
#endif
