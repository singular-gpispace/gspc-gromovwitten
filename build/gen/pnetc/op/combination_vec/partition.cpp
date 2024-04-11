#include <pnetc/op/combination_vec/partition.hpp>
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
    namespace combination_vec
    {
      void partition
        ( const std::list<pnet::type::value::value_type>& v
        , std::list<pnet::type::value::value_type>& vec
        )
      {
#line 60 "/home/atraore/gpi/try_gpi/gpispace/workflow/feynman.xpnet"

          std::vector<int> x;
          for (const auto& elem : v) {
            auto ptr = boost::get<int>(&elem) ;
              x.push_back(*ptr);
            
          }
          /*
            std::vector<std::vector<int>> comb = generate_combination(x);
            for (const auto& element : comb) {
            std::list<pnet::type::value::value_type> temp;
                for (int xi : element) {
                  temp.push_back(pnet::type::value::value_type(xi));
                }
                vec.push_back(temp);
            }
            */
          int k=x.size();
        if (k == 0) {
            throw std::invalid_argument("k should be nonzero");
        }
        std::vector<std::vector<int> > ru;
        int d= std::accumulate(x.begin(),x.end(),0);
        std::vector<int> xv(k , 0);
        xv[0] = d;
        if (xv==x){
        std::list<pnet::type::value::value_type> tempxv;
        for (int xi : x) {
            tempxv.push_back(pnet::type::value::value_type(xi));
        }
        vec.push_back(tempxv);
        }
        int n = 1;
        int nn=d + k - 1;
        int k1=d;
        for (int i = 1; i <= k1; ++i) {
            n *= (nn - i + 1);
            n /= i;
        }        int e = d - x[0];
        std::vector<int> a = x;    
        std::vector<int> y(k, 0); 
        y[0] = x[0] - 1;
        y[k - 1] = e + 1;
        
        for (int i = 0; i < n; ++i) {
            if (a != y) {
                //a = next_partition(a);
                for (int i = k - 1; i >= 0; --i) {
                    if (i == k - 1 && a[i] == d) {
                        //vec.push_back(a);
                        break;
                    } else {
                        for (int j = i - 1; j >= 0; --j) {
                            if (a[j] != 0) {
                                --a[j];
                                int ak = a[k - 1];
                                a[k - 1] = 0;
                                a[j + 1] = ak + 1;
                            // vec.push_back(a);
                                break;                       
                                }
                        }
                    }
                    //vec.push_back(a);
                    break;
                }
          std::list<pnet::type::value::value_type> temp_a;
        for (int xi : a) {
            temp_a.push_back(pnet::type::value::value_type(xi));
        }
        vec.push_back(temp_a);
            } else {
                break;
            }
        }

          
      }
    }
  }
}