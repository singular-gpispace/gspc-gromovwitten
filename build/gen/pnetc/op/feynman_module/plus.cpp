#include <pnetc/op/feynman_module/plus.hpp>
#include <string>
#include <we/type/literal/control.hpp>
#include <iostream>
#include <vector>
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
      void plus
        ( const std::string& s
        , std::string& sum
        , we::type::literal::control& ns
        )
      {
#line 263 "/home/atraore/gpi/try_gpi/gspc-gromovwitten/workflow/feynman.xpnet"

          std::cout << "s= " << s << std::endl;
          std::cout << "sum=: " << sum << std::endl;

           std::vector<fmpq_t*>v = stringToVectorFmpq(sum);
              std::vector<fmpq_t*> w = stringToVectorFmpq(s);

                // Perform the sum of the vectors
               std::vector<fmpq_t*> sum_vector = sumOfVectorsFmpq(v, w);

                // Convert the result back to a string
                sum = vectorToStringFmpq(sum_vector);
          std::cout << "sum2=: " << sum << std::endl;

          
      }
    }
  }
}