#include <iostream>
#include <vector>
#include <tuple>
#include "feynman.hpp"

using ntuple =std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>;
std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> cons0(const FeynmanIntegral& F, int j, int N) {
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> v;
    std::vector<Edge> ee = F.getGraph().getEdges();
    for (int i = 1; i <= N; ++i) {
        if (ee[j].src < ee[j].dst)
            v.push_back(std::make_pair(std::make_pair(ee[j].src, -i), std::make_pair(ee[j].dst, +i)));
        else
            v.push_back(std::make_pair(std::make_pair(ee[j].dst, +i), std::make_pair(ee[j].src, -i)));
    }
    return v;
}


std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> cons(const FeynmanIntegral& F, int j, int N) {
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> v;
    std::vector<Edge> ee = F.getGraph().getEdges();
    for (int i = 1; i <= N; ++i) {
        if (ee[j].dst < ee[j].src)
            v.push_back(std::make_pair(std::make_pair(ee[j].dst, -i), std::make_pair(ee[j].src, +i)));
        else
            v.push_back(std::make_pair(std::make_pair(ee[j].src, +i), std::make_pair(ee[j].dst, -i)));
    }
    return v;
}

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> prot(const FeynmanIntegral& F, int j, int a, int N) {
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> x_powers;
    std::vector<Edge> ee = F.getGraph().getEdges();

    for (int w = 1; w <= a; ++w) {
        if (a % w == 0) {
            x_powers.push_back(std::make_pair(std::make_pair(ee[j].src, +w), std::make_pair(ee[j].dst, -w)));
            x_powers.push_back(std::make_pair(std::make_pair(ee[j].src, -w), std::make_pair(ee[j].dst, +w)));
        }
    }
    return x_powers;
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


int main() {
    std::vector<std::pair<int, int>> edges = {{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}};
     FeynmanGraph graph(edges);
    FeynmanIntegral integral(graph);

    int j = 1;
    int N = 4;

    auto result_cons0 = cons0(integral, 0, N);
    auto result_cons = cons(integral, 1, N);
    auto result_prot = prot(integral,3, 1, N);
    
    std::vector<ntuple> uu;
    uu.push_back(result_cons0);
    uu.push_back(result_cons);
    uu.push_back(result_prot);
    std::cout << "vector uu is: " << std::endl;
    for (const auto& u : uu) {
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
       std::cout << "vector uu[1] is : " << std::endl;
    /*std::vector<std::vector<std::tuple<std::pair<int, int>, std::pair<int, int>>>> uv = {
        {{{{1, 1}, {3, -1}}, {{1, 2}, {3, -2}}, {{1, 3}, {3, -3}}, {{1, 4}, {3, -4}}}},
        {{{{1, 1}, {2, -1}}, {{1, -1}, {2, 1}}}},
         {{{2, 1}, {3, -1}}, {{2, -1}, {3, 1}}}
    };*/

    std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>> res = CartesianProduct(uu);

    for (const auto& vec : res) {
        for (const auto& tuple : vec) {
            std::cout << "(( " << std::get<0>(tuple).first << ", " << std::get<0>(tuple).second << " ), ( "
                      << std::get<1>(tuple).first << ", " << std::get<1>(tuple).second << " )) ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}
