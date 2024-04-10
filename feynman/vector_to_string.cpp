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


std::vector<std::vector<int>> stringToVector(const std::string& str) {
    std::vector<std::vector<int>> result;
    std::stringstream ss(str);
    std::string line;
    while (std::getline(ss, line)) {
        std::vector<int> innerVec;
        std::stringstream lineStream(line);
        int num;
        while (lineStream >> num) {
            innerVec.push_back(num);
        }
        result.push_back(innerVec);
    }
    return result;
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
std::vector<std::vector<int>> flattenVector(const std::vector<std::vector<std::vector<int>>>& ge) {
    std::vector<std::vector<int>> flat_vector;
    for (const auto& innerVec1 : ge) {
        for (const auto& innerVec2 : innerVec1) {
            flat_vector.push_back(innerVec2);
        }
    }
    return flat_vector;
}