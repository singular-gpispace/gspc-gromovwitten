#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include "feynman.hpp"

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
template<typename T>
std::vector<std::vector<T>> cartesian_product2(const std::vector<std::vector<std::pair<T, T>>>& uu) {
    std::vector<std::vector<T>> res;

    if (uu.empty()) {
        return res; // Return an empty vector if input is empty
    }

    // Calculate the total number of combinations
    size_t total_combinations = 1;
    for (const auto& vec : uu) {
        total_combinations *= vec.size();
    }

    // Initialize the result vector with the correct size
    res.resize(total_combinations);

    // Fill the result vector with Cartesian product combinations
    for (size_t i = 0; i < total_combinations; ++i) {
        std::vector<T> combination;
        size_t index = i;
        for (const auto& vec : uu) {
            combination.push_back(vec[index % vec.size()].first);
            index /= vec.size();
        }
        res[i] = combination;
    }

    return res;
}





int main() {
    std::vector<std::pair<int, int>> edges = {{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}};
    FeynmanGraph graph(edges);
    std::vector<int> aa = {0, 2, 1, 0, 0, 1};

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
    std::vector<std::vector<std::pair<int, int>>> uu = {
        {{{1, 1}, {3, -1}}, {{1, 2}, {3, -2}}, {{1, 3}, {3, -3}}, {{1, 4}, {3, -4}}},
        {{{1, 1}, {2, -1}}, {{1, -1}, {2, 1}}},
        {{{1, 1}, {2, -1}}, {{1, -1}, {2, 1}}, {{1, 3}, {2, -3}}, {{1, -3}, {2, 3}}},
        {{{2, -1}, {4, 1}}, {{2, -2}, {4, 2}}, {{2, -3}, {4, 3}}, {{2, -4}, {4, 4}}},
        {{{3, 1}, {4, -1}}, {{3, 2}, {4, -2}}, {{3, 3}, {4, -3}}, {{3, 4}, {4, -4}}},
        {{{3, 1}, {4, -1}}, {{3, 2}, {4, -2}}, {{3, 3}, {4, -3}}, {{3, 4}, {4, -4}}}
    };

    auto result = cartesian_product2(uu);

    // Print the result
    for (const auto& vec : result) {
        for (const auto& pair : vec) {
            std::cout << "(" << pair.first << ", " << pair.second << ") ";
        }
        std::cout << std::endl;
    }
    return 0;
}