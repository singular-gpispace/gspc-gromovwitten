#include <iostream>
#include <vector>
#include <tuple>
#include <sstream> // for std::ostringstream
#include <string>

std::string tupleToString(const std::tuple<int, std::vector<int>>& tuple) {
    std::ostringstream oss;
    oss << "(" << std::get<0>(tuple) << ", {";
    const auto& vec = std::get<1>(tuple);
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i != vec.size() - 1) {
            oss << ", ";
        }
    }
    oss << "})";
    return oss.str();
}

std::tuple<int, std::vector<int>> stringToTuple(const std::string& a) {
    std::tuple<int, std::vector<int>> mk;
    std::istringstream iss(a);
    char discard;
    int integer;
    std::vector<int> vec;
    if (!(iss >> discard >> integer >> discard >> discard)) {
        throw std::invalid_argument("Invalid string format");
    }

    char comma;
    while (iss >> integer >> comma) {
        vec.push_back(integer);
    }
    mk=std::make_tuple(integer, vec);
    return mk;
}
int main() {
    std::tuple<int, std::vector<int>> g = {1, {10, 20, 30}};

    std::string str = tupleToString(g);
    std::cout << str << std::endl;



    std::tuple<int, std::vector<int>> gg = stringToTuple(str);

    std::cout << "Integer: " << std::get<0>(gg) << "  ";
    for (const auto& element : std::get<1>(gg)) {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    return 0;
}
