#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include "feynman.hpp"
#include <functional>
#include <cmath>
#include <numeric>
#include <chrono>

int main() {
    //std::vector<std::pair<int, int>> edges = {{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}};
    std::vector<std::pair<int, int>> edges = {{1, 2}, {1, 2}, {1, 2}};
    FeynmanGraph graph(edges);
    int n=graph.ne();
    std::vector<int> aa = {0, 2, 2};
    int d = std::accumulate(aa.begin(), aa.end(), 0);


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

 //std::cout << "feynman is"<<feynman_integral(graph,aa) << std::endl;
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

    int d=3;
    std::vector<double> fey=feynman_integral_degree(graph,d);
    std::cout<<"[";
    for(double fe:fey){
        std::cout<<""<<fe<<" ";
    }
    std::cout << "]" ;
        std::cout<<std::endl;

    double su=0;
    for (double u:fey){
        su+=u;
    }
    std::cout<<su<<std::endl;*/
    
    std::cout << " sum_degree : "<<std::endl;
    std::vector<std::vector<int> > sum_deg=sum_degree(n,d);
    std::string v =vectorToString(sum_deg);
    std::cout << vectorToString(sum_deg) << std::endl;

    std::vector<std::vector<std::vector<int>> > ge;
    std::vector<std::vector<int>> ru = stringToVector(v);
              for( std::vector<int> x:ru){
                ge.push_back(generate_combination(x));
              }

    std::string gg = vectorToStringv(ge);
    std::cout << "String representation of ge:\n" << gg << std::endl;
        std::cout <<  std::endl;

        std::cout << " feynamn : "<<std::endl;
        std::vector<std::vector<std::vector<int>>> aj=stringToVectorv(gg);
        std::vector<std::vector<int>> ag=flattenVector(aj);

        std::vector<double> fey;
        for( std::vector<int> v:ag){
            fey.push_back(feynman_integral(graph,v));
        }
        std::cout<<"[";
            for(double fe:fey){
                std::cout<<""<<fe<<" ";
            }
            std::cout << "]" ;
                std::cout<<std::endl;
    return 0;
}