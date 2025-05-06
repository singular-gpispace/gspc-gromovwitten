#ifndef FEYNMAN_HPP
#define FEYNMAN_HPP

#include <list>
#include <tuple>
#include <sstream> // for std::istringstream
/* #include "../../../code/boost/include/boost/variant.hpp"
#include "../../../code/util-generic/split.hpp"
#include "../../../code/we/type/value.hpp" */
#include <stdio.h>
#include <flint/fmpz_mpoly.h>
#include <flint/fmpq.h>
#include <flint/fmpq_mat.h>
#include <flint/fmpq_poly.h>
#include <flint/fmpq_mpoly.h>  // Add this for rational polynomial functions
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

// String and vector conversion functions
std::string vectorToStringULong(const std::vector<unsigned long>& vec);
std::vector<unsigned long> stringToVectorUlong(const std::string& str);
std::vector<unsigned long> sumOfVectors(const std::vector<unsigned long>& v1, const std::vector<unsigned long>& v2);
void updateVectorFromString(std::vector<unsigned long>& v, const std::string& s);
std::string updateAndConvertVector(std::vector<unsigned long>& v, const std::string& s);

// FLINT rational number conversion functions
std::string vectorToStringFmpq(const std::vector<fmpq_t*>& vec);
std::vector<fmpq_t*> stringToVectorFmpq(const std::string& str);
std::vector<fmpq_t*> sumOfVectorsFmpq(const std::vector<fmpq_t*>& v1, const std::vector<fmpq_t*>& v2);

// FLINT rational polynomial wrapper functions
void feynman_fmpq_mpoly_ctx_init(fmpq_mpoly_ctx_t ctx, slong nvars, const ordering_t ord);
void feynman_fmpq_mpoly_init(fmpq_mpoly_t poly, const fmpq_mpoly_ctx_t ctx);
void feynman_fmpq_mpoly_clear(fmpq_mpoly_t poly, const fmpq_mpoly_ctx_t ctx);
void feynman_fmpq_mpoly_ctx_clear(fmpq_mpoly_ctx_t ctx);
int feynman_fmpq_mpoly_print_pretty(const fmpq_mpoly_t poly, const char ** x, const fmpq_mpoly_ctx_t ctx);
std::string feynman_fmpq_mpolyToString(const fmpq_mpoly_t poly, const fmpq_mpoly_ctx_t ctx);
void feynman_quasimodular_form(fmpq_mpoly_t result, const std::vector<fmpq_t*>& Iq, int weightmax, const fmpq_mpoly_ctx_t ctx);

// Basic functions
vector2d gen_block(int d, int n);
int binomial(int n, int k);
std::vector<int> next_partition(std::vector<int> a);
vector2d iterate(std::vector<int> xa);
 int number_monomial(int weightmax);
std::vector<std::tuple<int, std::vector<int>>> signature_and_multiplicitie(std::vector<std::pair<int, int>> G, const std::vector<int> a);

// Feynman integral functions
void constterm(const int k, const int j, mp_limb_signed_t N, const int nv, fmpz_mpoly_t result, const fmpz_mpoly_ctx_t ctx);
void proterm(const int k, const int j, int a, mp_limb_signed_t N, const int nv, fmpz_mpoly_t result, const fmpz_mpoly_ctx_t ctx);
unsigned long feynman_integral_type(std::vector<std::pair<int, int>> Gv, int factor, std::vector<int> av);

// Different signatures of feynman_integral_branch_type
unsigned long feynman_integral_branch_type(std::vector<std::pair<int, int>> Gv, std::vector<int> a);
void feynman_integral_branch_type(fmpq_t myfey, graph& Gv, const std::vector<int>& av, const std::vector<int>& g = std::vector<int>(), const std::vector<int>& l = std::vector<int>());
unsigned long feynman_integral_degree(std::vector<std::pair<int, int>> Gv, int d);

// Double edges functions
int nv(const std::vector<std::pair<int, int>>& ve);
std::vector<std::vector<int>> find_equal_pairs(const std::vector<std::pair<int, int>>& ve);
std::string vector_to_monomial(const graph& G, const std::vector<int>& v);
std::vector<std::vector<int>> generate_permutation(const std::vector<int>& l, const std::vector<std::vector<int>>& indices);
vector2d iterate_permutation(const graph& ve, const std::vector<int>& a);

// Functions from feynman_sum_psi.cpp
std::vector<int> extractIntegers(const std::string& graph);
unsigned long partialBinomialSum(int n, int d);
std::vector<int> stringToVectorInt(const std::string& str);
int sumVector(const std::vector<int>& vec);

// FLINT rational number functions
void fmpq_init(fmpq_t x);
void fmpq_clear(fmpq_t x);
void fmpq_add(fmpq_t res, const fmpq_t op1, const fmpq_t op2);
void fmpq_mul_ui(fmpq_t res, const fmpq_t op, unsigned long c);
std::string fmpqToString(const fmpq_t x);
void stringToFmpq(fmpq_t res, const std::string& str);

std::vector<fmpq_t*> stringToVectorFmpq(const std::string& str);
std::vector<fmpq_t*> sumOfVectorsFmpq(const std::vector<fmpq_t*>& v, const std::vector<fmpq_t*>& w);
std::string vectorToStringFmpq(const std::vector<fmpq_t*>& vec);
// Function declarations
std::string fmpq_mpolyToString(const fmpq_mpoly_t A, const fmpq_mpoly_ctx_t ctx);

