#include <vector>
#include <sstream>
#include <iostream>

std::string vectorToString(const std::vector<std::vector<int>>& vec) {
    std::stringstream ss;
    for (const auto& innerVec : vec) {
        for (int val : innerVec) {
            ss << val << ' ';
        }
        ss << '\n'; // Assuming you want a newline between inner vectors
    }
    return ss.str();
}