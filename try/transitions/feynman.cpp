#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include "../../workflow/feynman.hpp"
#include <functional>
#include <cmath>
#include <numeric>
#include <chrono>


using Element = std::pair<std::pair<int, int>, std::pair<int, int>>;
using Sequence = std::vector<Element>;

Sequence  cons0(const std::vector<std::pair<int, int>>& G, int j, int N) {
    Sequence  v;

    for (int i = 1; i <= N; ++i) {
        if (G[j].first < G[j].second)
            v.push_back(std::make_pair(std::make_pair(G[j].first, -i), std::make_pair(G[j].second, +i)));
        else
            v.push_back(std::make_pair(std::make_pair(G[j].second, +i), std::make_pair(G[j].first, -i)));
    }
    return v;
}

Sequence  cons(const std::vector<std::pair<int, int>>& G, int j, int N) {
    Sequence  v;

    for (int i = 1; i <= N; ++i) {
        if (G[j].second < G[j].first)
            v.push_back(std::make_pair(std::make_pair(G[j].second, -i), std::make_pair(G[j].first, +i)));
        else
            v.push_back(std::make_pair(std::make_pair(G[j].first, +i), std::make_pair(G[j].second, -i)));
    }
    return v;
}
std::vector<std::vector<int>> pairsToVectorOfVectors(const std::vector<std::pair<int, int>>& pairs) {
    std::vector<std::vector<int>> result;
    for (const auto& pair : pairs) {
        std::vector<int> temp;
        temp.push_back(pair.first);
        temp.push_back(pair.second);
        result.push_back(temp);
    }
    return result;
}

std::vector<std::pair<int, int>> vectorOfVectorsToPairs(const std::vector<std::vector<int>>& vecOfVec) {
    std::vector<std::pair<int, int>> result;
    for (const auto& vec : vecOfVec) {
        if (vec.size() >= 2) {
            result.emplace_back(vec[0], vec[1]);
        }
    }
    return result;
}

Sequence  prot(const std::vector<std::pair<int, int>>& G, int j, int a, int N) {
    Sequence  x_powers;


    for (int w = 1; w <= a; ++w) {
        if (a % w == 0) {
            x_powers.push_back(std::make_pair(std::make_pair(G[j].first, +w), std::make_pair(G[j].second, -w)));
            x_powers.push_back(std::make_pair(std::make_pair(G[j].first, -w), std::make_pair(G[j].second, +w)));
        }
    }
    return x_powers;
}
void sum_exp(std::vector<int>& res, const std::vector<std::vector<std::pair<int, int>>>& uu) {
    for (const auto& pairs : uu) {
        for (const auto& pair : pairs) {
            int p1 = pair.first;
            int p2 = pair.second;
            int len = res.size();

            if (p1 > len) {
                res.resize(p1); // Resize vector to new size
                for (int i = len; i < p1; ++i) {
                    res[i] = 0;
                }
            }

            res[p1] += p2;
        }
    }
}
void CartesianRecurse(std::vector<Sequence > &accum,
                      Sequence  stack,
                      const std::vector<Sequence > &sequences,
                      int index) {
    Sequence  sequence = sequences[index];
    
    for (const auto& tuple : sequence) {
        stack.push_back(tuple);
        
        if (index == 0) {
            // If this is the last sequence, add the current stack to the accum vector
            accum.push_back(stack);
        } else {
            // Otherwise, recursively call CartesianRecurse with the next sequence
            CartesianRecurse(accum, stack, sequences, index - 1);
        }
        
        // Remove the last element from the stack for backtracking
        stack.pop_back();
    }
}

std::vector<Sequence > CartesianProduct(const std::vector<Sequence > &sequences) {
    
    std::vector<Sequence > accum;
    Sequence  stack;
    
    if (!sequences.empty())
        CartesianRecurse(accum, stack, sequences, sequences.size() - 1);
    
    return accum;
}
bool m(const Sequence& u, std::vector<int>& v) {
    for (const auto& pp : u) {
        v[pp.first.first] += pp.first.second;
        v[pp.second.first] += pp.second.second;
    }
    return std::all_of(v.begin(), v.end(), [](int val) { return val == 0; });
}

std::vector<Sequence> mergetuple(const std::vector<Sequence>& uu) {
    std::vector<Sequence> res;
    int N = 0;
    for (const auto& subvec : uu) {
        for (const auto& elem : subvec) {
            N = std::max({N, elem.first.first, elem.second.first});
        }
    }
    std::vector<int> v(N + 1, 0);

    std::function<void(const Sequence&, std::vector<int>&)> m_wrapper = [&](const Sequence& u, std::vector<int>& v) {
        if (m(u, v)) {
            res.push_back(u);
        }
        std::fill(v.begin(), v.end(), 0);
    };

    std::vector<std::vector<Element>> cartesian_product = CartesianProduct(uu);

    for (const auto& u : cartesian_product) {
        m_wrapper(u, v);
    }
    return res;
}

int sum_absolute_products(std::vector<Sequence> tt) {
    int total_abs_product = 0;
    for (auto &t : tt) {
        int abs_product = 1.0;
        for (auto &ui : t) {
            abs_product *= abs(ui.first.second );
        }
        total_abs_product += abs_product;
    }
    return total_abs_product;
}

