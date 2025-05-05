#include <iostream>
#include <vector>

// Function to compute binomial coefficient C(n, k)
unsigned long binomial(const int n, const int k) {
    if (k == 0 || k == n) return 1; // Special case for C(n, 0) or C(n, n)

    std::vector<unsigned long> vec(k);
    vec[0] = n - k + 1;

    for (int i = 1; i < k; ++i) {
        vec[i] = vec[i - 1] * (n - k + 1 + i) / (i + 1);
    }

    return vec[k - 1];
}

// Function to calculate the partial sum of binomial coefficients S(d, n)
unsigned long partialBinomialSum(int n, int d) {
    unsigned long sum = 0;

    for (int i = 2; i <= d; ++i) {
        sum += binomial(i + n - 1, i); // Calculate and add C(i+n-1, i)
    }

    return sum;
}

int main() {
    int d = 5;  // Example value of d
    int n = 6;  // Example value of n
    int dd = 3;
    unsigned long res = binomial(dd + n - 1, dd);

    unsigned long result = partialBinomialSum(n, d);


    std::cout << "  binomial coefficients: " << res << std::endl;


    std::cout << "Partial sum of binomial coefficients: " << result << std::endl;

    return 0;
}
