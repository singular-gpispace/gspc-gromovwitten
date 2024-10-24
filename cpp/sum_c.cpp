#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Function to convert a string of space-separated integers into a vector of integers
std::vector<int> stringToVectorInt(const std::string& str) {
    std::vector<int> result;
    std::stringstream ss(str);
    int num;
    while (ss >> num) {
        result.push_back(num);
    }
    return result;
}

// Function to sum a vector of integers
int sumVector(const std::vector<int>& vec) {
    int sum = 0;
    for (int num : vec) {
        sum += num;
    }
    return sum;
}

int main() {
    std::string genus = "1 0 0";

    // Convert the string to a vector of integers
    std::vector<int> genusVector = stringToVectorInt(genus);

    // Sum the integers in the vector
    int sum = sumVector(genusVector);

    std::cout << "Sum of genus: " << sum << std::endl;

    return 0;
}
