
#include <list>
#include <tuple>
#include <sstream> // for std::istringstream
#include "../../code/boost/include/boost/variant.hpp"
#include "../../code/util-generic/split.hpp"
#include "../../code/we/type/value.hpp"
#include <stdio.h>
#include <flint/fmpz.h>
#include <flint/fmpz_poly.h>
#include <flint/fmpq_mpoly.h>
#include <flint/fmpq_poly.h>
#include <flint/fmpq.h>
#include <flint/fmpz_mpoly.h>
#include <stdlib.h> // for malloc and free
#include <flint/fmpz.h>
#include <flint/bernoulli.h>

#include <fstream>
#include <sys/resource.h>
#include <iostream>
#include <chrono>
#include <functional> // Include for std::function
#include <vector>     // Include for std::vector
#include <numeric>
#include <unordered_set> // for std::unordered_set
#include <algorithm>

using Element = std::pair<std::pair<int, int>, std::pair<int, int>>;
using Sequence = std::vector<Element>;

using pnet_value = pnet::type::value::value_type;
using pnet_list = std::list<pnet_value>;
using pnet_list2d = std::list<std::list<pnet_value>>;
using vector2d = std::vector<std::vector<int>>;
using list_type = std::list<std::string>; // Define list_type as std::list<std::string>
using graph = std::vector<std::pair<int, int>>;
using Edge = std::pair<int, int>;
using signature = std::vector<std::tuple<int, std::vector<int>>>;
std::size_t get_memory_usage()
{
    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    return r_usage.ru_maxrss; // ru_maxrss is in kilobytes
}

template <typename Func>
struct ResourceUsage
{
    std::size_t memory_usage; // in kilobytes
    long long elapsed_time;   // in microseconds
};

template <typename Func>
ResourceUsage<Func> measure_resource_usage(Func func)
{
    // Measure start time
    auto start_time = std::chrono::steady_clock::now();

    // Measure start memory usage
    std::size_t start_memory = get_memory_usage();

    // Execute the provided function
    func();

    // Measure end time
    auto end_time = std::chrono::steady_clock::now();

    // Measure end memory usage
    std::size_t end_memory = get_memory_usage();

    // Calculate elapsed time
    auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    // Calculate memory usage difference
    std::size_t memory_usage = end_memory - start_memory;

    return { memory_usage, elapsed_time };
}

std::string vectorToStringInt(const std::vector<int>& vec) {
    std::stringstream ss;
    for (int val : vec) {
        ss << val << ' ';
    }
    return ss.str();
}

std::vector<int> stringToVectorInt(const std::string& str) {
    std::vector<int> result;
    std::stringstream ss(str);
    int num;
    while (ss >> num) {
        result.push_back(num);
    }
    return result;
}
std::string fmpqToString(const fmpq_t f) {
    char* str = fmpq_get_str(NULL, 10, f);
    std::string result(str);
    flint_free(str);
    return result;
}

// Function to convert string to fmpq_t
void stringToFmpq(fmpq_t f, const std::string& str) {
    fmpq_set_str(f, str.c_str(), 10);
    fmpq_canonicalise(f);
}


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

unsigned long  binomial(const int n, const int k) {
    std::vector<unsigned long > vec(k);
    vec[0] = n - k + 1;

    for (int i = 1; i < k; ++i) {
        vec[i] = vec[i - 1] * (n - k + 1 + i) / (i + 1);
    }

    return vec[k - 1];
}
/* unsigned long binomial(const int n, const int k)
{
    if (k < 0 || k > n)
    {
        throw std::invalid_argument("Invalid arguments for binomial coefficient");
    }

    unsigned long result = 1;
    for (unsigned long i = 1; i <= k; ++i)
    {
        result *= (n - i + 1);
        result /= i;
    }
    return result;
} */

std::vector<int> next_partition(std::vector<int> a)
{
    int n = std::accumulate(a.begin(), a.end(), 0);
    int k = a.size();
    if (a[k - 1] == n)
    {
        return a;
    }
    for (int i = k - 1; i >= 0; --i)
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
    return a;
}

