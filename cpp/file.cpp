#include <iostream>
#include <fstream>
#include <string>

int main() {
    // File path
    std::string filePath = "/home/gnawali/example.txt";

    // Create an input file stream (ifstream) to read the file
    std::ifstream file(filePath);

    // Check if the file was successfully opened
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file at " << filePath << std::endl;
        return 1;
    }

    // Read and display the file content
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }

    // Close the file
    file.close();

    return 0;
}