#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include <algorithm>
#include <cmath>
#include "feynman.hpp"
#include <numeric>
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



int main() {
    std::vector<Sequence> uu={
 {{{1, 1}, {3, -1}}, {{1, 2}, {3, -2}}, {{1, 3}, {3, -3}}, {{1, 4}, {3, -4}}},
 {{{1, 1}, {2, -1}}, {{1, -1}, {2, 1}}},
 {{{1, 1}, {2, -1}}, {{1, -1}, {2, 1}}, {{1, 3}, {2, -3}}, {{1, -3}, {2, 3}}},
 {{{2, -1}, {4, 1}}, {{2, -2}, {4, 2}}, {{2, -3}, {4, 3}}, {{2, -4}, {4, 4}}},
 {{{3, 1}, {4, -1}}, {{3, 2}, {4, -2}}, {{3, 3}, {4, -3}}, {{3, 4}, {4, -4}}},
 {{{3, 1}, {4, -1}}, {{3, 2}, {4, -2}}, {{3, 3}, {4, -3}}, {{3, 4}, {4, -4}}}
};
 std::vector<Sequence> uv={
 {{{ 1, -1 }, { 2, 1}}}, {{{ 1, -2 }, { 2, 2}}}, {{{ 1, -3 }, { 2, 3}}}, {{{ 1, -4 }, { 2, 4}}}, {{{ 1, -5 }, { 2, 5}}}, {{{ 1, -6 }, { 2, 6}}}, {{{ 1, -7 }, { 2, 7}}}, {{{ 1, -8 }, { 2, 8}}}, {{{ 1, -9 }, { 2, 9}}}, {{{ 1, -10 }, { 2, 10}}}, 
{{{ 1, 1 }, { 2, -1}}}, {{{ 1, -1 }, { 2, 1}}}, {{{ 1, 2 }, { 2, -2}}}, {{{ 1, -2 }, { 2, 2}}}, 
{{{ 2, 1 }, { 4, -1}}}, {{{ 2, -1 }, { 4, 1}}}, {{{ 2, 2 }, { 4, -2}}}, {{{ 2, -2 }, { 4, 2}}}, 
{{{ 3, 1 }, { 4, -1}}}, {{{ 3, -1 }, { 4, 1}}}, {{{ 3, 2 }, { 4, -2}}}, {{{ 3, -2 }, { 4, 2}}}, 
{{{ 3, 1 }, { 4, -1}}}, {{{ 3, -1 }, { 4, 1}}}, {{{ 3, 2 }, { 4, -2}}}, {{{ 3, -2 }, { 4, 2}}}, 
{{{ 0, 1 }, { 2, -1}}}, {{{ 0, -1 }, { 2, 1}}}, {{{ 0, 2 }, { 2, -2}}}, {{{ 0, -2 }, { 2, 2}}}, 
};
    std::vector<Sequence> res = mergetuple(uv);

    // Output the result
    for (const auto& vec : res) {
        for (const auto& tuple : vec) {
            std::cout << "(( " << tuple.first.first << ", " << tuple.first.second << " ), ( "
                      << tuple.second.first << ", " << tuple.second.second << ")), ";
        }
        std::cout << std::endl;
    }

std::cout << "Sum of absolute products: " << sum_absolute_products(res) << std::endl;
    return 0;
}
