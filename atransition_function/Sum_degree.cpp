#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>
//#include "../include/feynman/feynman.hpp"
#include <functional>
#include <cmath>
#include <numeric>
#include <chrono>
#include <sstream>
#include <unordered_set>
#include <stack>

int main() {

using Element = std::pair<std::pair<int, int>, std::pair<int, int>>;
using Sequence = std::vector<Element>;

    // Example usage:
    int n = 3;
    int d = 4;
     std::vector<std::vector<int>> ru; 
   
    for (int e = 0; e < d; e++) {
        std::vector<int> x(n, 0);
        x[0] = d - e;
        x[n - 1] = e;
        ru.push_back(x);
    }

   std::stringstream ss1;
    for (const auto& innerVec : ru) {
        for (int val : innerVec) {
            ss1 << val << ' ';
        }
        ss1 << '\n'; 
    }
    // ss1.str();
    std::string d_vectors=ss1.str();
    std::cout <<" sum_degree(d,n)" <<std::endl;
    std::cout << d_vectors <<std::endl;
    
    std::string v=d_vectors;             
    std::vector<std::vector<int>> result2;
    std::stringstream ss2(v);
    std::string line;
    while (std::getline(ss2, line)) {
        std::vector<int> innerVec;
        std::stringstream lineStream(line);
        int num;
        while (lineStream >> num) {
            innerVec.push_back(num);
        }
        result2.push_back(innerVec);
    }
    /* for (const auto& partition : result2) {
        for (int val : partition) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
            std::cout << std::endl;
*/
    std::vector<std::vector<int>> xx=result2;
    std::vector<std::vector<std::vector<int>> > ge;
    for( std::vector<int> x:xx){
        int k=x.size();
        if (k == 0) {
            throw std::invalid_argument("k should be nonzero");
        }
        std::vector<std::vector<int> > ru;
        int d= std::accumulate(x.begin(),x.end(),0);
        std::vector<int> v(k , 0);
        v[0] = d;
        if (v==x){
        ru.push_back(v);
        }
        int n = 1;
        int nn=d + k - 1;
        int k1=d;
        for (int i = 1; i <= k1; ++i) {
            n *= (nn - i + 1);
            n /= i;
        }        int e = d - x[0];
        std::vector<int> a = x;    
        std::vector<int> y(k, 0); 
        y[0] = x[0] - 1;
        y[k - 1] = e + 1;
        
        for (int i = 0; i < n; ++i) {
            if (a != y) {
                //a = next_partition(a);
                for (int i = k - 1; i >= 0; --i) {
                    if (i == k - 1 && a[i] == d) {
                        //ru.push_back(a);
                        break;
                    } else {
                        for (int j = i - 1; j >= 0; --j) {
                            if (a[j] != 0) {
                                --a[j];
                                int ak = a[k - 1];
                                a[k - 1] = 0;
                                a[j + 1] = ak + 1;
                            // ru.push_back(a);
                                break;                       
                                }
                        }
                    }
                    //ru.push_back(a);
                    break;
                }
                ru.push_back(a);
            } else {
                break;
            }
        }
        ge.push_back(ru);      
    }
    std::stringstream ss3;
    for (const auto& innerVec1 : ge) {
        for (const auto& innerVec2 : innerVec1) {
            for (int val : innerVec2) {
                ss3 << val << ' ';
            }
            ss3 << '\n'; 
        }
        ss3 << '\n'; 
    }
       
    std::string vec=ss3.str();
    std::cout <<" combination(v)" <<std::endl;
    std::cout << ss3.str() <<std::endl;
    std::string branchtype=vec;
    std::string a=branchtype;
    // std::vector<std::vector<std::vector<int>>> aj=stringToVectorv(a);
    std::vector<std::vector<std::vector<int>>> aj;
    std::stringstream ssv(a);
    std::string line1;

    std::vector<std::vector<int>> innerVec1;
    while (std::getline(ssv, line1)) {
        if (line1.empty()) {
            if (!innerVec1.empty()) {
                aj.push_back(innerVec1);
                innerVec1.clear();
            }
        } 
        else {
            std::vector<int> innerVec2;
            std::stringstream lineStream(line1);
            int num;
            while (lineStream >> num) {
                innerVec2.push_back(num);
            }
            innerVec1.push_back(innerVec2);
        }
    }
    if (!innerVec1.empty()) {
        aj.push_back(innerVec1);
    }

  std::vector<std::vector<int>> ag;
    for (const auto& innerVec1 : aj) {
        for (const auto& innerVec2 : innerVec1) {
            ag.push_back(innerVec2);
        }
    }

    std::string graph= "{{1, 2}, {1, 2}, {1, 2}}";
    std::string G = graph;
   // std::cout << "graph: " << G << std::endl;
/*
    // Remove double quotes at the beginning and end
    if (!G.empty() && G.front() == '"' && G.back() == '"') {
        G = G.substr(1, G.size() - 2);
    }

    // Vector to store parsed values
    std::vector<std::vector<int>> Gvec;

    // Loop to parse the string
    std::vector<int> innerVec;
    std::string numString;
    for (char ch : G) {
        if (isdigit(ch)) {
            numString += ch;
        } else if (ch == ',' || ch == ' ') {
            if (!numString.empty()) {
                innerVec.push_back(std::stoi(numString));
                numString.clear();
            }
        } else if (ch == '}') {
            if (!numString.empty()) {
                innerVec.push_back(std::stoi(numString));
                numString.clear();
            }
            if (!innerVec.empty()) {
                Gvec.push_back(innerVec);
                innerVec.clear();
            }
        }
    }
*/

 // Remove double quotes end and begin
    if (!G.empty() && G.front() == '"' && G.back() == '"') {
        G = G.substr(1, G.size() - 2);
    }

    // store
    std::vector<std::vector<int>> Gvec;

    // Loop string
    std::vector<int> innerVec;
    std::string numString;
    bool inBraces = false; // Flag 
    for (char ch : G) {
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
        // Check if we have a comma or space
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

    // Output the vector of pairs
   /* std::cout << "Vector of pairs:" << std::endl;
    for (const auto& pair : Gv) {
        std::cout << "{" << pair.first << ", " << pair.second << "} ";
    }
    std::cout << std::endl;
   */

    std::unordered_set<int> nbv;
          std::vector<int> fey;
          for (const auto& e : Gv) {
        nbv.insert(e.first);
        nbv.insert(e.second);
    }
        std::vector<int> fey_d;

    int nv = nbv.size(); //nb vertices.
    //std::cout<<" nv = "<<nv<<std::endl;



    for( std::vector<int> av:ag){
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
                }
                else {
                    j++;
                }
            }
            if (!comp) {
                b.push_back(std::make_tuple(1, tmp));
            }
        }
        std::vector<std::tuple<int, std::vector<int>>> f;

        for (auto& tuple : b) {
        int& element = std::get<0>(tuple); 
        // std::cout<<" element "<<element<<std::endl;
        element *= fact / per.size();
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
        //std::cout<<"myfey "<< myfey<<std::endl;

        fey_d.push_back(myfey);
          }
          int su=0;
          for (int s:fey_d  ){
            su+=s;
          }
          std::cout<< su;
        
    return 0;
}

