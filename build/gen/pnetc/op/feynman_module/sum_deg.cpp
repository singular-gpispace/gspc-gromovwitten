#include <pnetc/op/feynman_module/sum_deg.hpp>
#include <list>
#include <we/type/value.hpp>
#include <iostream>
#include <vector>
#include <sstream>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      void sum_deg
        ( const int& n
        , const int& d
        , std::list<pnet::type::value::value_type>& v
        )
      {
#line 23 "/home/atraore/gpi/try_gpi/gpispace/workflow/feynman.xpnet"



   for (int e = 0; e < d; e++) {
        std::list<int> x(n, 0);
        x.front() = d - e;
        x.back() = e;

        std::list<pnet::type::value::value_type> temp;
        for (int xi : x) {
            temp.push_back(pnet::type::value::value_type(xi));
        }
        v.push_back(temp);
    }

         
      }
    }
  }
}