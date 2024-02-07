#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>

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

int main() {
    std::vector<int> partition = {0, 2, 1, 1};
    std::vector<int> next = next_partition(partition);
    
    for (int i : next) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
