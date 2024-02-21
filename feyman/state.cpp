#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <stdexcept>

// Calculate binomial coefficient (n choose k)
int binomial(int n, int k) {
    if (k < 0 || k > n) {
        throw std::invalid_argument("Invalid arguments for binomial coefficient");
    }
    
    int result = 1;
    for (int i = 1; i <= k; ++i) {
        result *= (n - i + 1);
        result /= i;
    }
    return result;
}

std::vector<int> next_partition(std::vector<int> a) {
    int n = std::accumulate(a.begin(), a.end(), 0);
    int k = a.size();
    for (int i = k - 1; i >= 0; --i) {
        if (i == k - 1 && a[i] == n) {
            return a;
        } else {
            for (int j = i - 1; j >= 0; --j) {
                if (a[j] != 0) {
                    --a[j];
                    int ak = a[k - 1];
                    a[k - 1] = 0;
                    a[j + 1] = ak + 1;
                    return a;
                }
            }
        }
    }
    return a; // If no suitable partition is found
}

std::vector<std::vector<int>> combination(std::vector<int> (*f)(std::vector<int>), int k, int d) {
    if (k == 0) {
        throw std::invalid_argument("k should be nonzero");
    }
   
    std::vector<int> x(k , 0);
    x[0] = d;
    int n = binomial(d + k - 1, d);
    std::vector<std::vector<int>> ru;
    std::vector<int> result = x;
    ru.push_back(x);
    for (int i = 1; i < n; ++i) {
        result = f(result);
        ru.push_back(result);
    }
    return ru;
}

int main() {
     std::vector<int> partition = {0, 2, 1, 1};
    std::vector<int> next = next_partition(partition);
    
    
    std::vector<std::vector<int>> partitions = combination(next_partition, 4, 4);
    for (const auto& partition : partitions) {
        for (int num : partition) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
