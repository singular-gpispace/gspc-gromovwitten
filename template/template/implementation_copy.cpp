
#include <Singular/libsingular.h>


#include <interface/template_interface.hpp>
#include <interface/WorkflowResult.hpp>
#include <interface/ValuesOnPorts.hpp>
#include <interface/Workflow.hpp>
#include <stdexcept>
#include <unistd.h>

#include "config.hpp"
#include "singular_functions.hpp"
#include <chrono>
#include <tuple>
//#include "Singular/lists.h"
#include <typeinfo>

#include <stdio.h>
#include <flint/fmpz_mpoly.h>
#include <stdlib.h> // for malloc and free
#include <fstream>
#include <sys/resource.h>
#include <chrono>
#include <functional> // Include for std::function
#include <numeric>
#include <unordered_set> // for std::unordered_set
#include <algorithm>

#include <stdexcept>
#include <vector>
#include <flint/fmpq_mpoly.h>
#include <flint/fmpq_mat.h>
#include <flint/fmpq.h>
#include <flint/fmpq_poly.h>
#include <flint/arith.h>
#include <flint/fmpz.h>
#include <iostream>
#include <stdexcept>
#include <flint/flint.h>

using vector2d = std::vector<std::vector<int>>;
using graph = std::vector<std::pair<int, int>>;
using Edge = std::pair<int, int>;
using signature = std::vector<std::tuple<int, std::vector<int>>>;

void myctx_init(fmpq_mpoly_ctx_t ctx, slong nv)
{
  fmpq_mpoly_ctx_init(ctx, nv, ORD_DEGLEX);
}

void my_init(fmpq_mpoly_t res, fmpq_mpoly_ctx_t ctx)
{
  fmpq_mpoly_init(res, ctx);
}
void my_mpoly_pretty(fmpq_mpoly_t result, fmpq_mpoly_ctx_t ctx)
{
  fmpq_mpoly_print_pretty(result, NULL, ctx);
}
void my_ctx_clear(fmpq_mpoly_ctx_t ctx)
{
  fmpq_mpoly_ctx_clear(ctx);
}
void my_mpoly_clear(fmpq_mpoly_t result, fmpq_mpoly_ctx_t ctx)
{
  fmpq_mpoly_clear(result, ctx);
}


NO_NAME_MANGLING


std::string singular_template_compute_StdBasis(std::string const& input_filename
  , std::string const& needed_library
)
{
  init_singular(config::singularLibrary().string());
  load_singular_library(needed_library);
  std::pair<int, lists> input;
  std::pair<int, lists> out;
  std::string ids;
  std::string out_filename;
  std::string out_filename1;
  ids = worker();
  //std::cout << ids << " in singular_..._compute" << std::endl;
  input = deserialize(input_filename, ids);

  ScopedLeftv args(input.first, lCopy(input.second));
  //std::string function_name2 = "stdBasis";

  lists Token = (lists)(args.leftV()->data);

  int L_size = lSize(Token) + 1;
  for (int i = 0; i < L_size; i++) {
    sleftv& listElement = Token->m[i];  // Access each element as `leftv`
    if (listElement.data == NULL) {
      std::cout << "Input: NULL" << std::endl;
    }
    else if (i == 3) {
      out_filename1 = listElement.String();
      std::cout << "out_filename1= " << out_filename1 << std::endl;
    }
  }
  /*  // Example output filename for debug purposes
   std::cout << "Base filename: " << base_filename << std::endl;
   std::cout << "Output filename: " << out_filename1 << std::endl;
   std::cout << "Type of out_filename1: " << typeid(out_filename1).name() << std::endl; */

   //out = call_user_proc(function_name2, needed_library, args);
   //std::cout << "myout " << out.second << std::endl;
   //out_filename = serialize(out.second, base_filename);
   //std::cout << base_filename << std::endl;;


  return out_filename1;
}

NO_NAME_MANGLING

