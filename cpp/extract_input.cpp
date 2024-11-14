#include <iostream>
#include <vector>
#include <string>
#include <cctype>


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
            // Add current token to result if not inside braces
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        }
        else {
            // Add character to current token
            current += c;
        }
    }

    // Add the last token
    if (!current.empty()) {
        result.push_back(current);
    }

    // Trim leading/trailing whitespace from each entry
    for (auto& entry : result) {
        entry.erase(0, entry.find_first_not_of(" \t"));
        entry.erase(entry.find_last_not_of(" \t") + 1);
    }

    return result;
}

int main() {
    std::string myinput = "1, 5, { {1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4} }, 4, {1,2,3,4}";
    std::vector<std::string> myinput_vector = parseInput(myinput);

    // Print results
    for (size_t i = 0; i < myinput_vector.size(); ++i) {
        std::cout << "myinput_vector[" << i << "]=" << myinput_vector[i] << std::endl;
    }
    std::cout << "vector " << myinput_vector[4] << std::endl;
    std::cout << " size of vector " << myinput_vector[4].size() << std::endl;

    std::string ss = { 1,2,3,4 };
    std::cout << " size of ss " << ss.size() << std::endl;

    return 0;
}
