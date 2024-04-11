#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include "../include/feynman/feynman.hpp"
#include <functional>
#include <cmath>
#include <numeric>
#include <chrono>
#include <sstream>
#include <unordered_set>
#include <stack>


using Element = std::pair<std::pair<int, int>, std::pair<int, int>>;
using Sequence = std::vector<Element>;

Sequence  cons0(const std::vector<std::pair<int, int>>& G, int j, int N) {
    Sequence  vv;

    for (int i = 1; i <= N; ++i) {
        if (G[j].first < G[j].second)
            vv.push_back(std::make_pair(std::make_pair(G[j].first, -i), std::make_pair(G[j].second, +i)));
        else
            vv.push_back(std::make_pair(std::make_pair(G[j].second, +i), std::make_pair(G[j].first, -i)));
    }
    return vv;
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
/*
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
void CartesianRecurse(std::vector<Sequence>& accum,
                      Sequence stack,
                      const std::vector<Sequence>& sequences,
                      int index) {
    std::stack<std::tuple<Sequence, int>> stackIndex;
    stackIndex.push(std::make_tuple(stack, index));
    
    while (!stackIndex.empty()) {
        auto [curStack, curIndex] = stackIndex.top();
        stackIndex.pop();
        
        Sequence sequence = sequences[curIndex];
        
        for (const auto& tuple : sequence) {
            curStack.push_back(tuple);
            
            if (curIndex == 0) {
                accum.push_back(curStack);
            } else {
                stackIndex.push(std::make_tuple(curStack, curIndex - 1));
            }
            
            curStack.pop_back();
        }
    }
}

std::vector<Sequence > CartesianProducts(const std::vector<Sequence > &sequences) {
    
    std::vector<Sequence > accum;
    Sequence  stack;
    
    if (!sequences.empty())
        CartesianRecurse(accum, stack, sequences, sequences.size() - 1);
    
    return accum;
}
std::vector<Sequence> CartesianProductss(const std::vector<Sequence>& tmp) {
    std::vector<Sequence> cartesian_product;
    Sequence stack;

    std::stack<std::pair<Sequence, int>> stackIndex;
    if (!tmp.empty())
        stackIndex.push(std::make_pair(stack, tmp.size() - 1));

    while (!stackIndex.empty()) {
        std::pair<Sequence, int> topPair = stackIndex.top();
        stackIndex.pop();

        Sequence curStack = topPair.first;
        int curIndex = topPair.second;

        Sequence sequence = tmp[curIndex];

        for (const auto& tuple : sequence) {
            curStack.push_back(tuple);

            if (curIndex == 0) {
                cartesian_product.push_back(curStack);
            } else {
                stackIndex.push(std::make_pair(curStack, curIndex - 1));
            }

            curStack.pop_back();
        }
    }

    return cartesian_product;
}
*/

std::vector<Sequence> CartesianProduct(const std::vector<Sequence>& tmp) {
    std::vector<Sequence> cartesian_product;
    Sequence stack;

    std::stack<std::pair<Sequence, int>> stackIndex;
    if (!tmp.empty())
        stackIndex.push(std::make_pair(stack, tmp.size() - 1));

    while (!stackIndex.empty()) {
        // Retrieve the top pair from stackIndex
        std::pair<Sequence, int> topPair = stackIndex.top();
        stackIndex.pop();

        // Extract curStack and curIndex from the top pair
        Sequence curStack = topPair.first;
        int curIndex = topPair.second;

        // Retrieve the current sequence from tmp
        Sequence sequence = tmp[curIndex];

        // Iterate over elements of the current sequence
        for (const auto& tuple : sequence) {
            // Add tuple to curStack
            curStack.push_back(tuple);

            if (curIndex == 0) {
                // If at the last sequence, add curStack to cartesian_product
                cartesian_product.push_back(curStack);
            } else {
                // Push curStack and curIndex - 1 to stackIndex
                stackIndex.push(std::make_pair(curStack, curIndex - 1));
            }

            // Remove the last tuple from curStack
            curStack.pop_back();
        }
    }

    return cartesian_product;
}


bool m(const Sequence& u, std::vector<int>& v) {
    for (const auto& pp : u) {
        v[pp.first.first] += pp.first.second;
        v[pp.second.first] += pp.second.second;
    }
    return std::all_of(v.begin(), v.end(), [](int val) { return val == 0; });
}

std::vector<Sequence> mergetuples(const std::vector<Sequence>& uu) {
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

std::vector<Sequence> mergetuple(const std::vector<Sequence>& uu) {
    std::vector<Sequence> res;
    int N = 0;
    for (const auto& subvec : uu) {
        for (const auto& elem : subvec) {
            N = std::max({N, elem.first.first, elem.second.first});
        }
    }
    std::vector<int> v(N + 1, 0);
 std::vector<Sequence> cartesian_product;
    Sequence stack;

    std::stack<std::pair<Sequence, int>> stackIndex;
    if (!uu.empty())
        stackIndex.push(std::make_pair(stack, uu.size() - 1));

    while (!stackIndex.empty()) {
        // Retrieve the top pair from stackIndex
        std::pair<Sequence, int> topPair = stackIndex.top();
        stackIndex.pop();

        // Extract curStack and curIndex from the top pair
        Sequence curStack = topPair.first;
        int curIndex = topPair.second;

        // Retrieve the current sequence from tmp
        Sequence sequence = uu[curIndex];

        // Iterate over elements of the current sequence
        for (const auto& tuple : sequence) {
            // Add tuple to curStack
            curStack.push_back(tuple);

            if (curIndex == 0) {
                // If at the last sequence, add curStack to cartesian_product
                cartesian_product.push_back(curStack);
            } else {
                // Push curStack and curIndex - 1 to stackIndex
                stackIndex.push(std::make_pair(curStack, curIndex - 1));
            }

            // Remove the last tuple from curStack
            curStack.pop_back();
        }
    }



    for (const auto& u : cartesian_product) {
        bool allZero = true;
        for (const auto& pp : u) {
            v[pp.first.first] += pp.first.second;
            v[pp.second.first] += pp.second.second;
        }
        for (int val : v) {
            if (val != 0) {
                allZero = false;
                break;
            }
        }
        if (allZero) {
            res.push_back(u);
        }
        std::fill(v.begin(), v.end(), 0);
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

std::vector<std::tuple<int,std::vector<int>>> signature_and_multiplicitie(const std::vector<std::pair<int, int>>& Gv, const std::vector<int>& av) {
    std::vector<int> p;
    std::vector<std::tuple<int, std::vector<int>>> b;
    std::unordered_set<int> nbv;
    for (const auto& e : Gv) {
        nbv.insert(e.first);
        nbv.insert(e.second);
    }
    int nv = nbv.size();
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
    } while (std::next_permutation(p.begin(), p.end()));


    int fact = 1;
    for(int i = 1; i <= nv; i++) {
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

                return group;
            }
}


int feynman_integral(const std::vector<std::pair<int, int>>& Gv, const std::vector<int>& av) {
   int N = std::accumulate(av.begin(), av.end(), 0);
   // std::vector<std::tuple<int, std::vector<int>>> f = signature_and_multiplicitie(Gv, av);
    std::vector<int> p;
    std::vector<std::tuple<int, std::vector<int>>> b;
    std::unordered_set<int> nbv;
    for (const auto& e : Gv) {
        nbv.insert(e.first);
        nbv.insert(e.second);
    }
    int nv = nbv.size();
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
    } while (std::next_permutation(p.begin(), p.end()));


    int fact = 1;
    for(int i = 1; i <= nv; i++) {
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
        std::vector<std::tuple<int, std::vector<int>>> f;

    for (size_t i = 0; i < b.size(); i++) {
        for (auto& element : std::get<1>(b[i])) {
            element *= fact / per.size();
        }
    }
    if (b.size() == 1) {
                 f= b;
            } 
            else {
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
    int myfey;
    std::vector<int> fey;
    for (const auto& item : f) {
        int factor = std::get<0>(item);
        const auto& multiplicities = std::get<1>(item);
        std::vector<Sequence> tmp;
        for (size_t j = 0; j < av.size(); j++) {
            const auto& multiplicity = multiplicities[j]; 
            if (multiplicity == -1) {
                Sequence  v;

        for (int i = 1; i <= N; ++i) {
            if (Gv[j].second < Gv[j].first)
                v.push_back(std::make_pair(std::make_pair(Gv[j].second, -i), std::make_pair(Gv[j].first, +i)));
            else
                v.push_back(std::make_pair(std::make_pair(Gv[j].first, +i), std::make_pair(Gv[j].second, -i)));
        }
                        tmp.push_back(v);

            } else if (multiplicity == 0) {
            Sequence  vv;
            for (int i = 1; i <= N; ++i) {
                if (Gv[j].first < Gv[j].second)
                    vv.push_back(std::make_pair(std::make_pair(Gv[j].first, -i), std::make_pair(Gv[j].second, +i)));
                else
                    vv.push_back(std::make_pair(std::make_pair(Gv[j].second, +i), std::make_pair(Gv[j].first, -i)));
            }
                                    tmp.push_back(vv);

            } else {

                Sequence  x_powers;


    for (int w = 1; w <= multiplicity; ++w) {
        if (multiplicity % w == 0) {
            x_powers.push_back(std::make_pair(std::make_pair(Gv[j].first, +w), std::make_pair(Gv[j].second, -w)));
            x_powers.push_back(std::make_pair(std::make_pair(Gv[j].first, -w), std::make_pair(Gv[j].second, +w)));
        }
    }
                 tmp.push_back(x_powers);
            }
        }
         //   std::vector<Sequence> tt = mergetuple(tmp);
            std::vector<Sequence> res;
    int N = 0;
    for (const auto& subvec : tmp) {
        for (const auto& elem : subvec) {
            N = std::max({N, elem.first.first, elem.second.first});
        }
    }
    std::vector<int> v(N + 1, 0);
 std::vector<Sequence> cartesian_product;
    Sequence stack;

    std::stack<std::pair<Sequence, int>> stackIndex;
    if (!tmp.empty())
        stackIndex.push(std::make_pair(stack, tmp.size() - 1));

    while (!stackIndex.empty()) {
        // Retrieve the top pair from stackIndex
        std::pair<Sequence, int> topPair = stackIndex.top();
        stackIndex.pop();

        // Extract curStack and curIndex from the top pair
        Sequence curStack = topPair.first;
        int curIndex = topPair.second;

        // Retrieve the current sequence from tmp
        Sequence sequence = tmp[curIndex];

        // Iterate over elements of the current sequence
        for (const auto& tuple : sequence) {
            // Add tuple to curStack
            curStack.push_back(tuple);

            if (curIndex == 0) {
                // If at the last sequence, add curStack to cartesian_product
                cartesian_product.push_back(curStack);
            } else {
                // Push curStack and curIndex - 1 to stackIndex
                stackIndex.push(std::make_pair(curStack, curIndex - 1));
            }

            // Remove the last tuple from curStack
            curStack.pop_back();
        }
    }

    for (const auto& u : cartesian_product) {
        bool allZero = true;
        for (const auto& pp : u) {
            v[pp.first.first] += pp.first.second;
            v[pp.second.first] += pp.second.second;
        }
        for (int val : v) {
            if (val != 0) {
                allZero = false;
                break;
            }
        }
        if (allZero) {
            res.push_back(u);
        }
        std::fill(v.begin(), v.end(), 0);
    }
            //int ty = sum_absolute_products(res);
             int ty = 0;
            for (auto &t : res) {
                int abs_product = 1.0;
                for (auto &ui : t) {
                    abs_product *= abs(ui.first.second );
                }
                ty += abs_product;
            }
            fey.push_back(factor * ty);
            }
    myfey= std::accumulate(fey.begin(), fey.end(), 0.0);
    return myfey;
}
 
 

 /*
int main() {
   std::vector<std::pair<int, int>> edges = {{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}};
    std::vector<std::pair<int, int>> graph=edges;
    int n=graph.size();
    std::vector<int> aa = {0, 2, 1, 0, 0, 1};
    std::vector<int> ga = {4, 2, 3, 1};
     int fe= feynman_integral(edges,aa);

    std::cout << "feynman is "<<fe << std::endl;

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

  
 /*
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
