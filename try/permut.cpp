#include <iostream>
#include <algorithm>
#include <vector>
#include "print.hpp"


void generateAndPrintPermutations(std::vector<int> vec)
{
    // Sort the vector to ensure permutations are unique
    std::sort(vec.begin(), vec.end());

    // Collect permutations and print them
    do
    {
        printVector(vec);
    } while (std::next_permutation(vec.begin(), vec.end()));
}


int main()
{
    std::vector<int> yy = {1, 1, 3, 1};

    // Call the function to generate and print permutations
    generateAndPrintPermutations(yy);

    return 0;
}