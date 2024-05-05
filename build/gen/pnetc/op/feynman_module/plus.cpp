#include <pnetc/op/feynman_module/plus.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      void plus
        ( const unsigned long& s
        , unsigned long& sum
        )
      {
#line 296 "/home/atraore/gpi/try_gpi/gpispace/workflow/feynman.xpnet"

            sum += s;
          
      }
    }
  }
}