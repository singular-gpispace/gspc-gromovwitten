#include <iostream>
#include <vector>
#include <sstream>
#include <string>

// Convert a string to a vector of unsigned long
std::vector<unsigned long> stringToVectorUlong(const std::string& str) {
    std::vector<unsigned long> result;
    std::stringstream ss(str);
    unsigned long num;
    while (ss >> num) {
        result.push_back(num);
    }
    return result;
}

// Convert a vector of unsigned long to a string
std::string vectorToStringULong(const std::vector<unsigned long>& vec) {
    std::stringstream ss;
    for (unsigned long val : vec) {
        ss << val << ' ';
    }
    return ss.str();
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

int main() {
    // Example vectors to demonstrate the sum of strings
    std::vector<unsigned long> v = { 1, 2, 3 };
    std::vector<unsigned long> w = { 4, 5, 6 };

    // Convert vectors to strings
    std::string v_str = vectorToStringULong(v);
    std::string w_str = vectorToStringULong(w);

    // Convert strings back to vectors
    std::vector<unsigned long> v_vec = stringToVectorUlong(v_str);
    std::vector<unsigned long> w_vec = stringToVectorUlong(w_str);

    // Sum of the two vectors
    std::vector<unsigned long> result_vec = sumOfVectors(v_vec, w_vec);

    // Convert the result vector back to a string
    std::string result_str = vectorToStringULong(result_vec);

    // Output the result
    std::cout << "Sum of the two vectors: " << result_str << std::endl;

    return 0;
}
