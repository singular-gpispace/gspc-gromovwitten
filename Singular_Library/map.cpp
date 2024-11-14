#include <iostream>
#include <map>

int main() {
    // Define a map with key of type string and value of type int
    std::map<std::string, int> ages;

    // Insert key-value pairs into the map
    ages["Alice"] = 30;
    ages["Bob"] = 25;
    ages["Charlie"] = 35;

    // Access elements in the map
    std::cout << "Alice's age: " << ages["Alice"] << std::endl;

    // Iterate over the elements in the map
    for (const auto& pair : ages) {
        std::cout << pair.first << "'s age is " << pair.second << std::endl;
    }

    return 0;
}
