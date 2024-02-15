#include <iostream>
#include <vector>
#include <tuple>
#include "feynman.hpp"

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

// Main function for testing
int main() {
    std::vector<std::pair<int, int>> edges = {{1, 2}, {2, 3}, {3, 4}};
    FeynmanGraph graph(edges);
    FeynmanIntegral integral(graph);

    int j = 1;
    int N = 4;

    auto result_cons0 = cons0(integral, j, N);
    std::cout << "cons0 result:" << std::endl;
    for (const auto& item : result_cons0) {
        std::cout << "(" << item.first.first << ", " << item.first.second << "), (" 
                  << item.second.first << ", " << item.second.second << ")" << std::endl;
    }

    auto result_cons = cons(integral, j, N);
    std::cout << "cons result:" << std::endl;
    for (const auto& item : result_cons) {
        std::cout << "(" << item.first.first << ", " << item.first.second << "), (" 
                  << item.second.first << ", " << item.second.second << ")" << std::endl;
    }

    int a = 3;
    auto result_prot = prot(integral, j, a, N);
    std::cout << "prot result:" << std::endl;
    for (const auto& item : result_prot) {
        std::cout << "(" << item.first.first << ", " << item.first.second << "), (" 
                  << item.second.first << ", " << item.second.second << ")" << std::endl;
    }

    return 0;
}
