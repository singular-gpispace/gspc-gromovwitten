#include <iostream>
#include <algorithm>
#include <vector>
#include "print.hpp"
#include "parti.hpp"

// Forward declaration of parti function
std::vector<std::vector<int>> parti(int d, int n);

void generateAndPrintPermutations(std::vector<int> vec);
int main()
{
    int d = 5;
    int n = 6;

    // Generate partitions and print permutations
    for (const auto &vec : parti(d, n))
    {
        generateAndPrintPermutations(vec);
    }

    return 0;
}
