
#include <list>
#include <tuple>
#include <sstream> // for std::istringstream
/* #include "../../code/boost/include/boost/variant.hpp"
#include "../../code/util-generic/split.hpp"
#include "../../code/we/type/value.hpp" */
#include <stdio.h>
#include <flint/fmpz_mpoly.h>
#include <stdlib.h> // for malloc and free
#include <flint/fmpz.h>
#include <fstream>
#include <sys/resource.h>
#include <iostream>
#include <chrono>
#include <functional> // Include for std::function
#include <vector>     // Include for std::vector
#include <numeric>
#include <unordered_set> // for std::unordered_set
#include <algorithm>
#include <set>
#include <map>

// #include "../flint_try/time_memory.hpp"

using Element = std::pair<std::pair<int, int>, std::pair<int, int>>;
using Sequence = std::vector<Element>;
using signature = std::vector<std::pair<int, std::vector<int>>>;
using graph = std::vector<std::pair<int, int>>;

/*
using pnet_value = pnet::type::value::value_type;
using pnet_list = std::list<pnet_value>;
using pnet_list2d = std::list<std::list<pnet_value>>; */
using vector2d = std::vector<std::vector<int>>;
using list_type = std::list<std::string>; // Define list_type as std::list<std::string>

vector2d gen_block(int n, int d)
{
    vector2d v;
    for (int e = 0; e < d; e++)
    {
        std::vector<int> x(n, 0);
        x.front() = d - e;
        x.back() = e;

        v.push_back(x);
    }
    return v;
}

