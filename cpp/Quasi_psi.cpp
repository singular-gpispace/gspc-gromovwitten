
#include <unistd.h>
#include <chrono>
#include <tuple>
#include <set>
#include <map>
#include <sstream>
#include <stdio.h>
#include <flint/fmpz_mpoly.h>
#include <stdlib.h> // for malloc and free² ²  
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
#include <flint/bernoulli.h>

using vector2d = std::vector<std::vector<int>>;
using graph = std::vector<std::pair<int, int>>;
using Edge = std::pair<int, int>;
using signature = std::vector<std::tuple<int, std::vector<int>>>;





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


bool solve_polynomial_system(const fmpq_mat_t A, const fmpq_mat_t B, fmpq_mat_t X) {
    //std::cout << "Checking if the system can be solved using Dixon's method..." << std::endl;

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



// Function to create matrix from fmpq_t vector
void matrix_of_integral(const std::vector<fmpq_t*>& Iq, fmpq_mat_t& Q_matrix) {
    slong num_coeffs = Iq.size();

    // Initialize Q_matrix as a num_coeffs x 1 matrix
    fmpq_mat_init(Q_matrix, num_coeffs, 1);

    // Fill the first entry with zero (index 0)
    fmpq_set_ui(fmpq_mat_entry(Q_matrix, 0, 0), 0, 1); // Set the first entry to 0

    // Fill the rest of the matrix with coefficients from the Iq vector
    for (slong i = 1; i < num_coeffs; ++i) {
        fmpq_set(fmpq_mat_entry(Q_matrix, i, 0), *Iq[i]); // Use the fmpq_t directly
    }
}

void gcd(const std::vector<fmpq_t*>& coeffs, fmpq_t& common_factor) {
    fmpq_set_ui(common_factor, 1, 1); // Start with 1

    for (const auto& coeff : coeffs) {
        if (fmpq_is_zero(*coeff)) continue; // Skip zero coefficients
        fmpq_gcd(common_factor, common_factor, *coeff); // Update GCD
    }
}


void quasi_matrix(std::vector<fmpq_t*>& result, const std::vector<fmpq_t*>& Iq, int weightmax) {
    slong max_degree = Iq.size() - 1;
    std::cout << "Max degree: " << max_degree << std::endl;

    std::vector<fmpq_poly_t*> Evector;
    express_as_powers(Evector, max_degree, weightmax);

    std::vector<fmpq_poly_t*> filtered_Evector;
    filter_vector(filtered_Evector, Evector, max_degree);

    fmpq_mat_t A;
    polynomial_to_matrix(A, filtered_Evector);
    std::cout << "Matrix A dimensions: " << fmpq_mat_nrows(A) << " x " << fmpq_mat_ncols(A) << std::endl;
    std::cout << "Matrix A contents:" << std::endl;
    std::cout << "==================" << std::endl;
    for (slong i = 0; i < fmpq_mat_nrows(A); i++) {
        std::cout << "[ ";
        for (slong j = 0; j < fmpq_mat_ncols(A); j++) {
            fmpq_print(fmpq_mat_entry(A, i, j));
            if (j < fmpq_mat_ncols(A) - 1) {
                std::cout << ", ";
            }
        }
        std::cout << " ]" << std::endl;
    }
    std::cout << "==================" << std::endl;
    fmpq_mat_t Q;
    matrix_of_integral(Iq, Q);

    fmpq_mat_t A_transposed, Q_transposed;
    fmpq_mat_init(A_transposed, fmpq_mat_ncols(A), fmpq_mat_nrows(A));
    fmpq_mat_transpose(A_transposed, A);
    fmpq_mat_init(Q_transposed, fmpq_mat_ncols(Q), fmpq_mat_nrows(Q));
    fmpq_mat_transpose(Q_transposed, Q);
    // std::cout << "Matrices A and Q transposed." << std::endl;


    std::cout << "Matrix A_transposed created. Dimensions: "
        << fmpq_mat_nrows(A_transposed) << " x " << fmpq_mat_ncols(A_transposed) << std::endl;

    std::cout << "Matrix Q created. Dimensions: "
        << fmpq_mat_nrows(Q) << " x " << fmpq_mat_ncols(Q) << std::endl;

    fmpq_mat_print(Q);

    fmpq_mat_t X;
    fmpq_mat_init(X, fmpq_mat_nrows(A), fmpq_mat_ncols(Q));
    // std::cout << "Attempting to solve the system..." << std::endl;

    if (solve_polynomial_system(A_transposed, Q, X)) {
        std::cout << "System solved successfully!" << std::endl;
        std::cout << "Matrix X dimensions: "
            << fmpq_mat_nrows(X) << " x " << fmpq_mat_ncols(X) << std::endl;

        // Print the solution matrix X before normalization
        std::cout << "Solution matrix X (before normalization):" << std::endl;
        for (slong i = 0; i < fmpq_mat_nrows(X); ++i) {
            fmpq_print(fmpq_mat_entry(X, i, 0));
            std::cout << std::endl;
        }

        std::vector<fmpq_t*> new_coeffs;

        for (slong i = 0; i < fmpq_mat_nrows(X); ++i) {
            fmpq_t* coeff = (fmpq_t*)malloc(sizeof(fmpq_t));
            fmpq_init(*coeff);
            fmpq_set(*coeff, fmpq_mat_entry(X, i, 0));

            new_coeffs.push_back(coeff);

        }

        fmpq_t common_factor;
        fmpq_init(common_factor);
        gcd(new_coeffs, common_factor);
        std::cout << "GCD computed." << std::endl;

        // Print the GCD for debugging
        std::cout << "Computed GCD: ";
        fmpq_print(common_factor);
        std::cout << std::endl;

        std::cout << "Number of coefficients in new_coeffs: " << new_coeffs.size() << std::endl;
        std::cout << " new_coeff of size " << new_coeffs.size() << std::endl;
        for (size_t i = 0; i < new_coeffs.size(); ++i) {
            if (!fmpq_is_zero(*new_coeffs[i])) {
                fmpq_div(*new_coeffs[i], *new_coeffs[i], common_factor);  // Normalize by dividing by GCD
            }
            result.push_back(new_coeffs[i]);
        }


    }
    // Cleanup memory
    fmpq_mat_clear(A);
    fmpq_mat_clear(Q);
    fmpq_mat_clear(X);
    fmpq_mat_clear(A_transposed);
    fmpq_mat_clear(Q_transposed);

    for (auto& poly : Evector) {
        fmpq_poly_clear(*poly);
        delete poly;
    }
    for (auto& poly : filtered_Evector) {
        fmpq_poly_clear(*poly);
        delete poly;
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

 // Function to convert fmpq_t to a string
std::string fmpqToString(const fmpq_t f) {
    char* str = fmpq_get_str(NULL, 10, f);
    std::string result(str);
    flint_free(str);
    return result;
}

// Function to convert a vector of fmpq_t* to a string
std::string vectorToStringFmpq(const std::vector<fmpq_t*>& vec) {
    std::stringstream ss;
    for (const auto& f : vec) {
        if (f != nullptr) {
            ss << fmpqToString(*f) << " ";
        }
        else {
            ss << "null ";
        }
    }
    return ss.str();
}

// Function to convert a string to a vector of fmpq_t
std::vector<fmpq_t*> stringToVectorFmpq(const std::string& str) {
    std::vector<fmpq_t*> result;

    if (str.empty()) {
        return result;  // Return an empty vector if the string is empty
    }

    std::stringstream ss(str);
    std::string token;

    while (ss >> token) {
        // Allocate memory for a new fmpq_t
        fmpq_t* f = (fmpq_t*)malloc(sizeof(fmpq_t));
        fmpq_init(*f);

        // Set the fmpq_t value from the string
        fmpq_set_str(*f, token.c_str(), 10);
        fmpq_canonicalise(*f);

        // Add the fmpq_t pointer to the result vector
        result.push_back(f);
    }

    return result;
}
int sumVector(const std::vector<int>& vec) {
    int sum = 0;
    for (int num : vec) {
        sum += num;
    }
    return sum;
}
// Function to sum two vectors of fmpq_t*
std::vector<fmpq_t*> sumOfVectorsFmpq(const std::vector<fmpq_t*>& v, const std::vector<fmpq_t*>& w) {
    std::vector<fmpq_t*> sum_vector;
    size_t max_size = std::max(v.size(), w.size());

    for (size_t i = 0; i < max_size; ++i) {
        fmpq_t* sum_f = (fmpq_t*)malloc(sizeof(fmpq_t));
        fmpq_init(*sum_f);

        if (i < v.size() && v[i] != nullptr) {
            fmpq_set(*sum_f, *v[i]);
        }

        if (i < w.size() && w[i] != nullptr) {
            fmpq_add(*sum_f, *sum_f, *w[i]);
        }

        sum_vector.push_back(sum_f);
    }

    return sum_vector;
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

    return { memory_usage, elapsed_time };
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


std::string vectorToStringULong(const std::vector<unsigned long>& vec) {
    std::stringstream ss;
    for (unsigned long val : vec) {
        ss << val << ' ';
    }
    return ss.str();
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

unsigned long partialBinomialSum(int n, int d) {
    unsigned long sum = 0;

    for (int i = 1; i <= d; ++i) {
        sum += binomial(i + n - 1, i); // Calculate and add C(i+n-1, i)
    }
    return sum;
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
    for (size_t i = 0; i < l.size(); ++i) {
        if (i < L.size()) {
            L[i] += l[i];
        }
    }

    return L;
}

void filter_terms(fmpq_mpoly_t result, const fmpq_mpoly_t poly, const std::vector<slong>& variables, const std::vector<int>& power, const fmpq_mpoly_ctx_t ctx) {
    fmpq_t coeff;
    fmpq_init(coeff);

    fmpq_mpoly_zero(result, ctx);  // Initialize result to zero polynomial

    ulong* exps = new ulong[fmpq_mpoly_ctx_nvars(ctx)];

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
            fmpq_mpoly_get_coeff_fmpq_ui(coeff, poly, exps, ctx);
            fmpq_mpoly_set_coeff_fmpq_ui(result, coeff, exps, ctx);
        }
    }

    fmpq_clear(coeff);
    delete[] exps;
}

void feynman_integral_branch_type(fmpq_t myfey, graph& Gv, const std::vector<int>& av, const std::vector<int>& g, const std::vector<int>& l) {
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
        filter_terms(filtered_temp, temp, filter_vars, filter_power, ctx);

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
    filter_terms(final_result, invsfunct, filter_vars, filter_power, ctx);


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
                fmpq_mpoly_t constterm1_j, filter_terms1;
                fmpq_mpoly_init(constterm1_j, ctx);
                fmpq_mpoly_init(filter_terms1, ctx);

                constterm(Gv[j].first, Gv[j].second, Gv[j].first, Gv[j].second, N, m, constterm1_j, ctx);
                filter_terms(filter_terms1, constterm1_j, filter_vars, filter_power, ctx);

                fmpq_mpoly_mul(tmp, tmp, filter_terms1, ctx);
                fmpq_mpoly_clear(constterm1_j, ctx);
                fmpq_mpoly_clear(filter_terms1, ctx);

            }
            else if (fi2[j] == 0) {
                fmpq_mpoly_t constterm0_j, filter_terms0;
                fmpq_mpoly_init(constterm0_j, ctx);
                fmpq_mpoly_init(filter_terms0, ctx);
                constterm(Gv[j].second, Gv[j].first, Gv[j].second, Gv[j].first, N, m, constterm0_j, ctx);
                filter_terms(filter_terms0, constterm0_j, filter_vars, filter_power, ctx);
                fmpq_mpoly_mul(tmp, tmp, filter_terms0, ctx);
                fmpq_mpoly_clear(constterm0_j, ctx);
                fmpq_mpoly_clear(filter_terms0, ctx);
            }
            else if (fi2[j] == -2) {
                fmpq_mpoly_t looptermj, filter_terms_loop;
                fmpq_mpoly_init(looptermj, ctx);
                fmpq_mpoly_init(filter_terms_loop, ctx);
                loopterm(Gv[j].first, av[j], m, looptermj, ctx);
                filter_terms(filter_terms_loop, looptermj, filter_vars, filter_power, ctx);
                fmpq_mpoly_mul(tmp, tmp, filter_terms_loop, ctx);

                fmpq_mpoly_clear(looptermj, ctx);
                fmpq_mpoly_clear(filter_terms_loop, ctx);
            }
            else {
                fmpq_mpoly_t proterm_j, filter_terms_prot;
                fmpq_mpoly_init(proterm_j, ctx);
                fmpq_mpoly_init(filter_terms_prot, ctx);
                proterm(Gv[j].first, Gv[j].second, Gv[j].first, Gv[j].second, fi2[j], m, N, proterm_j, ctx);
                filter_terms(filter_terms_prot, proterm_j, filter_vars, filter_power, ctx);
                fmpq_mpoly_mul(tmp, tmp, filter_terms_prot, ctx);

                fmpq_mpoly_clear(proterm_j, ctx);
                fmpq_mpoly_clear(filter_terms_prot, ctx);
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

void feynman_integral_degrees(fmpq_t result, graph& Gv, const int& d, const std::vector<int>& g, const std::vector<int>& l) {

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
// Function to compute the quasimodular form
void quasimodular_form(fmpq_mpoly_t result, const std::vector<fmpq_t*>& Iq, int weightmax, fmpq_mpoly_ctx_t ctx) {
    // Step 1: Get the coefficients using quasi_matrix
    std::vector<fmpq_t*> coef; // Vector to store coefficients
    quasi_matrix(coef, Iq, weightmax);  //  coef with coefficients

    /*  std::cout << "coef vector:" << std::endl;
     for (size_t i = 0; i < coef.size(); ++i) {
         fmpq_print(*coef[i]);
         std::cout << std::endl;
     } */
     // Step 2: Get the Eisenstein series expressions
    std::vector<fmpq_mpoly_t*> comb_result; // Vector of fmpq_mpoly_t pointers
    express_as_eisenstein_series(comb_result, weightmax, ctx); //  comb_result with Eisenstein series

    /*   // Print the results
      for (size_t i = 0; i < comb_result.size(); ++i) {
          std::cout << "Result[" << i << "]: ";
          fmpq_mpoly_print_pretty(*comb_result[i], NULL, ctx);
          std::cout << std::endl;
      }
   */
   // Step 3: Initialize the result polynomial
    fmpq_mpoly_init(result, ctx); // Initialize the result polynomial

    // Step 4: Compute the sum of the products of coefficients and terms
    for (size_t i = 0; i < comb_result.size(); ++i) {
        if (i >= coef.size() || fmpq_is_zero(*coef[i])) {
            continue; // Skip if coefficient is zero or out of bounds
        }

        // Temporary polynomial to hold the product
        fmpq_mpoly_t tmp;
        fmpq_mpoly_init(tmp, ctx); // Initialize the temporary polynomial

        // Compute tmp = coef[i] * term
        fmpq_mpoly_scalar_mul_fmpq(tmp, *comb_result[i], *coef[i], ctx);

        // Add tmp to the result polynomial result
        fmpq_mpoly_add(result, result, tmp, ctx);

        // Clear the temporary polynomial after use
        fmpq_mpoly_clear(tmp, ctx);
    }

    // Clear coefficients
    for (auto& c : coef) {
        fmpq_clear(*c);
        // free(c);  // Free the memory allocated for fmpq_t pointers
    }
}


// Convert fmpq_mpoly to std::string representation
std::string fmpq_mpolyToString(const fmpq_mpoly_t A, const fmpq_mpoly_ctx_t ctx) {
    const char* x[] = { "E2", "E4", "E6" };  // Variable names
    char* result_str = fmpq_mpoly_get_str_pretty(A, x, ctx); // Get string representation

    std::string result(result_str);  // Convert to std::string
    flint_free(result_str);  // Free the memory allocated by fmpq_mpoly_get_str_pretty

    return result;
}

int main() {
    // Initialize Iq as a vector of fmpq_t pointers
    std::vector<fmpq_t*> Iq(11);

    // Initialize and assign values to Iq
    for (size_t i = 0; i < Iq.size(); ++i) {
        Iq[i] = (fmpq_t*)malloc(sizeof(fmpq_t));  // Allocate memory for fmpq_t
        fmpq_init(*Iq[i]);  // Initialize the fmpq_t value
    }

    // Set values for Iq as specified
    fmpq_set_ui(*Iq[0], 0, 1);            // 0
    fmpq_set_ui(*Iq[1], 1, 4);            // 1/4
    fmpq_set_ui(*Iq[2], 15, 1);           // 15
    fmpq_set_ui(*Iq[3], 117, 1);          // 117
    fmpq_set_ui(*Iq[4], 556, 1);          // 556
    fmpq_set_si(*Iq[5], 3075, 2);         // 3075/2
    fmpq_set_ui(*Iq[6], 4428, 1);         // 4428
    fmpq_set_ui(*Iq[7], 8330, 1);         // 8330
    fmpq_set_ui(*Iq[8], 18480, 1);        // 18480
    fmpq_set_ui(*Iq[9], 121581, 4);        // 121581/4
    fmpq_set_ui(*Iq[10], 56250, 1);        // 56250


    // Maximum weight
    int weightmax = 8;

    slong nv = 3;  // Number of variables
    fmpq_mpoly_ctx_t ctx;
    fmpq_mpoly_ctx_init(ctx, nv, ORD_DEGLEX);

    // Variable to hold the result
    fmpq_mpoly_t result;
    fmpq_mpoly_init(result, ctx);

    // Call the quasimodular_form function with Iq and weightmax
    quasimodular_form(result, Iq, weightmax, ctx);

    // Print the result
    std::cout << "Quasimodular form result (pretty): ";
    fmpq_mpoly_print_pretty(result, NULL, ctx);
    std::cout << std::endl;

    // Convert to string representation and print it
    std::string result_str = fmpq_mpolyToString(result, ctx);
    std::cout << "Quasimodular form result (string): " << result_str << std::endl;

    // Clear polynomials
    fmpq_mpoly_clear(result, ctx);

    // Clear context
    fmpq_mpoly_ctx_clear(ctx);

    // Free allocated memory for Iq
    for (size_t i = 0; i < Iq.size(); ++i) {
        fmpq_clear(*Iq[i]);  // Clear the fmpq_t value
        free(Iq[i]);         // Free the allocated memory
    }

    return 0;
}