std::vector<std::string> singular_template_compute_parseInput(const std::string& input) {

  std::vector<std::string> result;
  std::string current;
  int braceCount = 0;

  for (char c : input) {
    if (c == '{') {
      braceCount++;
      current += c;
    }
    else if (c == '}') {
      braceCount--;
      current += c;
    }
    else if (c == ',' && braceCount == 0) {
      // Add current token to result if not inside braces
      if (!current.empty()) {
        result.push_back(current);
        current.clear();
      }
    }
    else {
      // Add character to current token
      current += c;
    }
  }

  // Add the last token
  if (!current.empty()) {
    result.push_back(current);
  }

  // Trim leading/trailing whitespace from each entry
  for (auto& entry : result) {
    entry.erase(0, entry.find_first_not_of(" \t"));
    entry.erase(entry.find_last_not_of(" \t") + 1);
  }

  return result;
}
NO_NAME_MANGLING
std::vector<std::pair<int, int>> extractIntegerPairs(const std::string& graph) {
  std::vector<std::pair<int, int>> Gv;
  std::string num;
  int first = 0;
  bool expectingSecond = false;

  for (char c : graph) {
    if (isdigit(c)) {
      num += c;  // Collect digits to form a number
    }
    else if (!num.empty()) {
      int value = std::stoi(num);  // Convert the collected number string to an integer
      num.clear();  // Clear the string for the next number

      if (expectingSecond) {
        Gv.emplace_back(first, value);
        expectingSecond = false;
      }
      else {
        first = value;
        expectingSecond = true;
      }
    }
  }
  return Gv;
}
NO_NAME_MANGLING
int number_monomial(int weightmax) {
  int count = 0;
  std::vector<int> w = { 2, 4, 6 };

  for (int e2 = 0; e2 <= weightmax; ++e2) {
    for (int e4 = 0; e4 <= weightmax; ++e4) {
      for (int e6 = 0; e6 <= weightmax; ++e6) {
        int degree = w[0] * e2 + w[1] * e4 + w[2] * e6;
        if (degree <= weightmax && degree > 0) {
          ++count;
        }
      }
    }
  }
  return count;
}
NO_NAME_MANGLING
unsigned long  binomial(const int n, const int k) {
  std::vector<unsigned long > vec(k);
  vec[0] = n - k + 1;

  for (int i = 1; i < k; ++i) {
    vec[i] = vec[i - 1] * (n - k + 1 + i) / (i + 1);
  }

  return vec[k - 1];
}
NO_NAME_MANGLING
unsigned long partialBinomialSum(int n, int d) {
  unsigned long sum = 0;

  for (int i = 2; i <= d; ++i) {
    sum += binomial(i + n - 1, i); // Calculate and add C(i+n-1, i)
  }

  return sum;
}
NO_NAME_MANGLING
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



// Convert fmpq_mpoly to std::string representation
std::string fmpq_mpolyToString(const fmpq_mpoly_t A, const fmpq_mpoly_ctx_t ctx) {
  const char* x[] = { "E2", "E4", "E6" };  // Variable names
  char* result_str = fmpq_mpoly_get_str_pretty(A, x, ctx); // Get string representation

  std::string result(result_str);  // Convert to std::string

  flint_free(result_str);  // Free the memory allocated by fmpq_mpoly_get_str_pretty

  return result;
}

// Function to compute sum of divisor powers
void sum_of_divisor_powers(fmpz_t result, const fmpz_t n, slong k) {
  fmpz_zero(result); // Initialize result to 0
  fmpz_t i, mod, power;
  fmpz_init(i);
  fmpz_init(mod);
  fmpz_init(power);

  // Loop through divisors of n
  for (fmpz_set_ui(i, 1); fmpz_cmp(i, n) <= 0; fmpz_add_ui(i, i, 1)) {
    fmpz_mod(mod, n, i);
    if (fmpz_is_zero(mod)) {
      // Calculate i^(k-1)
      fmpz_pow_ui(power, i, k);
      fmpz_add(result, result, power);
    }
  }

  // Clear allocated memory
  fmpz_clear(i);
  fmpz_clear(mod);
  fmpz_clear(power);
}
#include <flint/fmpq_poly.h>
#include <flint/fmpq.h>
#include <stdexcept>

