#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <utility>

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

int main() {
    std::string graphInput = "{ {1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4} }";

    // Extract integer pairs from the input string
    std::vector<std::pair<int, int>> Gv = extractIntegerPairs(graphInput);

    // Print the pairs to verify
    for (const auto& pair : Gv) {
        std::cout << "{" << pair.first << ", " << pair.second << "} ";
    }
    std::cout << std::endl;
    std::cout << "size of graph " << Gv.size() << std::endl;

    return 0;
}
