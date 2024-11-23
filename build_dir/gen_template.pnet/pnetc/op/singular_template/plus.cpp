#include <pnetc/op/singular_template/plus.hpp>
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
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <interface/feynman.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void plus
        ( const std::string& s
        , std::string& sum
        , we::type::literal::control& ns
        )
      {
#line 342 "/home/atraore/gpi/try_gpi/gpispace/template/workflow/template.xpnet"


          std::cout << "sum2= and: " << sum << std::endl;
        std::vector<fmpq_t*> v = RESOLVE_INTERFACE_FUNCTION( stringToVectorFmpq) (sum);
        std::vector<fmpq_t*> w = RESOLVE_INTERFACE_FUNCTION( stringToVectorFmpq) (s);

        //  sum of the vectors
        std::vector<fmpq_t*>  sum_vector = RESOLVE_INTERFACE_FUNCTION( sumOfVectorsFmpq) (v,w);

        // Convert the result back to a string
      sum=  RESOLVE_INTERFACE_FUNCTION( vectorToStringFmpq) (sum_vector);

          
      }
    }
  }
}