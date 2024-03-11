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
std::vector<std::vector<int>> combinations(std::vector<int> (*f) (std::vector<int>), std::vector<int> x){
    int k=x.size();
    
    if (k == 0) {
        throw std::invalid_argument("k should be nonzero");
    }
    int d= std::accumulate(x.begin(),x.end(),0);
    int n = binomial(d + k - 1, d);
    int e = d - x[0];
    std::vector<std::vector<int>> ru;
    std::vector<int> result = x;    
    std::vector<int> y(k, 0); 
    y[0] = x[0] - 1;
    y[k - 1] = e + 1;
    for (int i = 0; i < n; ++i) {
        if (result != y) {
            result = next_partition(result);
            ru.push_back(result);
        } else {
            break;
        }
    }
    return ru;
}


std::vector<std::vector<int>> sum_degree(int n, int d){
    std::vector<std::vector<int>> ru;
    for(int e=0; e<d; e++){
        std::vector<int> x(n,0);
        x[0]=d-e; 
        x[n-1]=e;
        ru.push_back(x);
    }
    return ru;
}
int main() {
    /* std::vector<int> partition = {1, 0, 2, 0, 0, 1};
    std::vector<int> next = next_partition(partition);
    for (int num : next) {
            std::cout << num << " ";
        }
    std::cout<<std::endl;
    
    std::vector<std::vector<int>> partitions = combination(next_partition, 6, 4);
    for (const auto& partition : partitions) {
        for (int num : partition) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
*/
    std::vector<int> x = {};
    std::vector<std::vector<int>> result = combinations(next_partition, x);
    for (const auto& partition : result) {
        for (int val : partition) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
            std::cout << " sum_degree : "<<std::endl;
            std::vector<std::vector<int>> sum_deg=sum_degree(3,8);
            for (std::vector<int> v:sum_deg){
                for (int i :v){
                    std::cout<<i<< " " ;
                }
                        std::cout << std::endl;

            }

    return 0;
}