// Eisenstein series function
void eisenstein_series(fmpq_poly_t result, int num_terms, int k) {
  if (k % 2 != 0) {
    throw std::invalid_argument("Input k must be even in eisenstein_series()");
  }

  // Initialize polynomial for q: q + q^2 + q^3 + ...
  fmpq_poly_t q_term, sum_term;
  fmpq_poly_init(q_term);
  fmpq_poly_init(sum_term);

  fmpq_poly_set_coeff_ui(q_term, 1, 1); // This initializes q_term to q (q^1)

  // Compute Bernoulli number B_k
  fmpq_t B_k;
  fmpq_init(B_k);
  arith_bernoulli_number(B_k, k);

  // Begin computing the Eisenstein series
  fmpq_poly_one(result); // Start with 1

  fmpq_t coefficient;
  fmpq_init(coefficient);

  fmpz_t d, divisor_sum;
  fmpz_init(d);
  fmpz_init(divisor_sum);

  for (int i = 1; i <= num_terms; i++) {
    // Set d to the current term
    fmpz_set_ui(d, i);

    // Compute the sum of divisor powers for d
    sum_of_divisor_powers(divisor_sum, d, k - 1);

    // Calculate coefficient: (2*k / B_k) * divisor_sum
    fmpq_set_fmpz(coefficient, divisor_sum);     // Set coefficient as the sum of divisor powers
    fmpq_mul_si(coefficient, coefficient, -2 * k); // Multiply by 2*k
    fmpq_div(coefficient, coefficient, B_k);     // Divide by Bernoulli number

    // Multiply q^(i) to get the q-term (instead of q^(2*i))
    fmpq_poly_scalar_mul_fmpq(sum_term, q_term, coefficient); // Multiply coefficient

    // Add the term to the result
    fmpq_poly_add(result, result, sum_term);

    // Update q_term to q^(i) by multiplying with q
    fmpq_poly_shift_left(q_term, q_term, 1); // Shifts the polynomial by multiplying by q (instead of q^2)
  }

  // Clear all variables
  fmpq_poly_clear(q_term);
  fmpq_poly_clear(sum_term);
  fmpq_clear(B_k);
  fmpq_clear(coefficient);
  fmpz_clear(d);
  fmpz_clear(divisor_sum);
}


// Function to express polynomials as powers of Eisenstein series
void express_as_powers(std::vector<fmpq_poly_t*>& result, int max_degree, int weightmax) {
  int nb = max_degree;

  // Initialize polynomials for E2, E4, and E6
  fmpq_poly_t E2, E4, E6;
  fmpq_poly_init(E2);
  fmpq_poly_init(E4);
  fmpq_poly_init(E6);

  // Compute Eisenstein series for E2, E4, and E6
  eisenstein_series(E2, nb, 2);  // E2
  eisenstein_series(E4, nb, 4);  // E4
  eisenstein_series(E6, nb, 6);  // E6

  // Temporary polynomials for intermediate calculations
  fmpq_poly_t temp1, temp2, product;
  fmpq_poly_init(temp1);
  fmpq_poly_init(temp2);
  fmpq_poly_init(product);

  // Loop through powers of E2, E4, and E6
  for (int e2 = 0; 2 * e2 <= weightmax; ++e2) {
    for (int e4 = 0; 4 * e4 <= weightmax; ++e4) {
      for (int e6 = 0; 6 * e6 <= weightmax; ++e6) {
        int degree = 2 * e2 + 4 * e4 + 6 * e6;

        if (degree <= weightmax && degree > 0) {
          // Calculate E2^e2
          if (e2 > 0) {
            fmpq_poly_pow(temp1, E2, e2);
          }
          else {
            fmpq_poly_one(temp1);  // E2^0 = 1
          }

          // Calculate E4^e4
          if (e4 > 0) {
            fmpq_poly_pow(temp2, E4, e4);
          }
          else {
            fmpq_poly_one(temp2);  // E4^0 = 1
          }

          // Multiply temp1 (E2^e2) with temp2 (E4^e4)
          fmpq_poly_mul(product, temp1, temp2);

          // Calculate E6^e6
          if (e6 > 0) {
            fmpq_poly_pow(temp1, E6, e6);
          }
          else {
            fmpq_poly_one(temp1);  // E6^0 = 1
          }

          // Multiply product (E2^e2 * E4^e4) with E6^e6
          fmpq_poly_mul(product, product, temp1);

          // Dynamically allocate a new polynomial for storing the result
          fmpq_poly_t* result_poly = (fmpq_poly_t*)malloc(sizeof(fmpq_poly_struct));
          fmpq_poly_init(*result_poly);
          fmpq_poly_set(*result_poly, product);

          // Push the pointer to the polynomial into the result vector
          result.push_back(result_poly);
        }
      }
    }
  }

  // Clean up temporary polynomials
  fmpq_poly_clear(temp1);
  fmpq_poly_clear(temp2);
  fmpq_poly_clear(product);
  fmpq_poly_clear(E2);
  fmpq_poly_clear(E4);
  fmpq_poly_clear(E6);
}

