#include <iostream>
#include <vector>
#include <algorithm>

using vector2d = std::vector<std::vector<int>>;

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

int main() {
    int n = 4; // Provide value for n
    int d = 4; // Provide value for d
    auto result = gen_block(n, d);
    for (const auto& row : result) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }    return 0;
}
