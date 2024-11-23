#include <pnetc/op/singular_template/feynman_integral_degree.hpp>
#include <list>
#include <we/type/value.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <numeric>
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
      void feynman_integral_degree
        ( const std::string& genus
        , const std::string& loop
        , const int& deg
        , const std::string& graph
        , const std::list<pnet::type::value::value_type>& a
        , std::string& s
        )
      {
#line 244 "/home/atraore/gpi/try_gpi/gpispace/template/workflow/template.xpnet"

         // Extract integer pairs from the input string
 std::vector<std::pair<int, int>> Gv  =  RESOLVE_INTERFACE_FUNCTION( extractIntegerPairs) (graph);
/****************************************************************************/
          std::vector<int> av;
          int c=0;
          for (const auto &xi : a)
          {
              if (auto ptr = boost::get<int>(&xi))
              {
                  av.push_back( *ptr);
                  c+=*ptr;
              }
          }
    std::cout<<"genus= "<<genus<<std::endl;
    std::cout<<"= "<<genus<<std::endl;

/*
************************************************************************
*************************************************************************
*/
std::vector<int> g=RESOLVE_INTERFACE_FUNCTION(stringToVectorInt) (genus);
std::vector<int> l=RESOLVE_INTERFACE_FUNCTION(stringToVectorInt) (loop);
/*
************************************************************************
*************************************************************************
*/

          fmpq_t result;
        RESOLVE_INTERFACE_FUNCTION(my_init_fmpq) (result);

        

 fmpq_t fey_branch_av;
        RESOLVE_INTERFACE_FUNCTION(my_init_fmpq) (fey_branch_av);

    // Compute Feynman integral of degree d
     RESOLVE_INTERFACE_FUNCTION(feynman_integral_branch_type) (fey_branch_av, Gv, av, g, l);

        std::vector<fmpq_t*> v(deg+1, nullptr);
     v[c] = (fmpq_t*)malloc(sizeof(fmpq_t));
    RESOLVE_INTERFACE_FUNCTION(my_init_fmpq) (*v[c]);

    // Copy the value of fey_branch_av to v[c]
    RESOLVE_INTERFACE_FUNCTION(my_fmpq_set) (*v[c], fey_branch_av);

    // Convert the vector to a string
    s=  RESOLVE_INTERFACE_FUNCTION( vectorToStringFmpq) (v);

    //std::cout << "c= " << c << "deg "<<deg<<std::endl;
    //std::cout << "V==: " << s << std::endl;

    // Clear the fmpq value after usage
        RESOLVE_INTERFACE_FUNCTION(my_clear) (fey_branch_av);
    RESOLVE_INTERFACE_FUNCTION(my_clear) (*v[c]);
    free(v[c]);

/* ************************************************************************** */
/* ************************************************************************** */
  
      
      
      }
    }
  }
}