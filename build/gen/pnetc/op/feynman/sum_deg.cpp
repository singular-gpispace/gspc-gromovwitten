#include <pnetc/op/feynman/sum_deg.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

namespace pnetc
{
  namespace op
  {
    namespace feynman
    {
      std::string sum_deg
        ( const int& n
        , const int& d
        )
      {
#line 23 "/home/atraore/gpi/try_gpi/gpispace/workflow/feynman.xpnet"


std::vector<std::vector<int>> ru; 

    for (int e = 0; e < d; e++) {
        std::vector<int> x(n, 0);
        x[0] = d - e;
        x[n - 1] = e;
        ru.push_back(x);
    }

    std::stringstream ss;
    for (const auto& innerVec : ru) {
        for (int val : innerVec) {
            ss << val << ' ';
        }
        ss << '\n'; 
    }
    return ss.str();

          
      }
    }
  }
}