void polynomial_to_matrix(fmpq_mat_t A, const std::vector<fmpq_poly_t*>& polynomials) {
  // Determine the size of the matrix (rows = number of polynomials, columns = max degree of polynomials + 1)
  slong num_polynomials = polynomials.size();
  slong max_degree = 0;

  // Find the maximum degree among all polynomials
  for (slong i = 0; i < num_polynomials; i++) {
    slong deg = fmpq_poly_degree(*polynomials[i]);
    if (deg > max_degree) {
      max_degree = deg;
    }
  }

  // Initialize the matrix A with num_polynomials rows and max_degree+1 columns
  fmpq_mat_init(A, num_polynomials, max_degree + 1);

  // Fill the matrix A with coefficients from the polynomials
  for (slong i = 0; i < num_polynomials; i++) {
    for (slong j = 0; j <= fmpq_poly_degree(*polynomials[i]); j++) {
      fmpq_poly_get_coeff_fmpq(fmpq_mat_entry(A, i, j), *polynomials[i], j);
    }
  }
}

// Function to filter terms of a multi-variate polynomial based on variable powers
void filter_term(fmpq_mpoly_t result, const fmpq_mpoly_t poly, const std::vector<slong>& variables, const std::vector<int>& power, const fmpq_mpoly_ctx_t ctx) {
  fmpq_t coeff;
  fmpq_init(coeff);

  fmpq_mpoly_zero(result, ctx);  // Initialize result to zero polynomial

  ulong* exps = (ulong*)malloc(fmpq_mpoly_ctx_nvars(ctx) * sizeof(ulong));  // Allocate array for exponents

  slong num_terms = fmpq_mpoly_length(poly, ctx);
  for (slong i = 0; i < num_terms; ++i) {
    fmpq_mpoly_get_term_exp_ui(exps, poly, i, ctx);

    bool within_power = true;
    for (size_t j = 0; j < variables.size(); ++j) {
      if (static_cast<int>(exps[variables[j]]) > power[j]) {
        within_power = false;
        break;
      }
    }

    if (within_power) {
      fmpq_mpoly_get_coeff_fmpq_ui(coeff, poly, exps, ctx);  // Get coefficient for the term
      fmpq_mpoly_set_coeff_fmpq_ui(result, coeff, exps, ctx);  // Set coefficient in the result
    }
  }

  fmpq_clear(coeff);  // Clear temporary coefficient
  free(exps);  // Free dynamically allocated array
}

// Function to filter terms of a univariate polynomial based on degree
void filter_term(fmpq_poly_t result, const fmpq_poly_t poly, int max_degree) {
  fmpq_poly_zero(result);  // Initialize result to zero polynomial

  for (slong i = 0; i <= fmpq_poly_degree(poly); ++i) {
    if (i <= max_degree) {
      fmpq_t coeff;
      fmpq_init(coeff);
      fmpq_poly_get_coeff_fmpq(coeff, poly, i);
      fmpq_poly_set_coeff_fmpq(result, i, coeff);
      fmpq_clear(coeff);
    }
  }
}

// Function to filter a vector of univariate polynomials based on degree
void filter_vector(std::vector<fmpq_poly_t*>& result, const std::vector<fmpq_poly_t*>& polyvector, int max_degree) {
  // Clear the result vector to start fresh
  result.clear();

  // Loop through each polynomial in the input vector
  for (const auto& poly : polyvector) {
    // Create a new polynomial for storing the filtered result
    fmpq_poly_t* filtered_poly = (fmpq_poly_t*)malloc(sizeof(fmpq_poly_struct));
    fmpq_poly_init(*filtered_poly);

    // Apply the filtering function
    filter_term(*filtered_poly, *poly, max_degree);

    // Push the result into the result vector
    result.push_back(filtered_poly);
  }
}

// Function to print the polynomial
void print_poly(const fmpq_mpoly_t poly, const fmpq_mpoly_ctx_t ctx) {
  fmpq_mpoly_print_pretty(poly, nullptr, ctx);  // Use nullptr for default variable names (x1, x2, x3, ...)
  printf("\n");  // Add a new line after printing the polynomial
}


