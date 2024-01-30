#include <iostream>
#include <algorithm>
#include <vector>
#include "print.hpp"
#include "parti.hpp"

using namespace std;

// Forward declaration of parti function
std::vector<std::vector<int>> parti(int d, int n);


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