std::vector<int> next_partition(std::vector<int> a)
{
    int n = std::accumulate(a.begin(), a.end(), 0);
    int k = a.size();
    for (int i = k - 1; i >= 0; --i)
    {
        if (i == k - 1 && a[i] == n)
        {
            return a;
        }
        else
        {
            for (int j = i - 1; j >= 0; --j)
            {
                if (a[j] != 0)
                {
                    --a[j];
                    int ak = a[k - 1];
                    a[k - 1] = 0;
                    a[j + 1] = ak + 1;
                    return a;
                }
            }
        }
    }
    return a;
}
vector2d iterate(const std::vector<int>& xa)
{
    vector2d gen;

    int k = xa.size();
    if (k == 0)
    {
        throw std::invalid_argument("k should be nonzero");
    }
    int d = std::accumulate(xa.begin(), xa.end(), 0);
    std::vector<int> xv(k, 0);
    xv[0] = d;
    if (xv == xa)
    {
        gen.push_back(xa);
    }
    int e = d - xa[0];
    std::vector<int> a = xa;

    while (true)
    {
        if (a[k - 1] != e + 1)
        {
            a = next_partition(a);
            gen.push_back(a);
        }
        else
        {
            break;
        }
        if (a[k - 1] == d) {
            break;
        }
    }

    return gen;
}
std::vector<std::tuple<int, std::vector<int>>> signature_and_multiplicitie(graph G, std::vector<int> a)
{
    std::vector<int> p;
    std::vector<std::tuple<int, std::vector<int>>> b;
    std::unordered_set<int> nbv;
    for (const auto& e : G)
    {
        nbv.insert(e.first);
        nbv.insert(e.second);
    }
    int nv = nbv.size();
    std::vector<int> l(nv, 0);

    for (size_t i = 0; i < G.size(); i++)
    {
        int ai = a[i];
        std::pair<int, int> ev = G[i];
        if (ai == 0 && ev.first != ev.second)
        {
            l[ev.first - 1] = 1;
            l[ev.second - 1] = 1;
        }
    }

    for (size_t i = 0; i < l.size(); i++)
    {
        int li = l[i];
        if (li == 1)
        {
            p.push_back(i + 1);
        }
    }

    std::sort(p.begin(), p.end());

    std::vector<std::vector<int>> per;
    do
    {
        per.push_back(p);
    } while (std::next_permutation(p.begin(), p.end()));

    int fact = 1;
    for (int i = 1; i <= nv; i++)
    {
        fact *= i;
    }

    for (const auto& ga : per)
    {
        std::vector<int> flip(a.size(), 0);

        for (size_t i = 0; i < a.size(); ++i)
        {
            int ai = a[i];
            std::pair<int, int> ev = G[i];

            if (ai == 0 && ev.first != ev.second)
            {
                int ii = -1;
                int ij = -1;
                for (size_t j = 0; j < ga.size(); ++j)
                {
                    if (ga[j] == ev.first)
                    {
                        ii = j;
                    }
                    if (ga[j] == ev.second)
                    {
                        ij = j;
                    }
                }

                if (ii != -1 && ij != -1)
                {
                    if (ii < ij)
                    {
                        flip[i] = -1;
                    }
                    else
                    {
                        flip[i] = 0;
                    }
                }
            }
            else if (ev.first == ev.second)
            {
                flip[i] = -2;
            }
            else if (ai != 0 && ev.first != ev.second)
            {
                flip[i] = ai;
            }
        }

        std::vector<int> tmp = flip;

        bool comp = false;
        int j = 0;
        while (!comp && j < b.size())
        {
            int tt = 1;
            std::vector<int> bb = std::get<1>(b[j]);
            if (tmp.size() != bb.size())
                tt = 0;

            for (std::size_t i = 0; i < tmp.size(); ++i)
            {
                if (tmp[i] != bb[i])
                    tt = 0;
            }

            if (tt)
            { // Accessing the second element of the tuple
                comp = true;
                std::get<0>(b[j]) += 1;
            }
            else
            {
                j++;
            }
        }
        if (!comp)
        {
            b.push_back(std::make_tuple(1, tmp));
        }
    }
    for (auto& tuple : b)
    {
        int& element = std::get<0>(tuple);
        // std::cout<<" element "<<element<<std::endl;
        element *= fact / per.size();
    }
    if (b.size() == 1)
    {
        return b;
    }
    else
    {
        std::vector<std::tuple<int, std::vector<int>>> group;

        for (const auto& pair1 : b)
        {
            int n = std::get<0>(pair1);
            std::vector<int> values1 = std::get<1>(pair1);

            int mm = 2 * n;
            auto it1 = std::find(group.begin(), group.end(), pair1);
            auto it2 = std::find(group.begin(), group.end(), std::make_tuple(mm, values1));

            if (it1 != group.end() || it2 != group.end())
            {
                continue;
            }

            bool equiv = false;

            for (const auto& pair2 : b)
            {
                int m = std::get<0>(pair2);
                std::vector<int> values2 = std::get<1>(pair2);
                int mm = 2 * n;
                auto it1 = std::find(group.begin(), group.end(), pair2);
                auto it2 = std::find(group.begin(), group.end(), std::make_tuple(2 * m, values2));

                if (it1 != group.end() || it2 != group.end())
                {
                    continue;
                }
                std::vector<int> result_arr;
                for (int x : values2)
                {
                    result_arr.push_back(x == -1 ? 0 : (x == 0 ? -1 : x));
                }

                if (n == m && values1 == result_arr)
                {

                    equiv = true;
                    break;
                }
            }

            if (equiv)
            {
                group.push_back(std::make_tuple(mm, values1));
            }
        }

        return group;
    }
}

