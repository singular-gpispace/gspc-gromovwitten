#include <pnetc/op/feynman_module/gen_block_.hpp>
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
      void gen_block_
        ( const int& n
        , const int& d
        , std::list<pnet::type::value::value_type>& v
        )
      {
#line 24 "/home/atraore/gpi/try_gpi/gpispace/workflow/feynman.xpnet"


          vector2d gen=gen_block(n,d);
    for (std::vector<int> ge:gen){
        std::list<pnet::type::value::value_type> temp;
        for (int xi : ge) {
            temp.push_back(pnet::type::value::value_type(xi));
        }
        v.push_back(temp);
    }

         
      }
    }
  }
}