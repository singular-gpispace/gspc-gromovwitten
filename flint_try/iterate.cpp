#include <list>
#include <vector>
#include <numeric>
#include <stdexcept>
#include <iostream>
#include "../../code/boost/include/boost/variant.hpp"
#include "../../code/we/type/value.hpp"
#include "time_memory.hpp"
#include <unordered_set> // for std::unordered_set

using pnet_value = pnet::type::value::value_type;
using pnet_list = std::list<pnet_value>;
using pnet_list2d = std::list<std::list<pnet_value>>;
using vector2d = std::vector<std::vector<int>>;
using list_type = std::list<std::string>; // Define list_type as std::list<std::string>

int binomial(int n, int k)
{
    if (k < 0 || k > n)
    {
        throw std::invalid_argument("Invalid arguments for binomial coefficient");
    }

    int result = 1;
    for (int i = 1; i <= k; ++i)
    {
        result *= (n - i + 1);
        result /= i;
    }
    return result;
}
std::vector<int> next_partition(std::vector<int> a)
{
    int n = std::accumulate(a.begin(), a.end(), 0);
    int k = a.size();
    for (int i = k - 1; i >= 0; --i)
    {
        if (i == k - 1 && a[i] == n)
        {
            return a;
        }
        else
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
    }
    return a;
}
vector2d iterate(const std::vector<int> &xa)
{
    vector2d gen;

    int k = xa.size();
    if (k == 0)
    {
        throw std::invalid_argument("k should be nonzero");
    }
    int d = std::accumulate(xa.begin(), xa.end(), 0);
    std::vector<int> xv(k, 0);
    xv[0] = d;
    if (xv == xa)
    {
        gen.push_back(xa);
    }

    int factorial = binomial(d + k - 1, d);

    int e = d - xa[0];
    std::vector<int> a = xa;
    std::vector<int> y(k, 0);
    y[0] = xa[0] - 1;
    y[k - 1] = e + 1;

    for (int i = 0; i < factorial; ++i)
    {
        if (a != y)
        {
            {
                a = next_partition(a);
            }

            gen.push_back(a);
        }
        else
        {
            break;
        }
    }
    return gen;
}
/* pnet_list2d vec;
vector2d gen=iterate(xa);
for (std::vector<int>& a : gen) {
    pnet_list temp_a;
    for (int xi : a) {
        temp_a.push_back(pnet::type::value::value_type(xi));
    }
    vec.push_back(temp_a);
} */
int main()
{
    std::vector<int> aa = {1, 0, 3};
    vector2d gen = iterate(aa);
    for (std::vector<int> a : gen)
    {
        for (int ai : a)
        {
            std::cout << ai << " ";
        }
        std::cout << std::endl;
    }
    pnet_list2d vec;

    for (std::vector<int> &a : gen)
    {
        pnet_list temp_a;
        for (int xi : a)
        {
            temp_a.push_back(pnet::type::value::value_type(xi));
        }
        vec.push_back(temp_a);
    }

    std::cout << " from pnet to vec2d" << std::endl;

    for (const auto &inner_list : vec)
    {
        for (const auto &xi : inner_list)
        {
            if (auto ptr = boost::get<int>(&xi))
            {
                std::cout << *ptr << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    return 0;
}