// Function to convert coefficients from the Iq vector to a matrix representation
void matrix_of_integral(const std::vector<unsigned long>& Iq, fmpq_mat_t& Q_matrix) {
  // Get the number of coefficients in the Iq vector
  slong num_coeffs = Iq.size();

  // Initialize the matrix: a 1-row matrix with num_coeffs + 1 columns
  fmpq_mat_init(Q_matrix, 1, num_coeffs + 1);

  // Fill the first entry with zero
  fmpq_set_ui(fmpq_mat_entry(Q_matrix, 0, 0), 0, 1); // Set the first column to 0

  // Fill the rest of the matrix with coefficients from the Iq vector
  for (slong i = 0; i < num_coeffs; ++i) {
    fmpq_t coeff;
    fmpq_init(coeff);  // Initialize a rational number

    // Set the coefficient as an integer (since Iq contains unsigned long values)
    fmpq_set_ui(coeff, Iq[i], 1);  // Set as Iq[i] / 1 (since it's an integer)

    // Set the coefficient in the matrix
    fmpq_set(fmpq_mat_entry(Q_matrix, 0, i + 1), coeff); // Offset by 1 for the leading zero

    // Clear the temporary coefficient
    fmpq_clear(coeff);
  }
}


bool solve_polynomial_system(const fmpq_mat_t A, const fmpq_mat_t B, fmpq_mat_t X) {
  std::cout << "Checking if the system can be solved using Dixon's method..." << std::endl;

  // Attempt to solve the system using Dixon's method
  if (fmpq_mat_can_solve_dixon(X, A, B) != 0) {
    std::cout << "System can be solved. Attempting to solve..." << std::endl;

    // Since Dixon's method sets X with the solution, we can just return true
    return true;
  }
  else {
    std::cerr << "System cannot be solved." << std::endl;
    return false;
  }
}



// Function to express polynomials as powers of Eisenstein series
void express_as_eisenstein_series(std::vector<fmpq_mpoly_t*>& result, slong weightmax, fmpq_mpoly_ctx_t ctx) {
  // Initialize Eisenstein series polynomials
  fmpq_mpoly_t E1, E2, E3;
  fmpq_mpoly_init(E1, ctx);
  fmpq_mpoly_init(E2, ctx);
  fmpq_mpoly_init(E3, ctx);

  // Set E1, E2, and E3 as distinct variables (x1, x2, x3)
  ulong exp_E1[3] = { 1, 0, 0 };  // E1 corresponds to x1
  ulong exp_E2[3] = { 0, 1, 0 };  // E2 corresponds to x2
  ulong exp_E3[3] = { 0, 0, 1 };  // E3 corresponds to x3

  fmpq_t coeff;
  fmpq_init(coeff);
  fmpq_set_si(coeff, 1, 1);  // Coefficient 1

  // Set coefficients for E1, E2, and E3 as distinct variables
  fmpq_mpoly_set_coeff_fmpq_ui(E1, coeff, exp_E1, ctx);  // Set E1 as x1
  fmpq_mpoly_set_coeff_fmpq_ui(E2, coeff, exp_E2, ctx);  // Set E2 as x2
  fmpq_mpoly_set_coeff_fmpq_ui(E3, coeff, exp_E3, ctx);  // Set E3 as x3

  // Loop over powers of E1, E2, and E3
  for (slong e1 = 0; e1 <= weightmax; ++e1) {
    for (slong e2 = 0; e2 <= weightmax; ++e2) {
      for (slong e3 = 0; e3 <= weightmax; ++e3) {
        slong degree = 2 * e1 + 4 * e2 + 6 * e3;

        if (degree <= weightmax && degree > 0) {
          // Initialize a new term on the heap
          fmpq_mpoly_t* term = (fmpq_mpoly_t*)malloc(sizeof(fmpq_mpoly_struct));
          fmpq_mpoly_init(*term, ctx);
          fmpq_mpoly_set_ui(*term, 1, ctx);  // Set term = 1

          // Multiply term by E1^e1 if e1 > 0
          if (e1 > 0) {
            fmpq_mpoly_pow_ui(*term, E1, e1, ctx);  // Set term = E1^e1
          }

          // Multiply term by E2^e2 if e2 > 0
          if (e2 > 0) {
            fmpq_mpoly_t temp_E2;
            fmpq_mpoly_init(temp_E2, ctx);
            fmpq_mpoly_pow_ui(temp_E2, E2, e2, ctx);
            fmpq_mpoly_mul(*term, *term, temp_E2, ctx);  // Multiply term by E2^e2
            fmpq_mpoly_clear(temp_E2, ctx);  // Clear temp_E2
          }

          // Multiply term by E3^e3 if e3 > 0
          if (e3 > 0) {
            fmpq_mpoly_t temp_E3;
            fmpq_mpoly_init(temp_E3, ctx);
            fmpq_mpoly_pow_ui(temp_E3, E3, e3, ctx);
            fmpq_mpoly_mul(*term, *term, temp_E3, ctx);  // Multiply term by E3^e3
            fmpq_mpoly_clear(temp_E3, ctx);  // Clear temp_E3
          }

          // Push the pointer to the polynomial into the result vector
          result.push_back(term);
        }
      }
    }
  }

  // Clear allocated memory for Eisenstein series
  fmpq_clear(coeff);
  fmpq_mpoly_clear(E1, ctx);
  fmpq_mpoly_clear(E2, ctx);
  fmpq_mpoly_clear(E3, ctx);
}
// Function to compute the GCD of fmpq_t coefficients
void gcd(const std::vector<fmpq_t*>& coeffs, fmpq_t& common_factor) {
  fmpq_set_ui(common_factor, 1, 1); // Start with 1

  for (const auto& coeff : coeffs) {
    if (fmpq_is_zero(*coeff)) continue; // Skip zero coefficients
    fmpq_gcd(common_factor, common_factor, *coeff); // Update GCD
  }
}

