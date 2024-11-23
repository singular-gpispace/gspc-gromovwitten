#include <pnetc/op/singular_template/degrees_fey.hpp>
#include <list>
#include <we/type/value.hpp>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <interface/feynman.hpp>
#include <iostream>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void degrees_fey
        ( const int& weightmax
        , const int& d
        , std::list<pnet::type::value::value_type>& v
        )
      {
#line 116 "/home/atraore/gpi/try_gpi/gpispace/template/workflow/template.xpnet"

              int nb = RESOLVE_INTERFACE_FUNCTION(number_monomial)(weightmax);
         // number_monomial function
        if (d < nb) {
            // Throw an exception with a proper message
            throw std::runtime_error(
                "d = " + std::to_string(d) + " should be greater or equal than the number of monomials, which is " + std::to_string(nb));
        } else {
            // Populate the vector v with integers from 2 to d
            for (int i = 2; i <= d; ++i) {
                v.emplace_back(i);
            }
        }
         
      }
    }
  }
}