#include <pnetc/op/signature/signature_and_multiplicitie.hpp>
#include <list>
#include <we/type/value.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <numeric>
#include <sstream>
#include <tuple>
#include <../include/feynman/feynman.hpp>

namespace pnetc
{
  namespace op
  {
    namespace signature
    {
      void signature_and_multiplicitie
        ( const std::string& G
        , const std::list<pnet::type::value::value_type>& a
        , std::list<pnet::type::value::value_type>& s
        )
      {
#line 159 "/home/atraore/gpi/try_gpi/gpispace/workflow/feynman.xpnet"

        std::string G_copy=G;
        // Remove double quotes at the beginning and end
        if (!G_copy.empty() && G_copy.front() == '"' && G_copy.back() == '"') {
          G_copy = G_copy.substr(1, G_copy.size() - 2);
        }
        // Vector to store parsed values
        std::vector<std::vector<int>> Gvec;
        // Loop to parse the string
        std::vector<int> innerVec;
        std::string numString;
        bool inBraces = false;
        // Flag to track if currently parsing inside braces
        for (char ch : G_copy) {
          if (isdigit(ch)) {
            numString += ch;
          } else if (ch == ',' || ch == ' ') {
            if (!numString.empty()) {
              innerVec.push_back(std::stoi(numString));
              numString.clear();
            }
          } else if (ch == '{') {
            inBraces = true;
          } else if (ch == '}') {
            if (!numString.empty()) {
              innerVec.push_back(std::stoi(numString));
              numString.clear();
            }
            if (!innerVec.empty()) {
              Gvec.push_back(innerVec);
              innerVec.clear();
            }
            inBraces = false;
          }
          // Check if we are outside the braces and encountered a comma or space
          if (!inBraces && (ch == ',' || ch == ' ')) {
            // Reset the inner vector if outside braces
            innerVec.clear();
          }
        }
        // Construct vector of pairs
        std::vector<std::pair<int, int>> Gv;
        for (const auto& vec : Gvec) {
          if (vec.size() >= 2) {
            Gv.emplace_back(vec[0], vec[1]);
          }
        }

                  std::vector<std::tuple<int, std::vector<int>>> f;

        std::vector<int> av;
        for (const auto& elem : a) {
          if (auto ptr = boost::get<int>(&elem)) {
            av.push_back(*ptr);
          }
        }
        if (av.size() != Gv.size()) {
          throw std::runtime_error("av should be of length " + std::to_string(Gv.size()) + ", but it is of size " + std::to_string(av.size()));
        } else {
          std::unordered_set<int> nbv;
          for (const auto& e : Gv) {
            nbv.insert(e.first);
            nbv.insert(e.second);
          }
          int nv = nbv.size();
          //nb vertices.
          int N = std::accumulate(av.begin(), av.end(), 0);
          // std::vector<std::tuple<int, std::vector<int>>> f = signature_and_multiplicitie(Gv, av);
          std::vector<int> p;
          std::vector<std::tuple<int, std::vector<int>>> b;
          std::vector<int> l(nv, 0);
          for (size_t i = 0; i < Gv.size(); i++) {
            int ai = av[i];
            std::pair<int, int> ev = Gv[i];
            if (ai == 0 && ev.first != ev.second) {
              l[ev.first - 1] = 1;
              l[ev.second - 1] = 1;
            }
          }
          for (size_t i = 0; i < l.size(); i++) {
            int li = l[i];
            if (li == 1) {
              p.push_back(i + 1);
            }
          }
          std::sort(p.begin(), p.end());
          std::vector<std::vector<int>> per;
          do {
            per.push_back(p);
          }
          while (std::next_permutation(p.begin(), p.end()));
          int fact = 1;
          for (int i = 1; i <= nv; i++) {
            fact *= i;
          }
          for (const auto& ga : per) {
            std::vector<int> flip(av.size(), 0);
            for (size_t i = 0; i < av.size(); ++i) {
              int ai = av[i];
              std::pair<int, int> ev = Gv[i];
              if (ai == 0 && ev.first != ev.second) {
                int ii = -1;
                int ij = -1;
                for (size_t j = 0; j < ga.size(); ++j) {
                  if (ga[j] == ev.first) {
                    ii = j;
                  }
                  if (ga[j] == ev.second) {
                    ij = j;
                  }
                }
                if (ii != -1 && ij != -1) {
                  if (ii < ij) {
                    flip[i] = -1;
                  } else {
                    flip[i] = 0;
                  }
                }
              } else if (ev.first == ev.second) {
                flip[i] = -2;
              } else if (ai != 0 && ev.first != ev.second) {
                flip[i] = ai;
              }
            }
            std::vector<int> tmp=    flip;
            bool comp = false;
            int j = 0;
            while (!comp && j < b.size()) {
              int tt=1;
              std::vector<int> bb=std::get<1>(b[j]);
              if (tmp.size() != bb.size()) 
                            tt=0;
              for (std::size_t i = 0; i < tmp.size(); ++i) {
                if (tmp[i] != bb[i])
                                  tt=0;
              }
              if (tt) {
                // Accessing the second element of the tuple
                comp = true;
                std::get<0>(b[j]) += 1;
              } else {
                j++;
              }
            }
            if (!comp) {
              b.push_back(std::make_tuple(1, tmp));
            }
          }
          for (auto& tuple : b) {
            int& element = std::get<0>(tuple);
            // std::cout<<" element "<<element<<std::endl;
            element *= fact / per.size();
          }
          if (b.size() == 1) {
            f= b;
          } else {
            std::vector<std::tuple<int, std::vector<int>>> group;
            for (const auto& pair1 : b) {
              int n = std::get<0>(pair1);
              std::vector<int> values1 = std::get<1>(pair1);
              int mm = 2 * n;
              auto it1 = std::find(group.begin(), group.end(), pair1);
              auto it2 = std::find(group.begin(), group.end(), std::make_tuple(mm, values1));
              if (it1 != group.end() || it2 != group.end()) {
                continue;
              }
              bool equiv = false;
              for (const auto& pair2 : b) {
                int m = std::get<0>(pair2);
                std::vector<int> values2 = std::get<1>(pair2);
                int mm = 2 * n;
                auto it1 = std::find(group.begin(), group.end(), pair2);
                auto it2 = std::find(group.begin(), group.end(), std::make_tuple(2 * m, values2));
                if (it1 != group.end() || it2 != group.end()) {
                  continue;
                }
                std::vector<int> result_arr;
                for (int x : values2) {
                  result_arr.push_back(x == -1 ? 0 : (x == 0 ? -1 : x));
                }
                if (n == m && values1 == result_arr) {
                  equiv = true;
                  break;
                }
              }
              // std::cout <<" equiv = " <<equiv << std::endl;
              if (equiv) {
                group.push_back(std::make_tuple(mm, values1));
              }
            }
            f=group;
          }
        }
    
using pnet_value = pnet::type::value::value_type;
using pnet_list = std::list<pnet_value>;
    for (const auto& tuple : f) {
        int index = std::get<0>(tuple);
        const auto& fiVector = std::get<1>(tuple); // Get the vector<int> fi
        pnet_list fiList;
        for (int value : fiVector) {
            fiList.push_back(value);
        }
        std::string str;
        for (auto it = fiList.begin(); it != fiList.end(); ++it) {
            if (it != fiList.begin()) {
                str += ", ";
            }
            int listValue = boost::get<int>(*it);
            str += std::to_string(listValue);
        }
        std::string fiString = std::to_string(index) + ", " + str;
        s.push_back(fiString);
    }
    
      }
    }
  }
}