void constterm(const int k, const int j, mp_limb_signed_t N, const int nv, fmpz_mpoly_t result, const fmpz_mpoly_ctx_t ctx)
{
    // Initialize the result polynomial
    fmpz_mpoly_zero(result, ctx);

    // Loop over the values of i from 1 to N
    for (mp_limb_signed_t i = 1; i <= N; ++i)
    {
        // Compute the exponents for x_k and x_j
        ulong exp_k = N + i; // Exponent for x_k
        ulong exp_j = N - i; // Exponent for x_j

        // Create a polynomial term for i * x_k^(N+i) * x_j^(N-i)
        fmpz_mpoly_t term;
        fmpz_mpoly_init(term, ctx);

        // Set the coefficient and exponents in the polynomial term
        ulong exp[nv]; // Exponent vector for N variables
        for (int idx = 0; idx < nv; ++idx)
        {
            exp[idx] = 0; // Initialize all exponents to 0
        }
        exp[k - 1] = exp_k;                            // Set exponent for variable k
        exp[j - 1] = exp_j;                            // Set exponent for variable j
        fmpz_mpoly_set_coeff_ui_ui(term, i, exp, ctx); // Set coefficient i for the term

        // Add the term to the result polynomial
        fmpz_mpoly_add(result, result, term, ctx);

        // Clear the polynomial term
        fmpz_mpoly_clear(term, ctx);
    }
}

void proterm(const int k, const int j, int a, mp_limb_signed_t N, const int nv, fmpz_mpoly_t result, const fmpz_mpoly_ctx_t ctx)
{
    // Initialize the result polynomial
    fmpz_mpoly_zero(result, ctx);

    // Loop over the values of w from 1 to a
    for (int w = 1; w <= a; ++w)
    {
        // Check if a is divisible by w
        if (a % w == 0)
        {
            // Compute the exponents for x_k and x_j
            ulong exp_k1 = N + w; // Exponent for x_k in the first term
            ulong exp_j1 = N - w; // Exponent for x_j in the first term
            ulong exp_k2 = N - w; // Exponent for x_k in the second term
            ulong exp_j2 = N + w; // Exponent for x_j in the second term

            // Create polynomial terms for x_k^(N+w)*x_j^(N-w) and x_k^(N-w)*x_j^(N+w)
            fmpz_mpoly_t term1, term2;
            fmpz_mpoly_init(term1, ctx);
            fmpz_mpoly_init(term2, ctx);

            // Set the coefficients and exponents in the polynomial terms
            ulong exp1[nv] = { 0 };  // Exponent vector for the first term
            exp1[k - 1] = exp_k1; // Set exponent for variable k
            exp1[j - 1] = exp_j1; // Set exponent for variable j
            ulong exp2[nv] = { 0 };  // Exponent vector for the second term
            exp2[k - 1] = exp_k2; // Set exponent for variable k
            exp2[j - 1] = exp_j2; // Set exponent for variable j

            // Set the coefficient and exponents in the polynomial terms
            fmpz_mpoly_set_coeff_ui_ui(term1, w, exp1, ctx); // Set coefficient w for the first term
            fmpz_mpoly_set_coeff_ui_ui(term2, w, exp2, ctx); // Set coefficient w for the second term

            // Add the terms to the result polynomial
            fmpz_mpoly_add(result, result, term1, ctx);
            fmpz_mpoly_add(result, result, term2, ctx);

            // Clear memory for the polynomial terms
            fmpz_mpoly_clear(term1, ctx);
            fmpz_mpoly_clear(term2, ctx);
        }
    }
}

