#include <unordered_map>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <numeric>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <functional>
#include <cmath>
#include <chrono>


// Function to replace -1 with 0 and 0 with -1 in a vector
std::vector<int> replaces(const std::vector<int>& arr) {
    std::vector<int> result_arr;
    for (int x : arr) {
        result_arr.push_back(x == -1 ? 0 : (x == 0 ? -1 : x));
    }
    return result_arr;
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

std::vector<int> flip_signature(const std::vector<std::pair<int, int>>& G, const std::vector<int>& p, const std::vector<int>& a) {
    std::vector<int> b(a.size(), 0);

    for (size_t i = 0; i < a.size(); ++i) {
        int ai = a[i];
        std::pair<int, int> ev = G[i];

        if (ai == 0 && ev.first != ev.second) {
            int ii = -1;
            int ij = -1;
            for (size_t j = 0; j < p.size(); ++j) {
                if (p[j] == ev.first) {
                    ii = j;
                }
                if (p[j] == ev.second) {
                    ij = j;
                }
            }
                
            if (ii != -1 && ij != -1) {
                if (ii < ij) {
                    b[i] = -1;
                } else {
                    b[i] = 0;
                }
            }
        } else if (ev.first == ev.second) {
            b[i] = -2;
        } else if (ai != 0 && ev.first != ev.second) {
            b[i] = ai;
        }
    }

    return b;
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

// Define the hash functor
struct VectorHash {
    size_t operator()(const std::vector<int>& vec) const {
        size_t hash = 0;
        for (int i : vec) {
            hash ^= std::hash<int>()(i);
        }
        return hash;
    }
};

// Define the equality functor
struct VectorEqual {
    bool operator()(const std::vector<int>& lhs, const std::vector<int>& rhs) const {
        return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }
};

// Define the function count_member
std::unordered_map<std::vector<int>, int, VectorHash, VectorEqual> count_member(const std::vector<std::vector<int>>& itr) {
    std::unordered_map<std::vector<int>, int, VectorHash, VectorEqual> d;
    for (const auto& vec : itr) {
        d[vec]++;
    }
    return d;
}

std::vector<std::tuple<int, std::vector<int>>> signature_and_multiplicitie(const std::vector<std::pair<int, int>>& G, const std::vector<int>& a) {
    std::vector<int> p;
    std::vector<std::tuple<int, std::vector<int>>> b;
    std::unordered_set<int> nbv;
    for (const auto& e : G) {
        nbv.insert(e.first);
        nbv.insert(e.second);
    }
     int nv=nbv.size();
    std::vector<int> l(nv, 0);
    std::vector<std::vector<int> > y;
    int count_zeros = 0;
    for (int element : a) {
        if (element == 0) {
            count_zeros++;
        }
    }
    int fact = 1;
    for(int i = 1; i <= nv; i++) {
        fact *= i;
    }
    if (count_zeros <= 1) {
        b.push_back(std::make_tuple(fact, a));
        return b;
    } else {
        for (size_t i = 0; i < G.size(); i++) {
            int ai = a[i];
            std::pair<int, int>ev = G[i];
            if (ai == 0 && ev.first != ev.second) {
                l[ev.first-1] = 1;
                l[ev.second-1] = 1;
                //std::cout<<" first "<<ev.first<<" second "<<ev.second<<std::endl;
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
       // Sort the vector to ensure permutations are unique
    std::sort(p.begin(), p.end());

    // Vector to store permutations
    std::vector<std::vector<int>> per;

    // Collect permutations and push them into the per vector
    do {
        per.push_back(p);
    } while (std::next_permutation(p.begin(), p.end()));

     // Return the vector of permutations
       // std::vector<std::vector<int>> per = permutation(p);
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
            int dd = fact / per.size();
        // std::cout <<" size of p "<< per.size()<< std::endl;

            for (const auto& ga : per) {
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

                    if (n == m && values1 == replaces(values2)) {
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
}
/*
int main() {
    std::vector<std::pair<int, int>> edges = {{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}};
    std::vector<std::pair<int, int>> graph(edges);
    int n=graph.size();
    std::vector<int> aa = {0, 0, 0, 2, 2, 2};
    
    std::cout << "signature_multiplicities is " << std::endl;
    std::vector<std::tuple<int, std::vector<int>>> ss = signature_and_multiplicitie(graph, aa);
    for (const auto& tuple : ss) {
        std::cout << std::get<0>(tuple) << ", ";
        for (const auto& val : std::get<1>(tuple)) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
std::vector<int> ab = {1, 0, 1, 2, 0, 0};
    std::vector<int> ga = {4, 2, 3, 1};

    std::vector<int> tmp=flip_signature(graph,ga,ab);
    std::cout << "flip l: ";
        for (int val : tmp) {
            std::cout << val << " ";
        }
        std::cout << std::endl;

    return 0;
}
*/