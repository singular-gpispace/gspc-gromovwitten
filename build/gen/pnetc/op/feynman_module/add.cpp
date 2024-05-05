#include <pnetc/op/feynman_module/add.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      unsigned long add
        ( const unsigned long& l
        , const unsigned long& r
        )
      {
#line 333 "/home/atraore/gpi/try_gpi/gpispace/workflow/feynman.xpnet"

        unsigned long su = l + r;
        return su;
        
      }
    }
  }
}