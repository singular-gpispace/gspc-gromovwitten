#include <pnetc/op/feynman_module/feynman_integral_degree.hpp>
#include <list>
#include <we/type/value.hpp>
#include <string>
#include <iostream>
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
#include <feynman.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      void feynman_integral_degree
        ( const std::list<pnet::type::value::value_type>& G
        , const std::list<pnet::type::value::value_type>& a
        , const std::string& gg
        , const std::string& l
        , const int& deg
        , std::string& s
        )
      {
#line 162 "/home/atraore/gpi/try_gpi/gspc-gromovwitten/workflow/feynman.xpnet"

          std::vector<int> xxx; // Define xxx outside the inner loop
            for (const auto &vii : G)
            {
                if (auto ptr = boost::get<int>(&vii))
                {                             // Check if the element is an integer
                    xxx.push_back( *ptr ); // Push the integer to the vector xx
                }
            }
          std::vector<std::pair<int, int>> Gv;
        
            // Iterate over the vector of integers, creating pairs from consecutive elements
            for (size_t i = 0; i < xxx.size(); i += 2)
            {
                Gv.push_back(std::make_pair(xxx[i], xxx[i + 1]));
            }


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

    
/*************************************************************************
**************************************************************************/
std::vector<int> g=stringToVectorInt(gg);
std::vector<int> ll= stringToVectorInt(l);
/*************************************************************************
**************************************************************************/
fmpq_t result;
        fmpq_init(result);

        

 fmpq_t fey_branch_av;
  fmpq_init(fey_branch_av);

    // Compute Feynman integral of degree d
  feynman_integral_branch_type(fey_branch_av, Gv, av, g, ll);
   

        std::vector<fmpq_t*> v(deg+1, nullptr);
     v[c] = (fmpq_t*)malloc(sizeof(fmpq_t));
    fmpq_init(*v[c]);

    // Copy the value of fey_branch_av to v[c]
    fmpq_set(*v[c], fey_branch_av);

    // Convert the vector to a string
     s = vectorToStringFmpq(v);
    //std::cout << "c= " << c << "deg "<<deg<<std::endl;
    //std::cout << "V==: " << s << std::endl;

    // Clear the fmpq value after usage
    fmpq_clear(fey_branch_av);
    fmpq_clear(*v[c]);
    free(v[c]);

      
      }
    }
  }
}