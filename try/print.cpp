#include "print.hpp"
#include <iostream>

void printVector(const std::vector<int> &vec)
{
    for (int value : vec)
        std::cout << value << " ";
    std::cout << std::endl;
}
