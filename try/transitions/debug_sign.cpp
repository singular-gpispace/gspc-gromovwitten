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
#include <iterator>

int compareL(const std::vector<int>& a, const std::vector<int>& b) {
    if (a.size() != b.size()) 
        return 0;

    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}



std::vector<int> flip_signature(const std::vector<std::pair<int, int>>& G, const std::vector<int>& pf, const std::vector<int>& af) {
    std::vector<int> flip(af.size(), 0);

    for (size_t i = 0; i < af.size(); ++i) {
        int ai = af[i];
        std::pair<int, int> ev = G[i];

        if (ai == 0 && ev.first != ev.second) {
            int ii = -1;
            int ij = -1;
            for (size_t j = 0; j < pf.size(); ++j) {
                if (pf[j] == ev.first) {
                    ii = j;
                }
                if (pf[j] == ev.second) {
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

    return flip;
}

std::vector<int> replaces(const std::vector<int>& arr) {
    std::vector<int> result_arr;
    for (int x : arr) {
        result_arr.push_back(x == -1 ? 0 : (x == 0 ? -1 : x));
    }
    return result_arr;
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

int main() {
    std::vector<std::pair<int, int>> edges = {{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}};
    std::vector<std::pair<int, int>> graph(edges);
    int n=graph.size();
    std::vector<int> aa = {1, 0, 1, 2, 0, 0};
    std::vector<int> ga = {4, 2, 3, 1};

    std::cout << "signature_multiplicities is " << std::endl;
std::vector<std::tuple<int,std::vector<int>>> ss = signature_and_multiplicitie(graph, aa);
    for (const auto& tuple : ss) {
    std::cout << std::get<0>(tuple) << ", ";
    for (const auto& val : std::get<1>(tuple)) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

    return 0;
}
