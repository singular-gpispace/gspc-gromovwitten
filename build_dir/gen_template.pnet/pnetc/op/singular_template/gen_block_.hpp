#ifndef _PNETC_OP_singular_template_gen_block_
#define _PNETC_OP_singular_template_gen_block_

#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void gen_block_
        ( const int& n
        , const int& i
        , std::list<pnet::type::value::value_type>& v
        );
    }
  }
}
#endif