unsigned long feynman_integral_type(graph Gv, int factor, std::vector<int> av)
{
    if (av.size() != Gv.size())
    {
        throw std::runtime_error(
            "av should be of length " + std::to_string(Gv.size()) +
            ", but it is of size " + std::to_string(av.size()));
    }
    else
    {
        std::unordered_set<int> nbv;
        for (const auto& e : Gv)
        {
            nbv.insert(e.first);
            nbv.insert(e.second);
        }
        int nv = nbv.size();
        // nb vertices.
        int N = std::accumulate(av.begin(), av.end(), 0, [](int sum, int val)
            { return sum + (val > 0 ? val : 0); });

        unsigned long p = 0;
        std::vector<int> fey_degree;
        fmpz_mpoly_ctx_t ctx;
        fmpz_mpoly_ctx_init(ctx, nv, ORD_DEGLEX);
        // Initialize tmp polynomial
        fmpz_mpoly_t tmp;
        fmpz_mpoly_init(tmp, ctx);
        fmpz_mpoly_set_ui(tmp, 1, ctx);

        for (int i = 1; i <= 1; ++i)
        {
            int j = 0;

            for (const auto& multiplicity : av)
            {
                if (multiplicity == -1)
                {
                    fmpz_mpoly_t constterm1_j;
                    fmpz_mpoly_init(constterm1_j, ctx);
                    constterm(Gv[j].first, Gv[j].second, N, nv, constterm1_j, ctx);
                    fmpz_mpoly_mul(tmp, tmp, constterm1_j, ctx);
                    fmpz_mpoly_clear(constterm1_j, ctx);
                }
                else if (multiplicity == 0)
                {
                    fmpz_mpoly_t constterm0_j;
                    fmpz_mpoly_init(constterm0_j, ctx);
                    constterm(Gv[j].second, Gv[j].first, N, nv, constterm0_j, ctx);
                    fmpz_mpoly_mul(tmp, tmp, constterm0_j, ctx);
                    fmpz_mpoly_clear(constterm0_j, ctx);
                }
                else
                {
                    fmpz_mpoly_t proterm_j;
                    fmpz_mpoly_init(proterm_j, ctx);
                    proterm(Gv[j].first, Gv[j].second, multiplicity, N, nv, proterm_j, ctx);
                    fmpz_mpoly_mul(tmp, tmp, proterm_j, ctx);
                    fmpz_mpoly_clear(proterm_j, ctx);
                }

                j++;
            }
        }

        mp_limb_t exp_product[nv];
        for (int i = 0; i < nv; ++i)
        {
            exp_product[i] = 3 * N;
        }

        // Initialize vars array
        slong vars[nv];
        for (int i = 0; i < nv; ++i)
        {
            vars[i] = i;
        }

        // Get the coefficient
        fmpz_mpoly_get_coeff_vars_ui(tmp, tmp, vars, exp_product, nv, ctx);

        // Multiply the coefficient by factor=2
        fmpz_mpoly_scalar_mul_si(tmp, tmp, factor, ctx);

        ulong coeff;
        fmpz_t coeff_fmpz;
        fmpz_init(coeff_fmpz);
        fmpz_mpoly_get_fmpz(coeff_fmpz, tmp, ctx);
        coeff = fmpz_get_ui(coeff_fmpz);
        fmpz_clear(coeff_fmpz);

        // Clear memory
        fmpz_mpoly_clear(tmp, ctx);
        fmpz_mpoly_ctx_clear(ctx);

        // Return the result
        // p += coeff;
        return coeff;
    }
}
unsigned long feynman_integral_branch_type(graph Gv, std::vector<int> a)
{
    std::vector<std::tuple<int, std::vector<int>>> f = signature_and_multiplicitie(Gv, a);
    unsigned long sum = 0;
    for (const auto& tuple : f)
    {
        int factor = std::get<0>(tuple);
        std::vector<int> av = std::get<1>(tuple);

        unsigned long fe = feynman_integral_type(Gv, factor, av);
        sum += fe;
    }
    return sum;
}
unsigned long feynman_integral_degree(graph Gv, int d)
{
    unsigned long sum = 0;
    int ne = Gv.size();
    vector2d gen = gen_block(ne, d);
    for (std::vector<int> xa : gen)
    {
        vector2d it = iterate(xa);
        for (std::vector<int> xi : it)
        {
            sum += feynman_integral_branch_type(Gv, xi);
        }
    }
    return sum;
}
// Function to find the number of vertices in a list of edges
int nv(const std::vector<std::pair<int, int>>& ve) {
    std::set<int> vertices;
    for (const auto& edge : ve) {
        vertices.insert(edge.first);
        vertices.insert(edge.second);
    }
    return vertices.size();
}
// Function to find pairs of indices for equal edges
std::vector<std::vector<int>> find_equal_pairs(const std::vector<std::pair<int, int>>& ve) {
    std::map<std::pair<int, int>, std::vector<int>> equal_pairs;

    // Iterate over the input vector of pairs
    for (std::size_t i = 0; i < ve.size(); ++i) {
        // Append the index 'i' to the vector corresponding to the pair 've[i]' in the map
        equal_pairs[ve[i]].push_back(i);
    }

    // Declare a vector of vectors to store the indices of equal pairs
    std::vector<std::vector<int>> indices;

    // Iterate over the map
    for (auto it = equal_pairs.begin(); it != equal_pairs.end(); ++it) {
        // If the size of the vector (value) is greater than 1, it means there are duplicate pairs
        if (it->second.size() > 1) {
            // Add the vector of indices to the 'indices' vector
            indices.push_back(it->second);
        }
    }

    // Return the vector of indices
    return indices;
}
// Function to convert a vector to a monomial given the graph (edges)
std::string vector_to_monomial(const graph& G, const std::vector<int>& v) {
    std::vector<int> scaled_v = v;
    for (auto& val : scaled_v) {
        val *= 2;
    }
    std::string poly = "Poly:";
    for (size_t i = 0; i < scaled_v.size(); ++i) {
        poly += " q[" + std::to_string(i) + "]^" + std::to_string(scaled_v[i]);
        if (i < scaled_v.size() - 1) {
            poly += " *";
        }
    }
    return poly;
}

