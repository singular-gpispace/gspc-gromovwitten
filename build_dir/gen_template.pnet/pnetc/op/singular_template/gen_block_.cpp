#include <pnetc/op/singular_template/gen_block_.hpp>
#include <list>
#include <we/type/value.hpp>
#include <iostream>
#include <vector>
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
      void gen_block_
        ( const int& n
        , const int& i
        , std::list<pnet::type::value::value_type>& v
        )
      {
#line 154 "/home/atraore/gpi/try_gpi/gpispace/template/workflow/template.xpnet"

            using vector2d = std::vector<std::vector<int>>;
        vector2d gen = RESOLVE_INTERFACE_FUNCTION(gen_block)(n,i);

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