vector2d iterate(std::vector<int> xa)
{
    vector2d gen;

    int k = xa.size();
    if (k == 0)
    {
        throw std::invalid_argument("k should be nonzero");
    }
    int d = std::accumulate(xa.begin(), xa.end(), 0);
    if (d == xa[0])
    {
        gen.push_back(xa);
    }

    unsigned long factorial = binomial(d + k - 1, d);

    int e = d - xa[0] + 1;

    for (unsigned long i = 0; i < factorial; ++i)
    {
        if (xa[k - 1] != e)
        {
            xa = next_partition(xa);
            gen.push_back(xa);
        }
        else
        {
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
        Edge ev = G[i];
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
            Edge ev = G[i];

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
void invsfunction(const int j, const int m, fmpq_mpoly_t result, const fmpq_mpoly_ctx_t ctx) {
    fmpq_mpoly_t term, sum;
    fmpz_t num, den, fact;
    fmpq_t one, coeff;

    // Initialize variables
    fmpq_mpoly_init(term, ctx);
    fmpq_mpoly_init(sum, ctx);
    fmpz_init(num);
    fmpz_init(den);
    fmpz_init(fact);
    fmpq_init(one);
    fmpq_init(coeff);

    // Set result to zero polynomial
    fmpq_mpoly_zero(result, ctx);
    slong nv = fmpq_mpoly_ctx_nvars(ctx);
    int max = nv / 2;
    // Initialize the exponent vector for all variables
    ulong exp[nv];
    for (int idx = 0; idx < nv; ++idx) {
        exp[idx] = 0; // Initialize all exponents to 0
    }

    for (mp_limb_signed_t n = 0; n <= m + 1; ++n) {
        // Compute x^(n)
        ulong exp_j = n; // Exponent for x_j

        fmpq_t bernoulli;
        fmpq_init(bernoulli);
        bernoulli_fmpq_ui(bernoulli, n); // Compute B_n

        fmpz_ui_pow_ui(num, 2, n); // num = 2^n
        fmpz_sub_ui(num, num, 2); // num = 2^n - 2

        fmpq_set_fmpz(coeff, num); // coeff = num

        fmpz_ui_pow_ui(den, 2, n); // den = 2^n
        fmpz_fac_ui(fact, n); // fact = n!
        fmpz_mul(den, den, fact); // den = 2^n * n!
        fmpq_div_fmpz(coeff, coeff, den); // coeff = coeff / den

        fmpq_mul(coeff, coeff, bernoulli); // coeff = coeff * B_n

        // Create a polynomial term for x_j^(n)
        fmpq_mpoly_init(term, ctx);

        // Set exponent for variable 
        exp[j + max - 1] = exp_j;

        fmpq_mpoly_set_coeff_fmpq_ui(term, coeff, exp, ctx); // Set term with the computed coefficient
        fmpq_mpoly_sub(result, result, term, ctx); // Add term to result

        fmpq_clear(bernoulli);
        fmpq_mpoly_clear(term, ctx); // Clear term
    }

    // Clear temporary variables
    fmpz_clear(num);
    fmpz_clear(den);
    fmpz_clear(fact);
    fmpq_clear(one);
    fmpq_clear(coeff);
}

void sfunction(const int w, const int j, const int m, fmpq_mpoly_t result, const fmpq_mpoly_ctx_t ctx) {
    fmpq_mpoly_zero(result, ctx); // Initialize result as the zero polynomial
    fmpq_t coeff;
    fmpz_t fact, binom_coeff, num;
    fmpz_init(fact);
    fmpz_init(num);
    fmpz_init(binom_coeff);
    fmpq_init(coeff);

    slong nv = fmpq_mpoly_ctx_nvars(ctx);
    int max = nv / 2;
    // Initialize the exponent vector for all variables
    ulong exp[nv];

    for (mp_limb_signed_t n = 0; n <= m; ++n) {
        // Reset exponents to 0
        for (int idx = 0; idx < nv; ++idx) {
            exp[idx] = 0;
        }

        // Compute x^(2*n)
        ulong exp_j = 2 * n; // Exponent for x_j

        // Create a polynomial term for x_j^(2*n)
        exp[j + max - 1] = exp_j;

        fmpq_mpoly_t term;
        fmpq_mpoly_init(term, ctx);

        // Compute factorial coefficient (2 * n + 1)!
        fmpz_fac_ui(binom_coeff, 2 * n + 1);

        // Compute 2^(2*n)
        fmpz_ui_pow_ui(fact, 2, 2 * n);
        fmpz_mul(fact, fact, binom_coeff); // fact = 2^(2*n) * (2*n + 1)!

        fmpq_set_si(coeff, 1, 1);  // Set coefficient to 1/1
        // Compute w^(2*n)

        fmpz_ui_pow_ui(num, w, 2 * n);
        fmpq_div_fmpz(coeff, coeff, fact); // coeff = 1 / fact
        fmpq_mul_fmpz(coeff, coeff, num);
        fmpq_mpoly_set_coeff_fmpq_ui(term, coeff, exp, ctx); // Set term with the computed coefficient

        // Add the term to the result
        fmpq_mpoly_add(result, result, term, ctx);

        fmpq_mpoly_clear(term, ctx); // Clear term
    }

    // Clear temporary variables
    fmpz_clear(fact);
    fmpz_clear(num);
    fmpz_clear(binom_coeff);
    fmpq_clear(coeff);
}

// Function to compute loopterm
void loopterm(const int  z, const int a, const int m, fmpq_mpoly_t p, const fmpq_mpoly_ctx_t ctx) {
    fmpq_mpoly_zero(p, ctx); // Initialize p as the zero polynomial

    if (a == 0) {
        return; // If a is 0, return 0 polynomial
    }

    slong nv = fmpq_mpoly_ctx_nvars(ctx);
    ulong exp[nv];
    for (int idx = 0; idx < nv; ++idx) {
        exp[idx] = 0;
    }

    for (int w = 1; w <= a; ++w) {
        if (a % w == 0) {
            fmpq_mpoly_t S1, term, q_powered, S1_squared;
            fmpq_mpoly_init(S1, ctx);
            fmpq_mpoly_init(term, ctx);
            fmpq_mpoly_init(q_powered, ctx);
            fmpq_mpoly_init(S1_squared, ctx);

            // Compute S1 = sfunction(w * z, m)
            sfunction(w, z, m, S1, ctx); // Assuming 0 for the variable index

            // Compute S1 * S1
            fmpq_mpoly_mul(S1_squared, S1, S1, ctx);
            // Compute q^(2*a)

            exp[z] = 2 * a; // Assuming q is the first variable

            // Compute S1 * S1 * w * q^(2*a)
            fmpq_mpoly_scalar_mul_si(term, S1_squared, w, ctx);

            /*  std::cout << " " << std::endl;
             std::cout << "term  " << std::endl;

             fmpq_mpoly_print_pretty(term, NULL, ctx); */

             // Accumulate the result in p
            fmpq_mpoly_add(p, p, term, ctx);

            // Clear temporary variables
            fmpq_mpoly_clear(S1, ctx);
            fmpq_mpoly_clear(term, ctx);
            fmpq_mpoly_clear(q_powered, ctx);
            fmpq_mpoly_clear(S1_squared, ctx);
        }
    }
}
void constterm(const int k, const int j, const int i1, const int i2, mp_limb_signed_t N, const int m, fmpq_mpoly_t result, const fmpq_mpoly_ctx_t ctx) {
    // Initialize the result polynomial
    fmpq_mpoly_zero(result, ctx);
    slong nv = fmpq_mpoly_ctx_nvars(ctx);

    // Loop over the values of i from 1 to N
    for (mp_limb_signed_t i = 1; i <= N; ++i) {
        // Compute the exponents for x_k and x_j
        ulong exp_k = N + i; // Exponent for x_k
        ulong exp_j = N - i; // Exponent for x_j

        // Create a polynomial term for i * x_k^(N+i) * x_j^(N-i)
        fmpq_mpoly_t term;
        fmpq_mpoly_init(term, ctx);

        // Set the coefficient and exponents in the polynomial term
        ulong exp[nv]; // Exponent vector for N variables
        for (int idx = 0; idx < nv; ++idx) {
            exp[idx] = 0; // Initialize all exponents to 0
        }
        exp[k - 1] = exp_k; // Set exponent for variable k
        exp[j - 1] = exp_j; // Set exponent for variable j

        // Convert coefficient i to fmpq_t type (fraction type in FLINT)
        fmpq_t coeff_i;
        fmpq_init(coeff_i);
        fmpq_set_si(coeff_i, i, 1); // Set coeff_i to i (as a fraction)

        fmpq_mpoly_t sum1, sum2, prod;
        fmpq_mpoly_init(sum1, ctx);
        fmpq_mpoly_init(sum2, ctx);
        fmpq_mpoly_init(prod, ctx);

        sfunction(i, i1, m, sum1, ctx);
        sfunction(i, i2, m, sum2, ctx);
        fmpq_mpoly_mul(prod, sum1, sum2, ctx);

        // Set coefficient i for the term
        fmpq_mpoly_set_coeff_fmpq_ui(term, coeff_i, exp, ctx);
        fmpq_mpoly_mul(term, term, prod, ctx);

        // Add the term to the result polynomial
        fmpq_mpoly_add(result, result, term, ctx);

        // Clear the polynomial term and coefficient
        fmpq_clear(coeff_i);
        fmpq_mpoly_clear(term, ctx);
        fmpq_mpoly_clear(sum1, ctx);
        fmpq_mpoly_clear(sum2, ctx);
        fmpq_mpoly_clear(prod, ctx);
    }
}

void proterm(const int k, const int j, const int i1, const int i2, const int a, const int m, mp_limb_signed_t N, fmpq_mpoly_t result, const fmpq_mpoly_ctx_t ctx) {
    // Initialize the result polynomial
    fmpq_mpoly_zero(result, ctx);
    slong nv = fmpq_mpoly_ctx_nvars(ctx);

    // Loop over the values of w from 1 to a
    for (mp_limb_signed_t w = 1; w <= a; ++w)
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
            fmpq_mpoly_t term1, term2;
            fmpq_mpoly_init(term1, ctx);
            fmpq_mpoly_init(term2, ctx);

            // Set the coefficients and exponents in the polynomial terms
            ulong exp1[nv];  // Exponent vector for the first term
            ulong exp2[nv];  // Exponent vector for the second term
            for (int idx = 0; idx < nv; ++idx)
            {
                exp1[idx] = 0;
                exp2[idx] = 0;
            }
            exp1[k - 1] = exp_k1; // Set exponent for variable k
            exp1[j - 1] = exp_j1; // Set exponent for variable j
            exp2[k - 1] = exp_k2; // Set exponent for variable k
            exp2[j - 1] = exp_j2; // Set exponent for variable j

            fmpq_mpoly_t sum1, sum2, prod, term;
            fmpq_mpoly_init(term, ctx);

            fmpq_mpoly_init(sum1, ctx);
            fmpq_mpoly_init(sum2, ctx);
            fmpq_mpoly_init(prod, ctx);

            sfunction(w, i1, m, sum1, ctx);
            sfunction(w, i2, m, sum2, ctx);
            fmpq_mpoly_mul(prod, sum1, sum2, ctx);

            // Set the coefficient w (as a rational number with denominator 1) for the terms
            fmpq_t coeff;
            fmpq_init(coeff);
            fmpq_set_si(coeff, w, 1); // Set coefficient w/1

            // Set the coefficient and exponents in the polynomial terms
            fmpq_mpoly_set_coeff_fmpq_ui(term1, coeff, exp1, ctx); // Set coefficient w for the first term
            fmpq_mpoly_set_coeff_fmpq_ui(term2, coeff, exp2, ctx); // Set coefficient w for the second term

            // Add the terms to the result polyn'""é&omial
            fmpq_mpoly_add(term, term2, term1, ctx);
            fmpq_mpoly_mul(term, term, prod, ctx);
            fmpq_mpoly_add(result, result, term, ctx);

            // Clear the rational coefficient
            fmpq_clear(coeff);

            // Clear memory for the polynomial terms
            fmpq_mpoly_clear(term1, ctx);
            fmpq_mpoly_clear(term2, ctx);
        }
    }
}

// Function to get the coefficient of a specific monomial
void get_coefficient(fmpq_mpoly_t coeff, const fmpq_mpoly_t poly, const mp_limb_signed_t* var_indices, const mp_limb_t* exps, mp_limb_signed_t length, const fmpq_mpoly_ctx_t ctx) {
    fmpq_mpoly_get_coeff_vars_ui(coeff, poly, var_indices, exps, length, ctx);
}


// Function to calculate node values based on edges, value d, and vector l
std::vector<int> lise(const std::vector<Edge>& G, int d, const std::vector<int>& l) {
    // Determine the number of nodes
    int num_nodes = 0;
    for (const auto& edge : G) {
        num_nodes = std::max(num_nodes, std::max(edge.first, edge.second));
    }
    // Create a vector to store node values
    std::vector<int> L(num_nodes, 0);

    // Iterate over each edge
    for (const auto& edge : G) {
        int source = edge.first;
        int destination = edge.second;

        // Check if source and destination are different
        if (source != destination) {
            L[source - 1] += d;
            L[destination - 1] += d;
        }
    }

    // Add values from vector l to specific nodes
    for (int i = 0; i < l.size(); ++i) {
        if (i < L.size()) {
            L[i] += l[i];
        }
    }

    return L;
}

void filter_term(fmpq_mpoly_t result, const fmpq_mpoly_t poly, const std::vector<slong>& variables, const std::vector<int>& power, const fmpq_mpoly_ctx_t ctx) {
    fmpq_t coeff;
    fmpq_init(coeff);

    fmpq_mpoly_zero(result, ctx);  // Initialize result to zero polynomial

    ulong* exps = new ulong[fmpq_mpoly_ctx_nvars(ctx)];

    slong num_terms = fmpq_mpoly_length(poly, ctx);
    for (slong i = 0; i < num_terms; ++i) {
        fmpq_mpoly_get_term_exp_ui(exps, poly, i, ctx);

        bool within_power = true;
        for (size_t j = 0; j < variables.size(); ++j) {
            if (exps[variables[j]] > power[j]) {
                within_power = false;
                break;
            }
        }

        if (within_power) {
            fmpq_mpoly_get_coeff_fmpq_ui(coeff, poly, exps, ctx);
            fmpq_mpoly_set_coeff_fmpq_ui(result, coeff, exps, ctx);
        }
    }

    fmpq_clear(coeff);
    delete[] exps;
}

void feynman_integral_branch_type(fmpq_t myfey, graph& Gv, const std::vector<int>& av, const std::vector<int>& g = std::vector<int>(), const std::vector<int>& l = std::vector<int>()) {
    if (av.size() != Gv.size()) {
        throw std::runtime_error("av should be of length " + std::to_string(Gv.size()) + ", but it is of size " + std::to_string(av.size()));
    }

    std::unordered_set<int> nbv;
    for (const auto& e : Gv) {
        nbv.insert(e.first);
        nbv.insert(e.second);
    }

    int nv = nbv.size();
    int N = std::accumulate(av.begin(), av.end(), 0, [](int sum, int val) {
        return sum + (val > 0 ? val : 0);
        });

    std::vector<int> local_g = g.empty() ? std::vector<int>(nv, 0) : g;
    std::vector<int> local_l = l.empty() ? std::vector<int>(nv, 0) : l;
    std::vector<int> L = lise(Gv, N, local_l);

    fmpq_mpoly_ctx_t ctx;
    fmpq_mpoly_ctx_init(ctx, 2 * nv, ORD_DEGLEX);

    mp_limb_signed_t var_indices[nv];
    for (int i = 0; i < nv; ++i) {
        var_indices[i] = static_cast<mp_limb_t>(i);
    }
    mp_limb_signed_t length = nv;
    mp_limb_t LL[nv];
    for (int i = 0; i < nv; ++i) {
        LL[i] = static_cast<mp_limb_t>(L[i]);
    }

    // Initialize g_power and vars_g
    mp_limb_t g_power[nv];
    mp_limb_signed_t vars_g[nv];

    for (int i = 0; i < nv; ++i) {
        g_power[i] = static_cast<mp_limb_t>(2 * local_g[i]);
        vars_g[i] = static_cast<mp_limb_signed_t>(nv + i);
    }
    std::vector<int> filter_power;
    std::vector<slong> filter_vars;
    for (int i = 0; i < nv; ++i) {
        filter_power.push_back(2 * local_g[i]);
        filter_vars.push_back(nv + i);
    }
    int m = *std::max_element(filter_power.begin(), filter_power.end());

    fmpq_t sum;
    fmpq_init(sum);
    fmpq_zero(sum);

    fmpq_mpoly_t invsfunct;
    fmpq_mpoly_init(invsfunct, ctx);
    fmpq_mpoly_one(invsfunct, ctx); // Set invsfunct to 1 for multiplication

    for (int jj = 1; jj <= nv; ++jj) {
        fmpq_mpoly_t temp, filtered_temp;
        fmpq_mpoly_init(temp, ctx);
        fmpq_mpoly_init(filtered_temp, ctx);

        // Compute the temporary polynomial
        invsfunction(jj, m, temp, ctx);

        // Filter the term
        filter_term(filtered_temp, temp, filter_vars, filter_power, ctx);

        // Multiply invsfunct by the filtered_temp polynomial
        fmpq_mpoly_t result;
        fmpq_mpoly_init(result, ctx);
        fmpq_mpoly_mul(result, invsfunct, filtered_temp, ctx);

        // Update invsfunct to the new result
        fmpq_mpoly_set(invsfunct, result, ctx);

        // Clear temporary polynomials
        fmpq_mpoly_clear(temp, ctx);
        fmpq_mpoly_clear(filtered_temp, ctx);
        fmpq_mpoly_clear(result, ctx);
    }

    // Final filtering of the product
    fmpq_mpoly_t final_result;
    fmpq_mpoly_init(final_result, ctx);
    filter_term(final_result, invsfunct, filter_vars, filter_power, ctx);


    // Use invsfunct in the loop after filtering
    fmpq_mpoly_set(invsfunct, final_result, ctx);
    fmpq_mpoly_clear(final_result, ctx);

    signature f = signature_and_multiplicitie(Gv, av);

    for (size_t i = 0; i < f.size(); ++i) {
        int fi1;
        std::vector<int> fi2;
        std::tie(fi1, fi2) = f[i];
        fmpq_mpoly_t tmp;
        fmpq_mpoly_init(tmp, ctx);
        fmpq_mpoly_set_ui(tmp, 1, ctx);

        for (size_t j = 0; j < fi2.size(); ++j) {
            if (fi2[j] == -1) {
                fmpq_mpoly_t constterm1_j, filter_term1;
                fmpq_mpoly_init(constterm1_j, ctx);
                fmpq_mpoly_init(filter_term1, ctx);

                constterm(Gv[j].first, Gv[j].second, Gv[j].first, Gv[j].second, N, m, constterm1_j, ctx);
                filter_term(filter_term1, constterm1_j, filter_vars, filter_power, ctx);

                fmpq_mpoly_mul(tmp, tmp, filter_term1, ctx);
                fmpq_mpoly_clear(constterm1_j, ctx);
                fmpq_mpoly_clear(filter_term1, ctx);

            }
            else if (fi2[j] == 0) {
                fmpq_mpoly_t constterm0_j, filter_term0;
                fmpq_mpoly_init(constterm0_j, ctx);
                fmpq_mpoly_init(filter_term0, ctx);
                constterm(Gv[j].second, Gv[j].first, Gv[j].second, Gv[j].first, N, m, constterm0_j, ctx);
                filter_term(filter_term0, constterm0_j, filter_vars, filter_power, ctx);
                fmpq_mpoly_mul(tmp, tmp, filter_term0, ctx);
                fmpq_mpoly_clear(constterm0_j, ctx);
                fmpq_mpoly_clear(filter_term0, ctx);
            }
            else if (fi2[j] == -2) {
                fmpq_mpoly_t looptermj, filter_term_loop;
                fmpq_mpoly_init(looptermj, ctx);
                fmpq_mpoly_init(filter_term_loop, ctx);
                loopterm(Gv[j].first, av[j], m, looptermj, ctx);
                filter_term(filter_term_loop, looptermj, filter_vars, filter_power, ctx);
                fmpq_mpoly_mul(tmp, tmp, filter_term_loop, ctx);

                fmpq_mpoly_clear(looptermj, ctx);
                fmpq_mpoly_clear(filter_term_loop, ctx);
            }
            else {
                fmpq_mpoly_t proterm_j, filter_term_prot;
                fmpq_mpoly_init(proterm_j, ctx);
                fmpq_mpoly_init(filter_term_prot, ctx);
                proterm(Gv[j].first, Gv[j].second, Gv[j].first, Gv[j].second, fi2[j], m, N, proterm_j, ctx);
                filter_term(filter_term_prot, proterm_j, filter_vars, filter_power, ctx);
                fmpq_mpoly_mul(tmp, tmp, filter_term_prot, ctx);

                fmpq_mpoly_clear(proterm_j, ctx);
                fmpq_mpoly_clear(filter_term_prot, ctx);
            }
        }

        fmpq_mpoly_t coeff_poly;
        fmpq_mpoly_init(coeff_poly, ctx);

        get_coefficient(coeff_poly, tmp, var_indices, LL, length, ctx);
        fmpq_mpoly_mul(coeff_poly, coeff_poly, invsfunct, ctx);
        get_coefficient(coeff_poly, coeff_poly, vars_g, g_power, length, ctx);

        fmpq_mpoly_scalar_mul_si(coeff_poly, coeff_poly, fi1, ctx);

        fmpq_t temp_result;
        fmpq_init(temp_result);
        fmpq_mpoly_get_fmpq(temp_result, coeff_poly, ctx);
        fmpq_add(sum, sum, temp_result);
        fmpq_clear(temp_result);

        fmpq_mpoly_clear(coeff_poly, ctx);
        fmpq_mpoly_clear(tmp, ctx);
    }

    fmpq_set(myfey, sum);
    fmpq_clear(sum);
    fmpq_mpoly_clear(invsfunct, ctx); // Clear invsfunct here
    fmpq_mpoly_ctx_clear(ctx);
}
/*
void feynman_integral_branch_type(fmpq_t result, const std::vector<std::pair<int, int>>& Gv, const std::vector<int>& a, const int m, const std::vector<int>& l, const std::vector<int>& g) {
    std::vector<std::tuple<int, std::vector<int>>> f = signature_and_multiplicitie(Gv, a); // Assuming signature_and_multiplicitie is defined elsewhere
    fmpq_t sum;
    fmpq_init(sum);
    fmpq_zero(sum);

    for (const auto& tuple : f) {
        int factor = std::get<0>(tuple);
        const std::vector<int>& av = std::get<1>(tuple);

        fmpq_t term_result;
        fmpq_init(term_result);
        feynman_integral_type(term_result, Gv, factor, av, m, l, g);
        fmpq_add(sum, sum, term_result);
        fmpq_clear(term_result);
    }

    fmpq_set(result, sum);
    fmpq_clear(sum);
}
*/
vector2d compos_iterate(int n, int d)
{
    vector2d gen;
    std::vector<int> xv(n, 0);
    xv[0] = d;

    while (xv[n - 1] != d) {
        gen.push_back(xv);
        xv = next_partition(xv);
    }
    gen.push_back(xv);

    return gen;
}

void feynman_integral_degrees(fmpq_t result, graph Gv, const int& d, const std::vector<int>& g = std::vector<int>(), const std::vector<int>& l = std::vector<int>()) {

    fmpq_t sum;
    fmpq_init(sum);
    fmpq_zero(sum);

    int ne = Gv.size();
    vector2d gen = compos_iterate(ne, d);

    fmpq_t term_result;
    fmpq_init(term_result);

    for (std::vector<int> xa : gen)
    {
        // Call feynman_integral_branch_type for each vector xa from gen
        fmpq_t branch_result;
        fmpq_init(branch_result);
        feynman_integral_branch_type(branch_result, Gv, xa, g, l); // Assuming feynman_integral_branch_type returns fmpq_t

        // Accumulate the results
        fmpq_add(sum, sum, branch_result);

        fmpq_clear(branch_result);
    }

    // Set the final result
    fmpq_set(result, sum);

    // Clear temporary variables
    fmpq_clear(term_result);
    fmpq_clear(sum);
}

std::vector<fmpq_t*> feynman_integral_degree_sum(std::vector<std::pair<int, int>>& Gv, const int& d, const std::vector<int>& g, const std::vector<int>& l) {
    std::vector<fmpq_t*> sum_vec;

    for (int i = 0; i <= d; i++) {
        fmpq_t* result = (fmpq_t*)malloc(sizeof(fmpq_t)); // Allocate memory for fmpq_t
        fmpq_init(*result); // Initialize fmpq_t

        // Call the function with the correct type (dereference result)
        feynman_integral_degrees(*result, Gv, i, g, l);

        sum_vec.push_back(result); // Store the fmpq_t pointer in the vector
    }

    return sum_vec;
}

int main() {
    // Define your graph vertices, multiplicities, and other parameters here
    std::vector<std::pair<int, int>> Gv = { {1, 2}, {2, 3}, {3, 1} }; // Example graph
    int d = 4;
    std::vector<int> av = { 0, 0, 3 }; // Example multiplicities
    std::vector<int> l = { 0, 0, 0 }; // Ensure this matches the number of nodes
    std::vector<int> g = { 1, 0, 0 }; // Ensure this matches the number of nodes
    fmpq_t fey_branch, result;
    fmpq_init(fey_branch);
    fmpq_init(result);
    std::string gs = { 1, 0, 0 };
    std::vector<int> gi = stringToVectorInt(gs);
    std::cout << "g for= ";
    for (int gg : gi) {
        std::cout << gg << " ";
    }
    std::cout << std::endl;

    // Compute Feynman integral of degree d
    feynman_integral_branch_type(fey_branch, Gv, av, g);

    // Print the result
    std::cout << "fey_branch: ";
    fmpq_print(fey_branch);
    std::cout << std::endl;

    feynman_integral_degrees(result, Gv, d, g);

    // Print the result
    std::cout << "result: ";
    fmpq_print(result);
    std::cout << std::endl;

    std::vector<fmpq_t*> results = feynman_integral_degree_sum(Gv, d, g, l);

    // Output results
    std::cout << "Feynman integrals (up to degree " << d << "):\n";
    for (int i = 0; i <= d; i++) {
        std::cout << "Degree " << i << ": ";
        fmpq_print(*results[i]); // Dereference fmpq_t* to get the actual fmpq_t value
        std::cout << std::endl;
    }

    // Clear allocated memory
    for (int i = 0; i <= d; i++) {
        fmpq_clear(*results[i]); // Clear the fmpq_t value
        free(results[i]); // Free the allocated memory
    }
    return 0;
}
