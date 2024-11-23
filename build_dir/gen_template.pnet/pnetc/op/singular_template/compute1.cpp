#include <pnetc/op/singular_template/compute1.hpp>
#include <string>
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
      void compute1
        ( const std::string& input
        , std::string& graph
        , int& degree
        , int& edges
        , int& g
        , int& deg
        , unsigned long& total
        , std::string& genus
        , std::string& loop
        )
      {
#line 56 "/home/atraore/gpi/try_gpi/gpispace/template/workflow/template.xpnet"

        std::vector<std::string> result=  RESOLVE_INTERFACE_FUNCTION( singular_template_compute_parseInput) (input);

        degree = std::stoi(result[0]);
        edges = std::stoi(result[1]);
        int _g = std::stoi(result[2]);
        graph = result[3];
     

        deg=degree;
        genus=result[4];
        std::vector<int> gg=RESOLVE_INTERFACE_FUNCTION(stringToVectorInt) (genus);
        int gs=RESOLVE_INTERFACE_FUNCTION(sumVector) (gg);
          g = 2 * (edges + gs);
          std::cout<<"g= "<<g<<std::endl;
          std::cout<<"genus= "<<genus<<std::endl;

        loop=result[5];
        total =RESOLVE_INTERFACE_FUNCTION( partialBinomialSum) (edges,degree);
std::cout<<"total= "<<total<<std::endl;
            		
      }
    }
  }
}