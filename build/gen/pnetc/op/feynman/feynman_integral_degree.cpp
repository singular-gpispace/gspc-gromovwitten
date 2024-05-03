#include <pnetc/op/feynman/feynman_integral_degree.hpp>
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
#include <../include/feynman/feynman.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman
    {
      int feynman_integral_degree
        ( const std::string& G
        , const std::string& a
        )
      {
#line 406 "/home/atraore/gpi/try_gpi/gpispace/workflow/feynman.xpnet"

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
if (av.size() != Gv.size()) {
	throw std::runtime_error("av should be of length " + std::to_string(Gv.size()) + ", but it is of size " + std::to_string(av.size()));
} 
else {
	std::unordered_set<int> nbv;
	for (const auto& e : Gv) {
		nbv.insert(e.first);
		nbv.insert(e.second);
	}
	int nv = nbv.size();
	//nb vertices.
  int N = std::accumulate(av.begin(), av.end(), 0, [](int sum, int val) {
        return sum + (val > 0 ? val : 0);
    });        
    
  int myfey;
      std::vector<int> fey;
        for (int i = 1; i <= 1; ++i) {
            int j=0;
            std::vector<Sequence> tmp;
                for (const auto& multiplicity : av) {
                    if (multiplicity == -1) {
                                    Sequence v;

                        for (int i = 1; i <= N; ++i) {

                        if (Gv[j].second < Gv[j].first)
                            v.push_back(std::make_pair(std::make_pair(Gv[j].second, -i),
                                                    std::make_pair(Gv[j].first, +i)));
                        else
                            v.push_back(std::make_pair(std::make_pair(Gv[j].first, +i),
                                                    std::make_pair(Gv[j].second, -i)));
                        }
                            /*   std::cout << "cons result:" << std::endl;
                                                    std::cout << "multiplicity :" <<multiplicity <<std::endl;

                                for (const auto& item : v) {  
                                std::cout << "(" << item.first.first << ", " << item.first.second << "), (" 
                                        << item.second.first << ", " << item.second.second << ")" << std::endl;
                                }
                                std::cout  << std::endl; */
                        tmp.push_back(v);
                    } 
                    else if (multiplicity == 0) {
                                    Sequence vv;

                        for (int i = 1; i <= N; ++i) {
                            if (Gv[j].first < Gv[j].second){
                                vv.push_back(std::make_pair(std::make_pair(Gv[j].first, -i),
                                                            std::make_pair(Gv[j].second, +i)));
                            }
                            else{
                                vv.push_back(std::make_pair(std::make_pair(Gv[j].second, +i),
                                                            std::make_pair(Gv[j].first, -i)));
                            }
                        }
                        tmp.push_back(vv);
                        /*  std::cout << "cons result 0 :" << std::endl;
                        for (const auto& item : vv) {  
                        std::cout << "(" << item.first.first << ", " << item.first.second << "), (" 
                                << item.second.first << ", " << item.second.second << ")" << std::endl;
                        }
                        std::cout << " " << std::endl; */

                    } 
                    else {
                            Sequence x_powers;

                        for (int w = 1; w <= multiplicity; ++w) {
                            if (multiplicity % w == 0) {
                                x_powers.push_back(
                                    std::make_pair(std::make_pair(Gv[j].first, +w),
                                                std::make_pair(Gv[j].second, -w)));
                                x_powers.push_back(
                                    std::make_pair(std::make_pair(Gv[j].first, -w),
                                                std::make_pair(Gv[j].second, +w)));
                            }
                        }
                        tmp.push_back(x_powers);
                    }

                    j++;
                }

                
                      std::unordered_map<int, std::vector<std::pair<int, int>>> positions_dict;

                    for (int i = 0; i < Gv.size(); i++)
                    {
                        int x = Gv[i].first;
                        int y = Gv[i].second;

                        if (positions_dict.count(x))
                        {
                            positions_dict[x].emplace_back(i, 0);
                        }
                        else
                        {
                            positions_dict[x] = {{i, 0}};
                        }

                        if (positions_dict.count(y))
                        {
                            positions_dict[y].emplace_back(i, 1);
                        }
                        else
                        {
                            positions_dict[y] = {{i, 1}};
                        }
                    }
                    std::unordered_map<int, std::vector<std::pair<int, int>>> positions = positions_dict;

                    std::vector<Sequence> uni2(tmp.size());
                    for (size_t u = 0; u < tmp.size(); u++)
                    {
                        uni2[u] = Sequence(tmp[u].size(), std::make_pair(std::make_pair(0, 0), std::make_pair(0, 0)));
                    }

                    for (const auto &pair : Gv)
                    {
                        int i1 = pair.first;
                        int i2 = pair.second;
                        int idx11 = positions.at(i1)[0].first;
                        int pos11 = positions.at(i1)[0].second;

                        int idx12 = positions.at(i2)[0].first;
                        int pos12 = positions.at(i2)[0].second;

                        int idx21 = positions.at(i1)[1].first;
                        int pos21 = positions.at(i1)[1].second;
                        int idx22 = positions.at(i2)[1].first;
                        int pos22 = positions.at(i2)[1].second;
                        int idx31 = positions.at(i1)[2].first;
                        int pos31 = positions.at(i1)[2].second;
                        int idx32 = positions.at(i2)[2].first;
                        int pos32 = positions.at(i2)[2].second;
                        for (size_t j11 = 0; j11 < tmp[idx11].size(); j11++)
                        {
                            int w11 = (pos11 == 0) ? tmp[idx11][j11].first.second : tmp[idx11][j11].second.second;

                            for (size_t j12 = 0; j12 < tmp[idx12].size(); j12++)
                            {
                                int w12 = (pos12 == 0) ? tmp[idx12][j12].first.second : tmp[idx12][j12].second.second;

                                for (size_t j21 = 0; j21 < tmp[idx21].size(); j21++)
                                {
                                    int w21 = (pos21 == 0) ? tmp[idx21][j21].first.second : tmp[idx21][j21].second.second;

                                    for (size_t j22 = 0; j22 < tmp[idx22].size(); j22++)
                                    {
                                        int w22 = (pos22 == 0) ? tmp[idx22][j22].first.second : tmp[idx22][j22].second.second;

                                        for (size_t j31 = 0; j31 < tmp[idx31].size(); j31++)
                                        {
                                            int w31 = (pos31 == 0) ? tmp[idx31][j31].first.second : tmp[idx31][j31].second.second;

                                            for (size_t j32 = 0; j32 < tmp[idx32].size(); j32++)
                                            {
                                                int w32 = (pos32 == 0) ? tmp[idx32][j32].first.second : tmp[idx32][j32].second.second;

                                                if (w11 + w21 + w31 == 0 && w12 + w22 + w32 == 0)
                                                {

                                                    uni2[idx11][j11] = tmp[idx11][j11];
                                                    uni2[idx21][j21] = tmp[idx21][j21];
                                                    uni2[idx31][j31] = tmp[idx31][j31];
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    std::vector<Sequence> uni_f2;
                    for (const auto &subvec : uni2)
                    {
                        Sequence filtered_subvec;
                        for (const auto &elem : subvec)
                        {
                            if (elem != std::make_pair(std::make_pair(0, 0), std::make_pair(0, 0)))
                            {
                                filtered_subvec.push_back(elem);
                            }
                        }
                        uni_f2.push_back(filtered_subvec);
                    }
                    //   std::vector<Sequence> tt = mergetuple(tmp);

                    int Nm = 0;
                    for (const auto &subvec : uni_f2)
                    {
                        for (const auto &elem : subvec)
                        {
                            Nm = std::max({Nm, elem.first.first, elem.second.first});
                        }
                    }
                    std::vector<int> vy(Nm + 1, 0);
                    std::vector<Sequence> cartesian_product;
                    Sequence stack;

                    std::stack<std::pair<Sequence, int>> stackIndex;
                    if (!uni_f2.empty())
                        stackIndex.push(std::make_pair(stack, uni_f2.size() - 1));

                    while (!stackIndex.empty())
                    {
                        // Retrieve the top pair from stackIndex
                        std::pair<Sequence, int> topPair = stackIndex.top();
                        stackIndex.pop();

                        // Extract curStack and curIndex from the top pair
                        Sequence curStack = topPair.first;
                        int curIndex = topPair.second;

                        // Retrieve the current sequence from uni_f2
                        Sequence sequence = uni_f2[curIndex];

                        // Iterate over elements of the current sequence
                        for (const auto &tuple : sequence)
                        {
                            // Add tuple to curStack
                            curStack.push_back(tuple);

                            if (curIndex == 0)
                            {
                                // If at the last sequence, add curStack to cartesian_product
                                cartesian_product.push_back(curStack);
                            }
                            else
                            {
                                // Push curStack and curIndex - 1 to stackIndex
                                stackIndex.push(std::make_pair(curStack, curIndex - 1));
                            }

                            // Remove the last tuple from curStack
                            curStack.pop_back();
                        }
                    }
                    std::vector<Sequence> res;

                    for (const auto &u : cartesian_product)
                    {
                        bool allZero = true;
                        for (const auto &pp : u)
                        {
                            vy[pp.first.first] += pp.first.second;
                            vy[pp.second.first] += pp.second.second;
                        }
                        for (int val : vy)
                        {
                            if (val != 0)
                            {
                                allZero = false;
                                break;
                            }
                        }
                        if (allZero)
                        {
                            res.push_back(u);
                        }
                        std::fill(vy.begin(), vy.end(), 0);
                    }

                    // int ty = sum_absolute_products(res);
                    int ty = 0;
                    for (auto& t : res) {
                        int abs_product = 1.0;
                        for (auto& ui : t) {
                            abs_product *= abs(ui.first.second);
                        }
                        ty += abs_product;
                    }
                        fey.push_back(factor * ty);
            myfey = std::accumulate(fey.begin(), fey.end(), 0.0);

          //std::cout<<"myfey "<< myfey<<std::endl;

          //std::cout<<"myfey "<< myfey<<std::endl;
    }        
          return myfey;
}  
        
      }
    }
  }
}