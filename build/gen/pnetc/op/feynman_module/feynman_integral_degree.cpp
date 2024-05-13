#include <pnetc/op/feynman_module/feynman_integral_degree.hpp>
#include <list>
#include <we/type/value.hpp>
#include <iostream>
#include <vector>
#include <numeric>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <cmath>
#include <sstream>
#include <stack>
#include <unordered_set>
#include <feynman.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      unsigned long feynman_integral_degree
        ( const std::list<pnet::type::value::value_type>& G
        , const std::list<pnet::type::value::value_type>& a
        )
      {
#line 111 "/home/atraore/gpi/try_gpi/gpispace/workflow/feynman.xpnet"

          std::vector<int> xxx; // Define xxx outside the inner loop
            for (const auto &vii : G)
            {
                if (auto ptr = boost::get<int>(&vii))
                {                             // Check if the element is an integer
                    xxx.push_back( *ptr ); // Push the integer to the vector xx
                }
            }
         std::vector<std::pair<int, int>> Gv;
        
            // Iterate over the vector of integers, creating pairs from consecutive elements
            for (size_t i = 0; i < xxx.size(); i += 2)
            {
                Gv.push_back(std::make_pair(xxx[i], xxx[i + 1]));
            }
          std::vector<int> av;
          for (const auto &xi : a)
          {
              if (auto ptr = boost::get<int>(&xi))
              {
                  av.push_back( *ptr);
              }
          }

unsigned long fe=feynman_integral_branch_type(Gv, av);
return fe;

      
      }
    }
  }
}