#ifndef TRY_H
#define TRY_H

#include <vector>
#include <algorithm>

using vector2d = std::vector<std::vector<int>>;

// Function implementations
inline unsigned long binomial(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    k = std::min(k, n - k); // Take advantage of symmetry
    unsigned long result = 1;
    for (int i = 1; i <= k; ++i) {
        result = result * (n - k + i) / i;
    }
    return result;
}


#endif // TRY_H 