// Function to compute the rational solution matrix with Iq as a vector of unsigned long
void quasi_matrix(std::vector<fmpq_t*>& result, const std::vector<unsigned long>& Iq, int weightmax) {
  slong max_degree = Iq.size();  // Use the size of the Iq vector as the degree

  // Vector of fmpq_poly_t for the Eisenstein series or other polynomials
  std::vector<fmpq_poly_t*> Evector;
  express_as_powers(Evector, max_degree, weightmax);  // Modify this according to your logic

  std::vector<fmpq_poly_t*> filtered_Evector;
  filter_vector(filtered_Evector, Evector, max_degree);

  // Convert polynomials into matrices
  fmpq_mat_t A;
  polynomial_to_matrix(A, filtered_Evector);

  // Create matrix Q from the Iq vector
  fmpq_mat_t Q;
  matrix_of_integral(Iq, Q); // Fill Q with the values from Iq

  // Transpose matrices A and Q
  fmpq_mat_t A_transposed, Q_transposed;
  fmpq_mat_init(A_transposed, fmpq_mat_ncols(A), fmpq_mat_nrows(A));
  fmpq_mat_transpose(A_transposed, A);

  fmpq_mat_init(Q_transposed, fmpq_mat_ncols(Q), fmpq_mat_nrows(Q));
  fmpq_mat_transpose(Q_transposed, Q);
  /*
      // Print matrix dimensions for debugging
      std::cout << "Dimensions of A: " << fmpq_mat_nrows(A) << " x " << fmpq_mat_ncols(A) << std::endl;
      std::cout << "Dimensions of Q: " << fmpq_mat_nrows(Q) << " x " << fmpq_mat_ncols(Q) << std::endl; */

      // Check matrix dimensions
  if (fmpq_mat_nrows(A_transposed) != fmpq_mat_nrows(Q_transposed)) {
    std::cerr << "Error: Dimensions of A and Q do not match.\n";
    fmpq_mat_clear(A);
    fmpq_mat_clear(Q);
    fmpq_mat_clear(A_transposed);
    fmpq_mat_clear(Q_transposed);
    return;
  }

  // Solve the system
  fmpq_mat_t X;
  fmpq_mat_init(X, fmpq_mat_ncols(A_transposed), fmpq_mat_ncols(Q_transposed));
  bool solvable = solve_polynomial_system(A_transposed, Q_transposed, X);

  if (solvable) {
    // Clear previous results and resize the result vector
    result.clear();
    result.resize(fmpq_mat_nrows(X));  // Resize result vector to match the number of rows in X

    // Collect the solutions into a new vector for GCD calculation
    std::vector<fmpq_t*> new_coeffs;
    for (slong i = 0; i < fmpq_mat_nrows(X); ++i) {
      fmpq_t* coeff = (fmpq_t*)malloc(sizeof(fmpq_t)); // Allocate memory for each coeff
      fmpq_init(*coeff);
      fmpq_set(*coeff, fmpq_mat_entry(X, i, 0));
      new_coeffs.push_back(coeff); // Store the pointer in the vector
    }

    // Calculate the common factor (GCD)
    fmpq_t common_factor;
    fmpq_init(common_factor);
    gcd(new_coeffs, common_factor); // Compute GCD

    /* // Output the GCD for debugging
    std::cout << "Common GCD: ";
    fmpq_print(common_factor);
    std::cout << std::endl;
*/
// Normalize the results
    for (size_t i = 0; i < new_coeffs.size(); ++i) {
      if (!fmpq_is_zero(*new_coeffs[i])) {
        fmpq_div(*new_coeffs[i], *new_coeffs[i], common_factor);  // Normalize by dividing by GCD
      }
      result[i] = new_coeffs[i]; // Assign normalized values to the result
    }

    /*    // Print the normalized solution
       std::cout << "Normalized solution vector:\n";
       for (const auto& r : result) {
           fmpq_print(*r);
           std::cout << std::endl;
       } */

       // Clear the allocated coefficients
    for (auto& coeff : new_coeffs) {
      fmpq_clear(*coeff); // Clear the fmpq_t
    }
    // Clear the GCD
    fmpq_clear(common_factor); // Clear the GCD
  }
  else {
    std::cerr << "The system could not be solved.\n";
  }

  // Clear matrices
  fmpq_mat_clear(A);
  fmpq_mat_clear(Q);
  fmpq_mat_clear(A_transposed);
  fmpq_mat_clear(Q_transposed);
  fmpq_mat_clear(X);

  // Clear polynomials
  for (auto& poly : Evector) {
    fmpq_poly_clear(*poly); // Clear the polynomial
    delete poly; // Use delete instead of free, as it was allocated using new
  }
  for (auto& poly : filtered_Evector) {
    fmpq_poly_clear(*poly); // Clear the polynomial
    delete poly; // Use delete instead of free
  }
}

