#include <pnetc/op/feynman_module/final.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      void final
        ( const unsigned long& total
        , const unsigned long& counter_total
        , std::string& answer
        )
      {
#line 328 "/home/atraore/gpi/try_gpi/gspc-gromovwitten/workflow/feynman.xpnet"

          std::cout << "answer=: " << answer << std::endl;
          std::cout << "total=: " << total << std::endl;


         
      }
    }
  }
}