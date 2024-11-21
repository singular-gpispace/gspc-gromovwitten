#include <pnetc/op/feynman_module/control1.hpp>
#include <we/type/literal/control.hpp>
#include <iostream>
#include <vector>
#include <sstream>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      void control1
        ( const we::type::literal::control& ns
        , unsigned long& counter_total
        )
      {
#line 294 "/home/atraore/gpi/try_gpi/gpispace/workflow/feynman.xpnet"


          counter_total +=1;
          //std::cout << "counter_total=: " << counter_total << std::endl;

         
      }
    }
  }
}