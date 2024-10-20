#include <iostream>
#include <sstream>
#include <vector>

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

int main() {
    // Sample string inputs and vector
    std::string s = "3 5";  // First input
    std::string ss = "4 2"; // Second input
    std::vector<unsigned long> v;

    // Update the vector based on the input strings
    updateVectorFromString(v, s);
    updateVectorFromString(v, ss);

    // Output the updated vector for verification
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << "v[" << i << "] = " << v[i] << std::endl;
    }

    return 0;
}
