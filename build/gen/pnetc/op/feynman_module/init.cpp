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
        ( const int& weightmax
        , const int& d
        , std::list<pnet::type::value::value_type>& v
        )
      {
#line 37 "/home/atraore/gpi/try_gpi/gpispace/workflow/feynman.xpnet"

          int nb = number_monomial(weightmax); // Call to your number_monomial function
        if (d < nb) {
            // Throw an exception with a proper message
            throw std::runtime_error(
                "d = " + std::to_string(d) + " should be equal to or greater than the number of monomials, which is " + std::to_string(nb));
        } else {
            // Populate the vector v with integers from 2 to d
            for (int i = 1; i <= d; ++i) {
                v.emplace_back(i);
            }
        }


         
      }
    }
  }
}