std::vector<std::tuple<int,std::vector<int>>> signature_and_multiplicitie(const std::vector<std::pair<int, int>>& G, const std::vector<int>& a) {
    std::vector<int> p;
    std::vector<std::tuple<int, std::vector<int>>> b;
    std::unordered_set<int> nbv;
    for (const auto& e : G) {
        nbv.insert(e.first);
        nbv.insert(e.second);
    }
    int nv = nbv.size();
    std::vector<int> l(nv, 0);

    for (size_t i = 0; i < G.size(); i++) {
        int ai = a[i];
        std::pair<int, int> ev = G[i];
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
    } while (std::next_permutation(p.begin(), p.end()));


    int fact = 1;
    for(int i = 1; i <= nv; i++) {
        fact *= i;
    }

    for (const auto& ga : per) {
        std::vector<int> flip(a.size(), 0);

    for (size_t i = 0; i < a.size(); ++i) {
        int ai = a[i];
        std::pair<int, int> ev = G[i];

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

             if (tt) { // Accessing the second element of the tuple
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
    for (size_t i = 0; i < b.size(); i++) {
        for (auto& element : std::get<1>(b[i])) {
            element *= fact / per.size();
        }
    }
    if (b.size() == 1) {
                return b;
            } 
            else {
                std::vector<std::tuple<int, std::vector<int>>> group;

                for (const auto& pair1 : b) {
                    int n = std::get<0>(pair1);
                    std::vector<int> values1 = std::get<1>(pair1);
                /* std::cout<<n<<std::endl;
                    std::cout<<"[";
                    for (int val:values1){
                        std::cout<<""<<val<<" ";
                    }
                    std::cout << "]" << std::endl;
                    */
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
                        /* std::cout<<"values2"<<std::endl;
                            std::cout << '[';
                            bool first = true;
                            for (auto const& e : values2) {
                                if (first) { 
                                    first = false; 
                                } else { 
                                    std::cout << ", "; 
                                }
                            std::cout << e;
                            }
                            std::cout << ']' << std::endl;
                            */
                            equiv = true;
                            break;
                        }
                    }
                // std::cout <<" equiv = " <<equiv << std::endl;

                    if (equiv) {
                        group.push_back(std::make_tuple(mm, values1));
                    }
                }

                return group;
            }
}


int feynman_integral(const std::vector<std::pair<int, int>>& G, const std::vector<int>& a) {
    int N = std::accumulate(a.begin(), a.end(), 0);
    std::vector<std::tuple<int, std::vector<int>>> f = signature_and_multiplicitie(G, a);
    std::vector<int> fey;
    for (const auto& item : f) {
        int factor = std::get<0>(item);
        const auto& multiplicities = std::get<1>(item);
        std::vector<Sequence> tmp;
        for (size_t j = 0; j < a.size(); j++) {
            const auto& multiplicity = multiplicities[j]; 
            if (multiplicity == -1) {
                tmp.push_back(cons(G, j , N));
            } else if (multiplicity == 0) {
                tmp.push_back(cons0(G, j , N));
            } else {
                tmp.push_back(prot(G, j , multiplicity, N));
            }
        }
            std::vector<Sequence> tt = mergetuple(tmp);
            int ty = sum_absolute_products(tt);
            fey.push_back(factor * ty);
            }
    return std::accumulate(fey.begin(), fey.end(), 0.0);
}

 /*
int main() {
   std::vector<std::pair<int, int>> edges = {{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}};
    std::vector<std::pair<int, int>> graph(edges);
    int n=graph.size();
    std::vector<int> aa = {0, 2, 1, 0, 0, 1};
    std::vector<int> ga = {4, 2, 3, 1};

     std::cout << "signature_multiplicities is " << std::endl;
    std::vector<std::tuple<int, std::vector<int>>> ss = signature_and_multiplicitie(graph, aa);
    for (const auto& tuple : ss) {
        std::cout << std::get<0>(tuple) << ", ";
        for (const auto& val : std::get<1>(tuple)) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    signature_and_multiplicitie(graph, aa);
   
   // Define the input vectors
     std::vector<Sequence > uu={
 {{{1, 1}, {3, -1}}, {{1, 2}, {3, -2}}, {{1, 3}, {3, -3}}, {{1, 4}, {3, -4}}},
 {{{1, 1}, {2, -1}}, {{1, -1}, {2, 1}}},
 {{{1, 1}, {2, -1}}, {{1, -1}, {2, 1}}, {{1, 3}, {2, -3}}, {{1, -3}, {2, 3}}},
 {{{2, -1}, {4, 1}}, {{2, -2}, {4, 2}}, {{2, -3}, {4, 3}}, {{2, -4}, {4, 4}}},
 {{{3, 1}, {4, -1}}, {{3, 2}, {4, -2}}, {{3, 3}, {4, -3}}, {{3, 4}, {4, -4}}},
 {{{3, 1}, {4, -1}}, {{3, 2}, {4, -2}}, {{3, 3}, {4, -3}}, {{3, 4}, {4, -4}}}
};

     std::vector<Sequence> res = mergetuple(uu);

    // Output the result
    for (const auto& vec : res) {
        for (const auto& tuple : vec) {
            std::cout << "(( " << tuple.first.first << ", " << tuple.first.second << " ), ( "
                      << tuple.second.first << ", " << tuple.second.second << ")), ";
        }
        std::cout << std::endl;
    }

std::cout << "Sum of absolute products: " << sum_absolute_products(res) << std::endl;
  int fe= feynman_integral(graph,aa);

 std::cout << "feynman is"<<feynman_integral(graph,aa) << std::endl;

// Start measuring time
    auto start_time = std::chrono::steady_clock::now();

    // Call your function
    feynman_integral(graph,aa);

    // End measuring time
    auto end_time = std::chrono::steady_clock::now();

    // Calculate duration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Print duration
    std::cout << "Time taken by feynman_integral_b: " << duration.count() << " milliseconds" << std::endl << std::flush;
    int d=4;
    std::vector<int> fey=feynman_integral_degree(graph,d);
    std::cout<<"[";
    for(int fe:fey){
        std::cout<<""<<fe<<" ";
    }
    std::cout << "]" ;
    
    return 0;
}
*/
