#include <iostream>
#include <vector>
#include <algorithm> // for std::fill
#include <numeric>
using vector2d = std::vector<std::vector<int>>;

std::vector<int> comp_for(std::vector<int>& c) {
    int k = c.size();
    if (k == 1) {
        fill(c.begin(), c.end(), 0);
        return c;
    }
    std::vector<int> d(k);
    for (int i = 0; i < k; ++i) {
        d[i] = c[i] - 1;
    }

    int j = k;

    for (int i = 0; i < k; ++i) {
        if (d[j - 1] > 0) {
            d[j - 1] = 0;
            d[j - 2] += 1;
            d[k - 1] = c[j - 1] - 2;
            for (int l = 0; l < k; ++l) {
                c[l] = d[l] + 1;
            }
            return c;
        }
        else {
            j -= 1;
            if (j == 1) {
                fill(c.begin(), c.end(), 0);
                return c;
            }
        }
    }
    return c;
}
std::vector<int> next_composition(std::vector<int> s) {
    std::transform(s.begin(), s.end(), s.begin(), [](int x) { return x + 1; });
    s = comp_for(s);
    std::transform(s.begin(), s.end(), s.begin(), [](int x) { return x - 1; });
    return s;
}
vector2d iterate_composition(const std::vector<int>& xa)
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
    int e = xa[0] + 1;
    std::vector<int> a = xa;
    std::vector<int> y(k, 0);
    y[0] = xa[0] - 1;
    y[k - 1] = e + 1;

    while (true)
    {
        if (a[0] != e)
        {
            gen.push_back(a);
            a = next_composition(a);

        }
        else
        {
            break;
        }
    }
    return gen;
}
vector2d compos(int n, int d)
{
    vector2d gen;
    std::vector<int> xv(n, 0);
    xv[n - 1] = d;

    while (xv[0] != d) {
        gen.push_back(xv);
        xv = next_composition(xv);
    }
    gen.push_back(xv);

    return gen;
}
int main() {
    std::vector<int> c = { 1, 1, 5 }; // Initial composition
    int n = 9;
    int d = 20;
    vector2d ru = compos(n, d);
    int com = ru.size();
    std::cout << " size :  " << com << std::endl;
    std::cout << " list composition : " << std::endl;

    for (const auto& a : ru) {
        for (int ai : a) {
            std::cout << ai << " ";
        }
        std::cout << std::endl;
    }

    /*  std::vector<int> s = { 0, 3, 1 };
     std::vector<int> next = next_composition(s);

     std::cout << "Next composition: ";
     for (int x : next) {
         std::cout << x << " ";
     }
     std::cout << std::endl;
     std::cout << std::endl;

     std::vector<int> ss = { 1, 0, 3 };

     vector2d itcomp = iterate_composition(ss);
     std::cout << " list composition : " << std::endl;

     for (const auto& a : itcomp) {
         for (int ai : a) {
             std::cout << ai << " ";
         }
         std::cout << std::endl;
     } */
    return 0;
}