// Function to compute the quasimodular form
void quasimodular_form(fmpq_mpoly_t temp, const std::vector<unsigned long>& Iq, int weightmax, fmpq_mpoly_ctx_t ctx) {
  // Step 1: Get the coefficients using quasi_matrix
  std::vector<fmpq_t*> coef; // Vector to store coefficients
  quasi_matrix(coef, Iq, weightmax);  // Populate coef with coefficients


  // Step 2: Get the Eisenstein series expressions
  std::vector<fmpq_mpoly_t*> comb_result; // Vector of fmpq_mpoly_t pointers
  express_as_eisenstein_series(comb_result, weightmax, ctx); // Populate comb_result with Eisenstein series

  // Step 3: Initialize the result polynomial
  fmpq_mpoly_init(temp, ctx); // Initialize the result polynomial

  // Step 4: Compute the sum of the products of coefficients and terms
  fmpq_mpoly_t tmp; // Temporary polynomial to hold the product
  fmpq_mpoly_init(tmp, ctx); // Initialize the temporary polynomial

  for (size_t i = 0; i < comb_result.size(); ++i) {
    if (fmpq_is_zero(*coef[i])) {
      continue; // Skip if coefficient is zero
    }

    // Compute tmp = coef[i] * term
    fmpq_mpoly_scalar_mul_fmpq(tmp, *comb_result[i], *coef[i], ctx);

    // Add tmp to the result polynomial temp
    fmpq_mpoly_add(temp, temp, tmp, ctx);
  }

  // Clear the temporary polynomial
  fmpq_mpoly_clear(tmp, ctx);

  // Clear coefficients
  for (auto& c : coef) {
    fmpq_clear(*c);
  }
}
/*
int main() {
    // Initialize Iq as a vector of unsigned long with the provided values
    std::vector<unsigned long> Iq = { 0, 24, 192, 720, 1920, 4320 };
    std::vector<unsigned long> Iq6 = {  0, 32, 1792, 25344, 182272, 886656, 3294720, 10246144, 27353088, 66497472, 145337600, 302347264, 577972224, 1079026432, 1875116544, 3233267712, 5225373696, 8490271392, 12961886976, 20067375616, 29331341312, 43646419584, 61425005056 };
 std::vector<unsigned long> Ij ={ 0, 0, 1152, 20736, 165888, 843264, 3255552, 10285056, 28035072, 68594688, 152150400, 317058048, 612956160, 1145207808, 2005675776, 3459760128, 5629741056, 9136115712, 14037577344, 21710052864, 31871766528, 47407680000, 67007616768};
   // Maximum weight
    int weightmax = 12;


    slong nv = 3;  // Number of variables
    fmpq_mpoly_ctx_t ctx;
    fmpq_mpoly_ctx_init(ctx, nv, ORD_DEGLEX);


    // Variable to hold the result
    fmpq_mpoly_t result;
    fmpq_mpoly_init(result, ctx);

    // Call the quasimodular_form function with Iq and weightmax
    quasimodular_form(result, Iq6, weightmax, ctx);
    // Print the result
    std::cout << "Quasimodular form result: ";
    fmpq_mpoly_print_pretty(result, NULL, ctx);
    std::cout << std::endl;

    // Clear polynomials

    fmpq_mpoly_clear(result, ctx);

    // Clear context
    fmpq_mpoly_ctx_clear(ctx);

    return 0;
}  */


