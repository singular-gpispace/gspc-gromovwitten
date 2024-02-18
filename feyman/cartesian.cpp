#include <iostream>
#include <vector>
#include <tuple>
#include <functional>

void CartesianRecurse(std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>> &accum,
                      std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> stack,
                      const std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>> &sequences,
                      int index) {
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> sequence = sequences[index];
    
    for (const auto& tuple : sequence) {
        stack.push_back(tuple);
        
        if (index == 0) {
            // If this is the last sequence, add the current stack to the accum vector
            accum.push_back(stack);
        } else {
            // Otherwise, recursively call CartesianRecurse with the next sequence
            CartesianRecurse(accum, stack, sequences, index - 1);
        }
        
        // Remove the last element from the stack for backtracking
        stack.pop_back();
    }
}

std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>> CartesianProduct(
    const std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>> &sequences) {
    
    std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>> accum;
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> stack;
    
    if (!sequences.empty())
        CartesianRecurse(accum, stack, sequences, sequences.size() - 1);
    
    return accum;
}

int main() {
    // Example usage
    std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>> sequences = {
        {{{1, 1}, {3, -1}}, {{1, 2}, {3, -2}}, {{1, 3}, {3, -3}}, {{1, 4}, {3, -4}}},
        {{{1, 1}, {2, -1}}, {{1, -1}, {2, 1}}}
    };

    std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>> res = CartesianProduct(sequences);

    // Output the result
    for (const auto& vec : res) {
        for (const auto& tuple : vec) {
            std::cout << "(( " << tuple.first.first << ", " << tuple.first.second << " ), ( "
                      << tuple.second.first << ", " << tuple.second.second << " )) ";
        }
        std::cout << std::endl;
    }

    return 0;
}
