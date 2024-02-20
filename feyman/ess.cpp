#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include "feynman.hpp"
#include <functional>
#include <cmath>
#include <numeric>

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> cons0(const FeynmanGraph& G, int j, int N) {
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> v;
   std::vector<Edge> ee=G.edges();
    for (int i = 1; i <= N; ++i) {
        if (ee[j].src < ee[j].dst)
            v.push_back(std::make_pair(std::make_pair(ee[j].src, -i), std::make_pair(ee[j].dst, +i)));
        else
            v.push_back(std::make_pair(std::make_pair(ee[j].dst, +i), std::make_pair(ee[j].src, -i)));
    }
    return v;
}

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> cons(const FeynmanGraph& G, int j, int N) {
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> v;
   std::vector<Edge> ee=G.edges();
    for (int i = 1; i <= N; ++i) {
        if (ee[j].dst < ee[j].src)
            v.push_back(std::make_pair(std::make_pair(ee[j].dst, -i), std::make_pair(ee[j].src, +i)));
        else
            v.push_back(std::make_pair(std::make_pair(ee[j].src, +i), std::make_pair(ee[j].dst, -i)));
    }
    return v;
}

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> prot(const FeynmanGraph& G, int j, int a, int N) {
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> x_powers;
   std::vector<Edge> ee=G.edges();

    for (int w = 1; w <= a; ++w) {
        if (a % w == 0) {
            x_powers.push_back(std::make_pair(std::make_pair(ee[j].src, +w), std::make_pair(ee[j].dst, -w)));
            x_powers.push_back(std::make_pair(std::make_pair(ee[j].src, -w), std::make_pair(ee[j].dst, +w)));
        }
    }
    return x_powers;
}

// Function to replace -1 with 0 and 0 with -1 in a vector
std::vector<int> replaces(const std::vector<int>& arr) {
    std::vector<int> result_arr;
    for (int x : arr) {
        result_arr.push_back(x == -1 ? 0 : (x == 0 ? -1 : x));
    }
    return result_arr;
}

// Function to count the number of zeros in a vector
int count_zero(const std::vector<int>& arr) {
    int count_zeros = 0;
    for (int element : arr) {
        if (element == 0) {
            count_zeros++;
        }
    }
    return count_zeros;
}

// Function to find the position of an element in a vector
int preimg(const std::vector<int>& L, int xi) {
    for (int i = 0; i < L.size(); i++) {
        if (L[i] == xi) {
            return i;
        }
    }
    return -1; // Return -1 if the element is not found
}

// Custom hash function for vectors
struct VectorHash {
    size_t operator()(const std::vector<int>& vec) const {
        size_t hash = 0;
        for (int i : vec) {
            hash ^= std::hash<int>()(i);
        }
        return hash;
    }
};

// Custom equality function for vectors
struct VectorEqual {
    bool operator()(const std::vector<int>& lhs, const std::vector<int>& rhs) const {
        return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }
};