std::string updateAndConvertVector(std::vector<unsigned long>& v, const std::string& s) {
  std::stringstream ss(s);
  unsigned long i, fey;
  unsigned long z = 0;

  // Parse the string "i fey" (space-separated)
  ss >> i >> fey;

  // Ensure the vector has enough space
  if (i >= v.size()) {
    v.resize(i, z); // Resize and initialize new elements to z (0 in this case)
  }

  // Update v[i-1] by adding fey to it
  v[i - 1] += fey;

  // Convert the vector back to a string
  std::stringstream result;
  for (unsigned long val : v) {
    result << val << ' ';
  }

  return result.str(); // Return the updated vector as a string
}


// Function to parse "i fey" (space-separated) and update the vector
void updateVectorFromString(std::vector<unsigned long>& v, const std::string& s) {
  std::stringstream ss(s);
  unsigned long i, fey;
  unsigned long z = 0;

  // Parse the string "i fey" (space-separated)
  ss >> i >> fey;

  // Ensure the vector has enough space
  if (i >= v.size()) {
    v.resize(i, z); // Resize and initialize new elements to z (0 in this case)
  }

  // Update v[i] by adding fey to it
  v[i - 1] += fey;
}

// Add two vectors
std::vector<unsigned long> sumOfVectors(const std::vector<unsigned long>& v1, const std::vector<unsigned long>& v2) {
  std::vector<unsigned long> result(std::max(v1.size(), v2.size()), 0);

  for (size_t i = 0; i < result.size(); ++i) {
    if (i < v1.size()) {
      result[i] += v1[i];
    }
    if (i < v2.size()) {
      result[i] += v2[i];
    }
  }

  return result;
}
std::vector<unsigned long> stringToVectorUlong(const std::string& str) {
  std::vector<unsigned long> result;
  std::stringstream ss(str);
  unsigned long num;  // Corrected to use unsigned long
  while (ss >> num) {
    result.push_back(num);
  }
  return result;
}

NO_NAME_MANGLING
std::string vectorToStringULong(const std::vector<unsigned long>& vec) {
  std::stringstream ss;
  for (unsigned long val : vec) {
    ss << val << ' ';
  }
  return ss.str();
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
NO_NAME_MANGLING
vector2d iterate(std::vector<int> xa)
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

  unsigned long factorial = binomial(d + k - 1, d);

  int e = d - xa[0];
  std::vector<int> a = xa;
  std::vector<int> y(k, 0);
  y[0] = xa[0] - 1;
  y[k - 1] = e + 1;

  for (unsigned long i = 0; i < factorial; ++i)
  {
    if (a != y)
    {
      {
        a = next_partition(a);
      }

      gen.push_back(a);
    }
    else
    {
      break;
    }
  }
  return gen;
}
NO_NAME_MANGLING

std::vector<std::tuple<int, std::vector<int>>> signature_and_multiplicitie(std::vector<std::pair<int, int>> G, std::vector<int> a)
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
    size_t j = 0;
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

unsigned long feynman_integral_type(std::vector<std::pair<int, int>> Gv, int factor, std::vector<int> av)
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
NO_NAME_MANGLING
unsigned long feynman_integral_branch_type(std::vector<std::pair<int, int>> Gv, std::vector<int> a)
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
NO_NAME_MANGLING
unsigned long feynman_integral_degree(std::vector<std::pair<int, int>> Gv, int d)
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

std::vector<unsigned long>  feynman_integral_degree_sum(std::vector<std::pair<int, int>> Gv, int d)
{
  std::vector<unsigned long> sum;

  for (int i = 1;i <= d; i++)
  {
    sum.push_back(feynman_integral_degree(Gv, i));
  }

  return sum;
}
