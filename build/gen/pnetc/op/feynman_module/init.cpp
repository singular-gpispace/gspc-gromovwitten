#include <pnetc/op/feynman_module/init.hpp>
#include <list>
#include <we/type/value.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <feynman.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      void init
        ( const int& d
        , std::list<pnet::type::value::value_type>& v
        )
      {
#line 34 "/home/atraore/gpi/try_gpi/gpispace/workflow/feynman.xpnet"


            // Populate the vector v with integers from 2 to d
            for (int i = 1; i <= d; ++i) {
                v.emplace_back(i);
            }
        


         
      }
    }
  }
}