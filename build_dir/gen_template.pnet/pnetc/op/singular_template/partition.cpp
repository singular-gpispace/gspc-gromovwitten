#include <pnetc/op/singular_template/partition.hpp>
#include <list>
#include <we/type/value.hpp>
#include <iostream>
#include <vector>
#include <numeric>
#include <sstream>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <interface/feynman.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void partition
        ( const std::list<pnet::type::value::value_type>& v
        , std::list<pnet::type::value::value_type>& vec
        )
      {
#line 188 "/home/atraore/gpi/try_gpi/gspc-gromovwitten/template/workflow/template.xpnet"

    std::vector<int> x;
    for (const auto& elem : v) {
      auto ptr = boost::get<int>(&elem) ;
        x.push_back(*ptr);
      }
        vector2d gen = RESOLVE_INTERFACE_FUNCTION(iterate)(x);

    using pnet_value = pnet::type::value::value_type;
    using pnet_list = std::list<pnet_value>;
        for (std::vector<int> &a : gen)
    {
        pnet_list temp_a;
        for (int xi : a)
        {
            temp_a.push_back(pnet::type::value::value_type(xi));
        }
        vec.push_back(temp_a);
    }

          
      }
    }
  }
}