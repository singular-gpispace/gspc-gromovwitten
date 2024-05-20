
#include <list>
#include <tuple>
#include <sstream> // for std::istringstream
#include "../../code/boost/include/boost/variant.hpp"
#include "../../code/util-generic/split.hpp"
#include "../../code/we/type/value.hpp"
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
#include "time_memory.hpp"

using Element = std::pair<std::pair<int, int>, std::pair<int, int>>;
using Sequence = std::vector<Element>;

using pnet_value = pnet::type::value::value_type;
using pnet_list = std::list<pnet_value>;
using pnet_list2d = std::list<std::list<pnet_value>>;

using list_type = std::list<std::string>; // Define list_type as std::list<std::string>

pnet_list2d gen_block(int d, int n)
{
    pnet_list2d v;
    for (int e = 0; e < d; e++)
    {
        std::list<int> x(n, 0);
        x.front() = d - e;
        x.back() = e;

        std::list<pnet::type::value::value_type> temp;
        for (int xi : x)
        {
            temp.push_back(pnet::type::value::value_type(xi));
        }
        v.push_back(temp);
    }
    return v;
}
int binomial(int n, int k)
{
    if (k < 0 || k > n)
    {
        throw std::invalid_argument("Invalid arguments for binomial coefficient");
    }

    int result = 1;
    for (int i = 1; i <= k; ++i)
    {
        result *= (n - i + 1);
        result /= i;
    }
    return result;
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
pnet_list2d iterate(const std::vector<int> &xa)
{
    pnet_list2d vec;

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
        std::list<pnet::type::value::value_type> tempxv;
        for (int xi : xa)
        {
            tempxv.push_back(pnet::type::value::value_type(xi));
        }
        vec.push_back(tempxv);
    }

    int factorial = binomial(d + k - 1, d);

    int e = d - xa[0];
    std::vector<int> a = xa;
    std::vector<int> y(k, 0);
    y[0] = xa[0] - 1;
    y[k - 1] = e + 1;

    for (int i = 0; i < factorial; ++i)
    {
        if (a != y)
        {
            {
                a = next_partition(a);
            }
            std::list<pnet::type::value::value_type> temp_a;
            for (int xi : a)
            {
                temp_a.push_back(pnet::type::value::value_type(xi));
            }
            vec.push_back(temp_a);
        }
        else
        {
            break;
        }
    }

    return vec;
}

std::vector<std::tuple<int, std::vector<int>>> signature_and_multiplicitie(const std::vector<std::pair<int, int>> &G, const std::vector<int> &a)
{
    std::vector<int> p;
    std::vector<std::tuple<int, std::vector<int>>> b;
    std::unordered_set<int> nbv;
    for (const auto &e : G)
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

    for (const auto &ga : per)
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
    for (auto &tuple : b)
    {
        int &element = std::get<0>(tuple);
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

        for (const auto &pair1 : b)
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

            for (const auto &pair2 : b)
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
void constterm(const int k, const int j, mp_limb_signed_t N, fmpz_mpoly_t result, const fmpz_mpoly_ctx_t ctx)
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
        ulong exp[N]; // Exponent vector for N variables
        for (int idx = 0; idx < N; ++idx)
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

void proterm(const int k, const int j, int a, mp_limb_signed_t N, fmpz_mpoly_t result, const fmpz_mpoly_ctx_t ctx)
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
            ulong exp1[4] = {0};  // Exponent vector for the first term
            exp1[k - 1] = exp_k1; // Set exponent for variable k
            exp1[j - 1] = exp_j1; // Set exponent for variable j
            ulong exp2[4] = {0};  // Exponent vector for the second term
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

std::vector<int> fey_degree;

int feynman_integral_type(std::vector<std::pair<int, int>> Gv, std::tuple<int, std::vector<int>> factor, std::vector<int> av)
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
        for (const auto &e : Gv)
        {
            nbv.insert(e.first);
            nbv.insert(e.second);
        }
        int nv = nbv.size();
        // nb vertices.
        int N = std::accumulate(av.begin(), av.end(), 0, [](int sum, int val)
                                { return sum + (val > 0 ? val : 0); });
        int p = 0;
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

            for (const auto &multiplicity : av)
            {
                if (multiplicity == -1)
                {
                    fmpz_mpoly_t constterm1_j;
                    fmpz_mpoly_init(constterm1_j, ctx);
                    constterm(Gv[j].first, Gv[j].second, N, constterm1_j, ctx);
                    fmpz_mpoly_mul(tmp, tmp, constterm1_j, ctx);
                    fmpz_mpoly_clear(constterm1_j, ctx);
                }
                else if (multiplicity == 0)
                {
                    fmpz_mpoly_t constterm0_j;
                    fmpz_mpoly_init(constterm0_j, ctx);
                    constterm(Gv[j].second, Gv[j].first, N, constterm0_j, ctx);
                    fmpz_mpoly_mul(tmp, tmp, constterm0_j, ctx);
                    fmpz_mpoly_clear(constterm0_j, ctx);
                }
                else
                {
                    fmpz_mpoly_t proterm_j;
                    fmpz_mpoly_init(proterm_j, ctx);
                    proterm(Gv[j].first, Gv[j].second, multiplicity, N, proterm_j, ctx);
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
        fmpz_mpoly_scalar_mul_si(tmp, tmp, std::get<0>(factor), ctx);

        slong coeff;
        fmpz_t coeff_fmpz;
        fmpz_init(coeff_fmpz);
        fmpz_mpoly_get_fmpz(coeff_fmpz, tmp, ctx);
        coeff = fmpz_get_si(coeff_fmpz);
        fmpz_clear(coeff_fmpz);

        // Clear memory
        fmpz_mpoly_clear(tmp, ctx);
        fmpz_mpoly_ctx_clear(ctx);

        // Return the result
        p += coeff;
        return p;
    }
}

int main()
{
    std::vector<std::pair<int, int>> Gv = {{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}};
    std::vector<int> av = {-1, 0, 2, 2, 2, 2};

    auto operation = [&]()
    {
        return feynman_integral_type(Gv, std::make_tuple(16, std::vector<int>{}), av);
    };
    int result = operation();

    std::cout << "Result: " << result << std::endl;
    auto usage = measure_resource_usage(operation);

    // Print the resource usage
    std::cout << "Elapsed time: " << usage.elapsed_time << " microseconds" << std::endl;
    std::cout << "Memory usage: " << usage.memory_usage << " KiB" << std::endl;
    return 0;
}
