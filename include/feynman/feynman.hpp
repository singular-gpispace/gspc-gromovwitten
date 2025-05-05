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
/*
using pnet_value = pnet::type::value::value_type;
using pnet_list = std::list<pnet_value>;
using pnet_list2d = std::list<std::list<pnet_value>>; */
using vector2d = std::vector<std::vector<int>>;
using list_type = std::list<std::string>; // Define list_type as std::list<std::string>


#include <iostream>
#include <stdexcept>
#include <vector>
#include <flint/fmpq_mpoly.h>
#include <flint/fmpq_mat.h>
#include <flint/fmpq.h>
#include <flint/fmpq_poly.h>
#include <flint/arith.h>
#include <flint/fmpz.h>
#include <flint/flint.h>

std::vector<unsigned long> sumOfVectors(const std::vector<unsigned long>& v1, const std::vector<unsigned long>& v2);
std::string updateAndConvertVector(std::vector<unsigned long>& v, const std::string& s);
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
void matrix_of_integral(const fmpq_poly_t& Iq, fmpq_mat_t& Q_matrix);

// Function to solve a polynomial system
bool solve_polynomial_system(const fmpq_mat_t A, const fmpq_mat_t B, fmpq_mat_t X);

// Function to compute the quasi-modular matrix
void quasi_matrix(std::vector<fmpq_t*>& result, const std::vector<unsigned long>& Iq, int weightmax);

// Function to express polynomials as powers of Eisenstein series in multivariate form
void express_as_eisenstein_series(std::vector<fmpq_mpoly_t*>& result, slong weightmax, fmpq_mpoly_ctx_t ctx);

//
void gcd(const std::vector<fmpq_t*>& coeffs, fmpq_t& common_factor);

//
void quasimodular_form(fmpq_mpoly_t temp, const std::vector<unsigned long>& Iq, int weightmax, fmpq_mpoly_ctx_t ctx);

void updateVectorFromString(std::vector<unsigned long>& v, const std::string& s);
std::vector<unsigned long> stringToVectorUlong(const std::string& str);
std::string vectorToStringULong(const std::vector<unsigned long>& vec);
vector2d gen_block(int d, int n);
int binomial(int n, int k);
std::vector<int> next_partition(std::vector<int> a);
vector2d iterate(std::vector<int> xa);
std::vector<std::tuple<int, std::vector<int>>> signature_and_multiplicitie(std::vector<std::pair<int, int>> G, const std::vector<int> a);

void constterm(const int k, const int j, mp_limb_signed_t N, const int nv, fmpz_mpoly_t result, const fmpz_mpoly_ctx_t ctx);
void proterm(const int k, const int j, int a, mp_limb_signed_t N, const int nv, fmpz_mpoly_t result, const fmpz_mpoly_ctx_t ctx);
unsigned long feynman_integral_type(std::vector<std::pair<int, int>> Gv, int factor, std::vector<int> av);
unsigned long feynman_integral_branch_type(std::vector<std::pair<int, int>> Gv, std::vector<int> a);
unsigned long feynman_integral_degree(std::vector<std::pair<int, int>> Gv, int d);
std::string vectorToString(const std::vector<std::string>& vec, const std::string& delimiter = "");
#endif // FEYNMAN_HPP