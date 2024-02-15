#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include "feynman.hpp"

std::vector<int> replaces(const std::vector<int>& vector) {
    std::vector<int> result_vector;
    for (int x : vector) {
        result_vector.push_back(x == -1 ? 0 : (x == 0 ? -1 : x));
    }
    return result_vector;
}

int count_zero(const std::vector<int>& arr) {
    int count_zeros = 0;
    for (int element : arr) {
        if (element == 0) {
            count_zeros++;
        }
    }
    return count_zeros;
}

int preimg(const std::vector<int>& L, int xi) {
    for (int i = 0; i < L.size(); i++) {
        if (L[i] == xi) {
            return i;
        }
    }
}

std::unordered_map<int, int> count_member(const std::vector<int>& itr) {
    std::unordered_map<int, int> d;
    for (int val : itr) {
        if (!isnan(val)) {
            d[val]++;
        }
    }
    return d;
}

int main() {
    // Testing the functions
    return 0;
}
