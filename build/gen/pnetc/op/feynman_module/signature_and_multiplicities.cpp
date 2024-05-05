#include <pnetc/op/feynman_module/signature_and_multiplicities.hpp>
#include <list>
#include <we/type/value.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <numeric>
#include <sstream>
#include <algorithm>
#include <tuple>
#include <feynman.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      void signature_and_multiplicities
        ( const std::string& G
        , const std::list<pnet::type::value::value_type>& a
        , std::list<pnet::type::value::value_type>& s
        )
      {
#line 105 "/home/atraore/gpi/try_gpi/gpispace/workflow/feynman.xpnet"

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

            std::vector<int> av;
        for (const auto& elem : a) {
          if (auto ptr = boost::get<int>(&elem)) {
            av.push_back(*ptr);
          }
        }

    std::vector<std::tuple<int, std::vector<int>>> f= signature_and_multiplicitie( Gv,   av);
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