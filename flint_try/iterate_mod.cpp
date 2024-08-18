
#include <list>
#include <tuple>
#include <sstream> // for std::istringstream
#include <fstream>
#include <sys/resource.h>
#include <iostream>
#include <chrono>
#include <functional> // Include for std::function
#include <vector>     // Include for std::vector
#include <numeric>
#include <unordered_set> // for std::unordered_set

using vector2d = std::vector<std::vector<int>>;
using list_type = std::list<std::string>; // Define list_type as std::list<std::string>

std::size_t get_memory_usage()
{
    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    return r_usage.ru_maxrss; // ru_maxrss is in kilobytes
}

template <typename Func>
struct ResourceUsage
{
    std::size_t memory_usage; // in kilobytes
    long long elapsed_time;   // in microseconds
};

template <typename Func>
ResourceUsage<Func> measure_resource_usage(Func func)
{
    // Measure start time
    auto start_time = std::chrono::steady_clock::now();

    // Measure start memory usage
    std::size_t start_memory = get_memory_usage();

    // Execute the provided function
    func();

    // Measure end time
    auto end_time = std::chrono::steady_clock::now();

    // Measure end memory usage
    std::size_t end_memory = get_memory_usage();

    // Calculate elapsed time
    auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    // Calculate memory usage difference
    std::size_t memory_usage = end_memory - start_memory;

    return { memory_usage, elapsed_time };
}

vector2d gen_block1(int n, int d)
{
    vector2d v;
    for (int e = 0; e < d; e++)
    {
        std::vector<int> x(n, 0);
        x.front() = d - e;
        x.back() = e;

        v.push_back(x);
    }
    return v;
}
vector2d gen_block2(int n, int d)
{
    vector2d v;
    for (int e = 0; e < d + 1; e++)
    {
        std::vector<int> x(n, 0);
        x.front() = d - e;
        x.back() = e;

        v.push_back(x);
    }
    return v;
}

vector2d gen_block(int n, int d) {
    vector2d ru;
    if (n < 4) {
        return gen_block1(n, d);
    }
    auto blo = gen_block2(n, d);
    for (auto v : blo) {
        if (v[0] != 0 && v.back() < 3) {
            ru.push_back(v);
        }
        else if (n > 3 && v.back() > 3) {
            for (int e = 0; e < d; ++e) {
                if (e == 1 || d - e - v[0] <= 0) {
                    continue;
                }
                else {
                    std::vector<int> x = { v[0], d - e - v[0] };
                    x.insert(x.end(), n - 3, 0);
                    x.push_back(e);
                    ru.push_back(x);
                }
            }
        }
    }
    return ru;
}

unsigned long  binomial(const int n, const int k) {
    std::vector<unsigned long > vec(k);
    vec[0] = n - k + 1;

    for (int i = 1; i < k; ++i) {
        vec[i] = vec[i - 1] * (n - k + 1 + i) / (i + 1);
    }

    return vec[k - 1];
}
/* unsigned long binomial(const int n, const int k)
{
    if (k < 0 || k > n)
    {
        throw std::invalid_argument("Invalid arguments for binomial coefficient");
    }

    unsigned long result = 1;
    for (unsigned long i = 1; i <= k; ++i)
    {
        result *= (n - i + 1);
        result /= i;
    }
    return result;
} */

std::vector<int> next_partition(std::vector<int> a)
{
    int n = std::accumulate(a.begin(), a.end(), 0);
    int k = a.size();
    if (a[k - 1] == n)
    {
        return a;
    }
    for (int i = k - 1; i >= 0; --i)
    {
        for (int j = i - 1; j >= 0; --j)
        {
            if (a[j] != 0)
            {
                --a[j];
                int ak = a[k - 1];
                a[k - 1] = 0;
                a[j + 1] = ak + 1;
                return a;
            }

        }
    }
    return a;
}

vector2d iterate1(std::vector<int> xa)
{
    vector2d gen;

    int k = xa.size();
    if (k == 0)
    {
        throw std::invalid_argument("k should be nonzero");
    }
    int d = std::accumulate(xa.begin(), xa.end(), 0);
    if (d == xa[0])
    {
        gen.push_back(xa);
    }
    int e = d - xa[0] + 1;

    while (true)
    {
        if (xa[k - 1] != e)
        {
            xa = next_partition(xa);
            gen.push_back(xa);
        }
        else
        {
            break;
        }
    }
    return gen;
}

vector2d iterate(std::vector<int> x) {
    int k = x.size();
    int d = accumulate(x.begin(), x.end(), 0);
    int n = binomial(d + k - 1, d);

    if (k == 0) {
        throw std::invalid_argument("k should be nonzero");
    }

    vector2d ru;
    std::vector<int> result = x;
    int m = result[1] - 2;
    int nn = result.back() + 1;

    if (result.front() + result.back() == d && result.back() < 3) {
        return iterate1(result);
    }
    else if (result.front() + result[1] == d) {
        ru.push_back(result);
        for (int i = 1; i < n; ++i) {
            if (result[1] != m - 1 && result.back() != 2) {
                result = next_partition(result);
                ru.push_back(result);
            }
            else {
                break;
            }
        }
    }
    else {
        for (int i = 1; i < n; ++i) {
            if (result.back() != nn) {
                result = next_partition(result);
                ru.push_back(result);
            }
            else {
                break;
            }
        }
    }

    return ru;
}

int main()
{
    int n = 4;
    int d = 4;

    vector2d gen = gen_block(n, d);
    for (std::vector<int> xa : gen)
    {
        std::cout << " xa= ";
        for (int xi : xa) {
            std::cout << xi << " ";
        }
        std::cout << std::endl;
    }
    for (std::vector<int> xa : gen)
    {
        /*  std::cout << " xa= ";
         for (int xi : xa) {
             std::cout << xi << " ";
         }
         std::cout << std::endl;
  */
        vector2d it = iterate(xa);

        for (std::vector<int> a : it)
        {
            for (int ai : a)
            {
                std::cout << ai << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }



    return 0;
}