int number_monomial(int weightmax);

// Function to compute sum of divisor powers
void sum_of_divisor_powers(fmpz_t result, const fmpz_t n, slong k);

// Eisenstein series function
void eisenstein_series(fmpq_poly_t result, int num_terms, int k);

// Function to express polynomials as powers of Eisenstein series
void express_as_powers(std::vector<fmpq_poly_t*>& result, int max_degree, int weightmax);

// Function to convert a vector of polynomials to a matrix
void polynomial_to_matrix(fmpq_mat_t A, const std::vector<fmpq_poly_t*>& polynomials);

// Function to filter terms of a multi-variate polynomial
void filter_term(fmpq_mpoly_t result, const fmpq_mpoly_t poly, const std::vector<slong>& variables, const std::vector<int>& power, const fmpq_mpoly_ctx_t ctx);

// Function to filter terms of a univariate polynomial
void filter_term(fmpq_poly_t result, const fmpq_poly_t poly, int max_degree);

// Function to filter a vector of univariate polynomials
void filter_vector(std::vector<fmpq_poly_t*>& result, const std::vector<fmpq_poly_t*>& polyvector, int max_degree);

// Function to print a polynomial
void print_poly(const fmpq_mpoly_t poly, const fmpq_mpoly_ctx_t ctx);

// Function to obtain coefficients of a univariate polynomial
std::vector<fmpq_t> coefficients_of_univariate(const fmpq_poly_t& poly);

// Function to convert coefficients to a matrix representation
void matrix_of_integral(const std::vector<fmpq_t*>& Iq, fmpq_mat_t& Q_matrix);

// Function to solve a polynomial system
bool solve_polynomial_system(const fmpq_mat_t A, const fmpq_mat_t B, fmpq_mat_t X);

// Function to compute the quasi-modular matrix
void quasi_matrix(std::vector<fmpq_t*>& result, const std::vector<fmpq_t*>& Iq, int weightmax);

// Function to express polynomials as powers of Eisenstein series in multivariate form
void express_as_eisenstein_series(std::vector<fmpq_mpoly_t*>& result, slong weightmax, fmpq_mpoly_ctx_t ctx);

//
void gcd(const std::vector<fmpq_t*>& coeffs, fmpq_t& common_factor);

//
void quasimodular_form(fmpq_mpoly_t result, const std::vector<fmpq_t*>& Iq, int weightmax, fmpq_mpoly_ctx_t ctx);

std::string fmpqToString(const fmpq_t f);
void stringToFmpq(fmpq_t f, const std::string& str);
std::string vectorToStringInt(const std::vector<int>& vec);
std::vector<int> stringToVectorInt(const std::string& str);
vector2d gen_block(int d, int n);
std::vector<int> next_partition(std::vector<int> a);
vector2d iterate(std::vector<int> xa);
std::vector<std::vector<int>> find_equal_pairs(const std::vector<std::pair<int, int>>& ve);
std::vector<std::vector<int>> generate_permutation(const std::vector<int>& l, const std::vector<std::vector<int>>& indices);
vector2d iterate_permutation(const graph& ve, const std::vector<int>& a);
std::vector<std::tuple<int, std::vector<int>>> signature_and_multiplicitie(std::vector<std::pair<int, int>> G, const std::vector<int> a);

void constterm(const int k, const int j, mp_limb_signed_t N, const int nv, fmpz_mpoly_t result, const fmpz_mpoly_ctx_t ctx);
void proterm(const int k, const int j, int a, mp_limb_signed_t N, const int nv, fmpz_mpoly_t result, const fmpz_mpoly_ctx_t ctx);

void invsfunction(const int j, const int aa, fmpq_mpoly_t result, const fmpq_mpoly_ctx_t ctx);
void sfunction(const int w, const int j, const int aa, fmpq_mpoly_t result, const fmpq_mpoly_ctx_t ctx);
void loopterm(const int  z, const int a, const int aa, fmpq_mpoly_t p, const fmpq_mpoly_ctx_t ctx);
void constterm(const int k, const int j, const int i1, const int i2, mp_limb_signed_t N, const int aa, fmpq_mpoly_t result, const fmpq_mpoly_ctx_t ctx);
void proterm(const int k, const int j, const int i1, const int i2, const int a, const int aa, mp_limb_signed_t N, fmpq_mpoly_t result, const fmpq_mpoly_ctx_t ctx);
void get_coefficient(fmpq_mpoly_t coeff, const fmpq_mpoly_t poly, const mp_limb_signed_t* var_indices, const mp_limb_t* exps, mp_limb_signed_t length, const fmpq_mpoly_ctx_t ctx);
void filter_terms(fmpq_mpoly_t result, const fmpq_mpoly_t poly, const std::vector<slong>& variables, const std::vector<int>& power, const fmpq_mpoly_ctx_t ctx);

unsigned long feynman_integral_type(std::vector<std::pair<int, int>> Gv, int factor, std::vector<int> av);
unsigned long feynman_integral_branch_type(std::vector<std::pair<int, int>> Gv, std::vector<int> a);
unsigned long feynman_integral_degree(std::vector<std::pair<int, int>> Gv, int d);

void feynman_integral_degrees(fmpq_t result, std::vector<std::pair<int, int>>& Gv, const int& d, const std::vector<int>& g, const std::vector<int>& l);

#endif // FEYNMAN_HPP