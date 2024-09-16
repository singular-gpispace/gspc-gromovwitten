#include <iostream>
#include <sstream>
#include <vector>

// Convert two integers to a single string
std::string convertTwoIntsToString(unsigned long n1, unsigned long n2) {
    std::stringstream ss;
    ss << n1 << ' ' << n2;  // Use a space as a delimiter between the two numbers
    return ss.str();
}

// Convert a string back to two integers
std::pair<int, int> convertStringToTwoInts(const std::string& str) {
    std::stringstream ss(str);
    unsigned long n1, n2;
    ss >> n1 >> n2;  // Extract the two integers from the string
    return std::make_pair(n1, n2);
}

int main() {
    // Step 1: Convert two integers to a string
    unsigned long n1 = 123;
    unsigned long n2 = 456;
    std::string str = convertTwoIntsToString(n1, n2);
    std::cout << "Converted string: " << str << std::endl;

    // Step 2: Convert the string back to two integers
    std::pair<int, int> recoveredNumbers = convertStringToTwoInts(str);
    std::cout << "Recovered numbers: " << recoveredNumbers.first << ", " << recoveredNumbers.second << std::endl;

    return 0;
}
