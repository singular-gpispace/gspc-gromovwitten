#include <iostream>
#include <vector>
#include <numeric>
#include <sstream>

std::string sum_degree(int n, int d) {
    std::vector<std::vector<int>> ru; 

    for (int e = 0; e < d; e++) {
        std::vector<int> x(n, 0);
        x[0] = d - e;
        x[n - 1] = e;
        ru.push_back(x);
    }

    std::stringstream ss;
    for (const auto& innerVec : ru) {
        for (int val : innerVec) {
            ss << val << ' ';
        }
        ss << '\n'; 
    }
    return ss.str();
}

int main() {
    int n = 3; 
    int d = 4; 
    std::string result = sum_degree(n, d);
    
    // Print the result
    std::cout << result;

    return 0;
}
