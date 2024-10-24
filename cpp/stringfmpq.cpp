#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <flint/fmpq.h>
#include <flint/fmpz.h>

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

// Main example
int main() {
    std::string sum = "1/2 2/3";
    std::string s = "3/4 1 5/6";

    // Convert strings to fmpq_t vectors
    std::vector<fmpq_t*> v = stringToVectorFmpq(sum);
    std::vector<fmpq_t*> w = stringToVectorFmpq(s);

    // Perform the sum of the vectors
    std::vector<fmpq_t*> sum_vector = sumOfVectorsFmpq(v, w);

    // Convert the result back to a string
    sum = vectorToStringFmpq(sum_vector);
    std::cout << "Sum of vectors: " << sum << std::endl;

    // Clear memory for fmpq_t vectors
    for (auto& f : v) {
        fmpq_clear(*f);
        free(f);
    }
    for (auto& f : w) {
        fmpq_clear(*f);
        free(f);
    }
    for (auto& f : sum_vector) {
        fmpq_clear(*f);
        free(f);
    }

    return 0;
}
