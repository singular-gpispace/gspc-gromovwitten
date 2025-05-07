#ifndef QUASI_HPP
#define QUASI_HPP

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

#endif // QUASI_HPP