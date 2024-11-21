#include <pnetc/op/feynman_module/partition.hpp>
#include <list>
#include <we/type/value.hpp>
#include <iostream>
#include <vector>
#include <numeric>
#include <sstream>
#include <../include/feynman/feynman.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      void partition
        ( const std::list<pnet::type::value::value_type>& v
        , std::list<pnet::type::value::value_type>& vec
        )
      {
#line 97 "/home/atraore/gpi/try_gpi/gpispace/workflow/feynman.xpnet"

          std::vector<int> x;
          for (const auto& elem : v) {
            auto ptr = boost::get<int>(&elem) ;
              x.push_back(*ptr);
            
          }
      
    vector2d gen=iterate( x);
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