// Function to count the occurrences of vectors in a vector of vectors
std::unordered_map<std::vector<int>, int, VectorHash, VectorEqual> count_member(const std::vector<std::vector<int> >& itr){
    std::unordered_map<std::vector<int>, int, VectorHash, VectorEqual> d;
    for (const auto& vec : itr) {
        d[vec]++;
    }
    return d;
}
std::vector<int> flip_signature(const FeynmanGraph& G, const std::vector<int>& p, const std::vector<int>& a) {
    std::vector<Edge> ee = G.edges();
    std::vector<int> b(a.size(), 0);

    for (size_t i = 0; i < a.size(); ++i) {
        int ai = a[i];
        Edge ev = ee[i];

        if (ai == 0 && ev.src != ev.dst) {
            if (preimg(p, ev.src) < preimg(p, ev.dst)) {
                b[i] = -1;
            } else {
                b[i] = 0;
            }
        } else if (ev.src == ev.dst) {
            b[i] = -2;
        } else if (ai != 0 && ev.src != ev.dst) {
            b[i] = ai;
        }
    }

    return b;
}
//define factorial of an integer.
int factorial(int n) {
    if (n <= 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

//print vector function
void printVector(const std::vector<int> &vec)
{
    for (int value : vec){
        if (value==*vec.begin()){
            std::cout<<"  "<< value;
        }
        else{
            std::cout << ", "<<value ;
        }
    }
    std::cout << std::endl;
}

std::vector<std::vector<int>> permutation(std::vector<int>& vec) {
    // Sort the vector to ensure permutations are unique
    std::sort(vec.begin(), vec.end());

    // Vector to store permutations
    std::vector<std::vector<int>> per;

    // Collect permutations and push them into the per vector
    do {
        per.push_back(vec);
    } while (std::next_permutation(vec.begin(), vec.end()));

    return per; // Return the vector of permutations
}

/*
//define signature_and_multiplicities.
std::vector<std::tuple<int, std::vector<int>>> signature_and_multiplicities(const FeynmanGraph& G, const std::vector<int>& a) {
    std::vector<Edge> ee = G.edges();
    std::vector<int> p;
    std::vector<std::tuple<int, std::vector<int>>> b;
    std::vector<int> l(G.nv(), 0);
    std::vector<std::vector<int>> y;

    if (count_zero(a) <= 1) {
        b.push_back(std::make_tuple(factorial(G.nv()), a));
        return b;
    } else {
        for (size_t i = 0; i < ee.size(); i++) {
            int ai = a[i];
            Edge ev = ee[i];
            if (ai == 0 && ev.src != ev.dst) {
                l[ev.src] = 1;
                l[ev.dst] = 1;
            }
        }

        for (size_t i = 0; i < l.size(); i++) {
            int li = l[i];
            if (li == 1) {
                p.push_back(i);
            }
        }
    
        std::vector<std::vector<int>> per = permutation(p);
        int dd = factorial(G.nv()) / p.size();
        for (const auto& ga : per) {
            y.push_back(flip_signature(G, ga, a));
        }

        std::unordered_map<std::vector<int>, int, VectorHash, VectorEqual> py = count_member(y);
        for (auto it = py.begin(); it != py.end(); ++it) {
            const std::vector<int>& key = it->first;
            int val = it->second;
            b.push_back(std::make_tuple(dd * val, key));
        }

        if (b.size() == 1) {
            return b;
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
                    auto it2 = std::find(group.begin(), group.end(), std::make_tuple(2 * m, values1));

                    if (it1 != group.end() || it2 != group.end()) {
                        continue;
                    }

                    if (n == m && values1 == replaces(values2)) {
                        equiv = true;
                        break;
                    }
                }

                if (equiv) {
                    group.push_back(std::make_tuple(mm, values1));
                }
            }

            return group;
        }
    }
}
*/


std::vector<std::tuple<int, std::vector<int>>> signature_and_multiplicitie(const FeynmanGraph& G, const std::vector<int>& a) {
    std::vector<Edge> ee = G.edges();
    std::vector<int> p;
    std::vector<std::tuple<int, std::vector<int>>> b;
    std::vector<int> l(G.nv(), 0);
    std::vector<std::vector<int> > y;

    if (count_zero(a) <= 1) {
        b.push_back(std::make_tuple(factorial(G.nv()), a));
        return b;
    } else {
        for (size_t i = 0; i < ee.size(); i++) {
            int ai = a[i];
            Edge ev = ee[i];
            if (ai == 0 && ev.src != ev.dst) {
                l[ev.src-1] = 1;
                l[ev.dst-1] = 1;
                //std::cout<<" src "<<ev.src<<" dst "<<ev.dst<<std::endl;
            }
        }/*
        std::cout << "Vector l: ";
        for (int val : l) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        */
        for (size_t i = 0; i < l.size(); i++) {
            int li = l[i];
            if (li == 1) {
                p.push_back(i+1);
            }
        }
       /* std::cout << "Vector p: ";
        for (int val : p) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        */
    std::vector<std::vector<int>> per = permutation(p);
     /*
    for (const std::vector<int>& elt : per) {
        for (int value : elt) {
            if (value == *elt.begin()) {
                std::cout << " " << value;
            } else {
                std::cout << ", " << value;
            }
        }
            std::cout << std::endl;

    }*/

    std::cout << std::endl;
        int dd = factorial(G.nv()) / per.size();
       // std::cout <<" size of p "<< per.size()<< std::endl;

        for (const auto& ga : permutation(p)) {
            y.push_back(flip_signature(G, ga, a));
    }
        
        // Print the vector y
        /*
        std::cout << "Vector y: "<<std::endl;
        for (const std::vector<int>& val : y) {
            for (int v : val) {
                std::cout << v << " " ;
            }
            std::cout << std::endl;
        }
        */

        std::unordered_map<std::vector<int>, int, VectorHash, VectorEqual> py = count_member(y);
        for (auto it = py.begin(); it != py.end(); ++it) {
            const std::vector<int>& key = it->first;
            int val = it->second;
            b.push_back(std::make_tuple(dd * val, key));
        }
    }
    return b;
}
#include <vector>

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
void CartesianRecurse(std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>> &accum,
                      std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> stack,
                      const std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>> &sequences,
                      int index) {
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> sequence = sequences[index];
    
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

std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>> CartesianProduct(
    const std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>> &sequences) {
    
    std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>> accum;
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> stack;
    
    if (!sequences.empty())
        CartesianRecurse(accum, stack, sequences, sequences.size() - 1);
    
    return accum;
}
using Element = std::pair<std::pair<int, int>, std::pair<int, int>>;
using Sequence = std::vector<Element>;

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

double sum_absolute_products(std::vector<Sequence> tt) {
    double total_abs_product = 0;
    for (auto &t : tt) {
        double abs_product = 1.0;
        for (auto &ui : t) {
            abs_product *= abs(ui.first.second );
        }
        total_abs_product += abs_product;
    }
    return total_abs_product;
}

/* 
double feynman_integral(const FeynmanGraph& G, const std::vector<int>& a) {
    std::vector<Edge> ee = G.edges();
    int N = accumulate(a.begin(), a.end(), 0);
    std::vector<std::tuple<int, std::vector<int>>> f = signature_and_multiplicities(G, a);
    std::vector<int> fey;

    for (size_t i = 0; i < f.size(); i++) {
        std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>> tmp;
        for (size_t j = 0; j < std::get<1>(f[i]).size(); j++) {
            if (std::get<1>(f[i])[j] == -1) {
                tmp.push_back(cons(G, j, N));
            } else if (std::get<1>(f[i])[j] == 0) {
                tmp.push_back(cons0(G, j, N));
            } else {
                tmp.push_back(prot(G, j, std::get<1>(f[i])[j], N));
            }
        }
        fey.push_back(sum_absolute_products(mergetuple(tmp)));
        return tmp;
    }

    // Here you can do something with the fey vector if needed
    // For example, you can calculate the sum of its elements and return it
    //return  //accumulate(fey.begin(), fey.end(), 0.0); // Assuming fey is a vector of integers
}*/



/*double feynman_integral_b(FeynmanGraph& G, const std::vector<int>& a, const std::vector<int>& l = std::vector<int>()) {
    std::vector<Edge> ee = G.edges(); 
    int N = std::accumulate(a.begin(), a.end(), 0);
    std::vector<std::tuple<int, std::vector<int>>>  f = signature_and_multiplicities(G, a);
    std::vector<double> fey;
    for (size_t i = 0; i < f.size(); ++i) {
        std::vector<Sequence> tmp;
        for (size_t j = 0; j < f[i].second.size(); ++j) {
            if (f[i].second[j] == -1) {
                tmp.push_back(cons(F, j, N));
            } else if (f[i].second[j] == 0) {
                tmp.push_back(cons0(F, j, N));
            } else {
                tmp.push_back(prot(F, j, f[i].second[j], N));
            }
            for (const auto& element : sequence) {
        std::cout << "(" << element.first.first << ", " << element.first.second << "), (" 
                  << element.second.first << ", " << element.second.second << ")" << std::endl;
    }
        }
        std::vector<Sequence> tt = mergetuple(tmp);
        double ty = sum_absolute_products(tt);
        fey.push_back(f[i].first * ty);
    }
    return std::accumulate(fey.begin(), fey.end(), 0.0);
}
*/


double feynman_integral(FeynmanGraph& G, const std::vector<int>& a, const std::vector<int>& l = {}) {
    std::vector<Edge> ee = G.edges(); 
    int N = std::accumulate(a.begin(), a.end(), 0);
    std::vector<std::tuple<int, std::vector<int>>> f = signature_and_multiplicitie(G, a);
    std::vector<double> fey;
    std::cout << "signature_multiplicities is " << std::endl;
    for (const auto& tuple : f) {
        std::cout << std::get<0>(tuple) << ", ";
        for (const auto& val : std::get<1>(tuple)) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    for (const auto& item : f) {
        int factor = std::get<0>(item);
        const auto& multiplicities = std::get<1>(item);

        std::vector<Sequence> tmp;
        for (size_t j = 0; j < a.size(); j++) {
            const auto& multiplicity = multiplicities[j]; 
           // std::cout << "multiplicity "<< multiplicity<<"  j: "<<j<<std::endl;

            if (multiplicity == -1) {
                tmp.push_back(cons(G, j , N));

               /* Sequence result_cons=cons(G, j , N);
                std::cout << "cons result -1:" << std::endl;
                for (const auto& item : result_cons) {  
                std::cout << "(" << item.first.first << ", " << item.first.second << "), (" 
                        << item.second.first << ", " << item.second.second << ")" << std::endl;
                }
                std::cout << "cons result:" << std::endl;
                */

            } else if (multiplicity == 0) {
                tmp.push_back(cons0(G, j , N));

              /*  Sequence result_cons=cons0(G, j , N);
                std::cout << "cons result 0 :" << std::endl;
                for (const auto& item : result_cons) {  
                std::cout << "(" << item.first.first << ", " << item.first.second << "), (" 
                        << item.second.first << ", " << item.second.second << ")" << std::endl;
                }
                std::cout << " " << std::endl;
                */

            } else {
                tmp.push_back(prot(G, j , multiplicity, N));
            }
              

        }
                  std::cout << "tmp is   "<< std::endl;

         for (const auto& vec : tmp) {
                for (const auto& tuple : vec) {
                    std::cout << "(( " << tuple.first.first << ", " << tuple.first.second << " ), ( "
                    << tuple.second.first << ", " << tuple.second.second << ")), ";
                }
                std::cout << std::endl;
            }

          std::cout << "mergetuple is   "<< std::endl;

            std::vector<Sequence> tt = mergetuple(tmp);
            for (const auto& vec : tt) {
                for (const auto& tuple : vec) {
                    std::cout << "(( " << tuple.first.first << ", " << tuple.first.second << " ), ( "
                            << tuple.second.first << ", " << tuple.second.second << ")), ";
                }
                std::cout << std::endl;
            }
            double ty = sum_absolute_products(tt);

        //std::cout << "mergetuple "<< tmp.size()<<std::endl;


                // Output the result
          //  std::cout << "mergetuple "<< tt.size()<<std::endl;
           /* for (const auto& vec : tt) {
                for (const auto& tuple : vec) {
                    std::cout << "(( " << tuple.first.first << ", " << tuple.first.second << " ), ( "
                            << tuple.second.first << ", " << tuple.second.second << ")), ";
                }
                std::cout << std::endl;
            }*/


                //double ty = sum_absolute_products(tt);
                fey.push_back(factor * ty);
            }

    return std::accumulate(fey.begin(), fey.end(), 0.0);
}


int main() {
    std::vector<std::pair<int, int>> edges = {{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}};
    FeynmanGraph graph(edges);
    std::vector<int> aa = {0, 0, 2, 2, 2, 2};
    

    /* std::cout << "signature_multiplicities is " << std::endl;
    std::vector<std::tuple<int, std::vector<int>>> ss = signature_and_multiplicitie(graph, aa);
    for (const auto& tuple : ss) {
        std::cout << std::get<0>(tuple) << ", ";
        for (const auto& val : std::get<1>(tuple)) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    signature_and_multiplicitie(graph, aa);
   
    Define the input vectors
     std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>> uu={
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
   std::vector<Sequence> fe= feynman_integral(graph,aa);
std::cout << "vector uu is: " << std::endl;
    for (const auto& u : fe) {
        std::cout << "{";
        for (auto it = u.begin(); it != u.end(); ++it) {
            std::cout << "((" << it->first.first << ", " << it->first.second << "), (" 
                      << it->second.first << ", " << it->second.second << "))";
            if (std::next(it) != u.end()) {
                std::cout << ", ";
            }
        }
        std::cout << "}" << std::endl;
    }
*/
std::cout << "Feynman Integral is : \n " << feynman_integral(graph,aa) << std::endl;

    return 0;
}