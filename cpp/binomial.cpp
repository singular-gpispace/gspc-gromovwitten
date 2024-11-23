#include <iostream>
#include <vector>
// Function to compute binomial coefficients C(n, k)
unsigned long binomial(int n, int k) {
    if (k > n) return 0;      // Binomial coefficients are undefined for k > n
    if (k == 0 || k == n) return 1; // Base cases

    unsigned long result = 1;
    k = std::min(k, n - k);   // Use symmetry property C(n, k) = C(n, n-k)
    for (int i = 0; i < k; ++i) {
        result *= (n - i);
        result /= (i + 1);
    }
    return result;
}

// Function to compute partial binomial sum
unsigned long partialBinomialSum(int n, int d) {
    unsigned long sum = 0;
    for (int i = 2; i <= d; ++i) {
        sum += binomial(i + n - 1, i); // Calculate and add C(i+n-1, i)
    }
    return sum;
}
#include <iostream>

int main() {
    int n = 3, d = 10;
    std::cout << "Binomial(n, d): " << binomial(n, d) << std::endl;
    std::cout << "Partial Binomial Sum: " << partialBinomialSum(n, d) << std::endl;
    return 0;
}
