#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <sstream>
#include "feynman.hpp"

// Function to extract integer pairs from a string
std::vector<std::pair<int, int>> extractIntegerPairs(const std::string& graph) {
    std::vector<std::pair<int, int>> Gv;
    std::string num;
    int first = 0;
    bool expectingSecond = false;

    for (char c : graph) {
        if (isdigit(c)) {
            num += c;  // Collect digits to form a number
        }
        else if (!num.empty()) {
            int value = std::stoi(num);  // Convert the collected number string to an integer
            num.clear();  // Clear the string for the next number

            if (expectingSecond) {
                Gv.emplace_back(first, value);
                expectingSecond = false;
            }
            else {
                first = value;
                expectingSecond = true;
            }
        }
    }
    return Gv;
}
std::vector<std::string> parseInput(const std::string& input) {
    std::vector<std::string> result;
    std::string current;
    int braceCount = 0;

    for (char c : input) {
        if (c == '{') {
            braceCount++;
            current += c;
        }
        else if (c == '}') {
            braceCount--;
            current += c;
        }
        else if (c == ',' && braceCount == 0) {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        }
        else {
            current += c;
        }
    }
    if (!current.empty()) {
        result.push_back(current);
    }
    return result;
}

std::vector<int> parsevector(const std::string& input) {
    std::vector<int> numbers;
    std::stringstream ss(input);
    char c;
    int number;

    while (ss >> c) {
        if (std::isdigit(c) || (c == '-' && std::isdigit(ss.peek()))) {
            ss.putback(c);
            ss >> number;
            numbers.push_back(number);
        }
    }
    return numbers;
}