// Function to generate all unique permutations of a list based on specified indices
std::vector<std::vector<int>> generate_permutation(const std::vector<int>& l, const std::vector<std::vector<int>>& indices) {
    std::set<std::vector<int>> permuted_lists;
    std::vector<int> original_l = l;

    std::vector<std::vector<std::vector<int>>> group_permutations(indices.size());
    for (size_t i = 0; i < indices.size(); ++i) {
        std::vector<int> group_elements;
        for (int idx : indices[i]) {
            group_elements.push_back(l[idx]);
        }
        std::sort(group_elements.begin(), group_elements.end());
        do {
            group_permutations[i].push_back(group_elements);
        } while (std::next_permutation(group_elements.begin(), group_elements.end()));
    }

    std::vector<size_t> group_indices(indices.size(), 0);
    do {
        std::vector<int> temp_l = original_l;
        for (size_t i = 0; i < indices.size(); ++i) {
            for (size_t j = 0; j < indices[i].size(); ++j) {
                temp_l[indices[i][j]] = group_permutations[i][group_indices[i]][j];
            }
        }
        permuted_lists.insert(temp_l);

        // Increment the group indices
        for (size_t i = 0; i < group_indices.size(); ++i) {
            group_indices[i]++;
            if (group_indices[i] >= group_permutations[i].size()) {
                group_indices[i] = 0;
            }
            else {
                break;
            }
        }
    } while (!std::all_of(group_indices.begin(), group_indices.end(), [](size_t idx) { return idx == 0; }));

    return std::vector<std::vector<int>>(permuted_lists.begin(), permuted_lists.end());
}
/* struct IterateResult {
    bool is_vector2d;
    vector2d vectord;
    signature pairs;
};
 */

vector2d iterate_permutation(const graph& ve, const std::vector<int>& a) {
    auto indices = find_equal_pairs(ve);
    vector2d res;
    auto L = iterate(a);
    vector2d gg;

    for (const auto& ai : L) {
        if (std::find(gg.begin(), gg.end(), ai) == gg.end()) {
            auto ge = generate_permutation(ai, indices);
            std::vector<int> temp;
            temp.push_back(ge.size());
            temp.insert(temp.end(), ai.begin(), ai.end());
            /*  for (int a : temp) {
                 std::cout << a << " ";
             }
             std::cout << std::endl; */
            res.push_back(temp);
            gg.insert(gg.end(), ge.begin(), ge.end());
        }
    }
    return  res;

}