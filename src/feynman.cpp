#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <utility> // for std::pair
#include <string>
#include <ostream>
#include <iostream>


int h(int input, int P0) {
    std::cout << "DEBUG: h() called with input = " << input << ", P0 = " << P0 << std::endl;
    int result = input + P0;
    std::cout << "DEBUG: h() result = " << result << std::endl;
    return result;
}
int g(int input, std::string& output) {
    std::cout << "DEBUG: g() called with input = " << input << std::endl;
    
    int result = input * 2;  // Example transformation
    output = std::to_string(result);  // **Explicitly set output!**

    std::cout << "DEBUG: g() output = " << output << std::endl;
    return result;
}

