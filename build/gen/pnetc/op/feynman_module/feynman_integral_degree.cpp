#include <pnetc/op/feynman_module/feynman_integral_degree.hpp>
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
#include <feynman.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      unsigned long feynman_integral_degree
        ( const std::string& G
        , const std::string& a
        )
      {
#line 214 "/home/atraore/gpi/try_gpi/gpispace/workflow/feynman.xpnet"

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
 std::istringstream iss(a);

// Read the factor from the string
int factor;
char comma;
iss >> factor >> comma;

// Read the vector of integers from the string
std::vector<int> av;
int value;
while (iss >> value) {
    av.push_back(value); // Read each integer and push it into the vector
    iss >> comma; // Read the comma after each integer
}
unsigned long fe= feynman_integral_type( Gv,  std::make_tuple(factor, std::vector<int>{}), av);
return fe;

      
      }
    }
  }
}