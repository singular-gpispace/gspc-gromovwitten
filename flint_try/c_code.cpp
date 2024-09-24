#include <iostream>
#include <vector>
#include <sstream>
#include <string>

// Convert a string to a vector of unsigned long
std::vector<unsigned long> stringToVectorUlong(const std::string& str) {
    std::vector<unsigned long> result;
    std::stringstream ss(str);
    unsigned long num;  // Corrected to use unsigned long
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

int main() {
    std::string sum = "1 0";  // Initialize string properly
    int sss = sum[0];
    std::cout << "sss " << sss << std::endl;
    unsigned long s = 5;      // Example value for 's'

    std::vector<unsigned long> su = stringToVectorUlong(sum);

    su[0] += s;
    su[1] += 1;

    // Print the vector after modification
    std::cout << "su after modification: ";
    for (unsigned long i : su) {
        std::cout << i << ' ';  // Print each element followed by a space
    }
    std::cout << std::endl;

    // Convert the vector back to string
    std::string se = vectorToStringULong(su);
    std::cout << "se: " << se << std::endl;

    return 0;
}
