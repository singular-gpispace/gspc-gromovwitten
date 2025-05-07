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
    for (size_t i = 0; i < num_polynomials; i++) {
        slong deg = fmpq_poly_degree(*polynomials[i]);
        if (deg > max_degree) {
            max_degree = deg;
        }
    }

    // Initialize the matrix A with num_polynomials rows and max_degree+1 columns
    fmpq_mat_init(A, num_polynomials, max_degree + 1);

    // Fill the matrix A with coefficients from the polynomials
    for (size_t i = 0; i < num_polynomials; i++) {
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
            if (exps[variables[j]] > power[j]) {  // Ensure term does not exceed power limit
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
    int nv = 3;  // Number of variables

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