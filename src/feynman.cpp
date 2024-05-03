
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

    return {memory_usage, elapsed_time};
}

int feynman(std::vector<std::pair<int, int>> Gv, std::vector<int> av)
{
    // Count the number of unique vertices
    std::unordered_set<int> nbv;
    for (const auto &e : Gv)
    {
        nbv.insert(e.first);
        nbv.insert(e.second);
    }
    int nv = nbv.size();

    int factor = 2;
    int N = std::accumulate(av.begin(), av.end(), 0);

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
    fmpz_mpoly_scalar_mul_si(tmp, tmp, factor, ctx);

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
    return coeff;
}
