#include <iostream>
#include <vector>
#include <tuple>

// Define Edge class
class Edge {
public:
    int src;
    int dst;

    Edge(int src, int dst) : src(src), dst(dst) {}
};

// Define cons0 function
std::vector<std::tuple<std::tuple<int, int>, std::tuple<int, int>>> cons0(const Edge& ee, int N) {
    std::vector<std::tuple<std::tuple<int, int>, std::tuple<int, int>>> v;
    for (int i = 1; i <= N; ++i) {
        if (ee.src < ee.dst) {
            v.push_back(std::make_tuple(std::make_tuple(ee.src, -i), std::make_tuple(ee.dst, +i)));
        } else {
            v.push_back(std::make_tuple(std::make_tuple(ee.dst, +i), std::make_tuple(ee.src, -i)));
        }
    }
    return v;
}

// Define cons function
std::vector<std::tuple<std::tuple<int, int>, std::tuple<int, int>>> cons(const Edge& ee, int N) {
    std::vector<std::tuple<std::tuple<int, int>, std::tuple<int, int>>> v;
    for (int i = 1; i <= N; ++i) {
        if (ee.dst < ee.src) {
            v.push_back(std::make_tuple(std::make_tuple(ee.dst, -i), std::make_tuple(ee.src, +i)));
        } else {
            v.push_back(std::make_tuple(std::make_tuple(ee.src, +i), std::make_tuple(ee.dst, -i)));
        }
    }
    return v;
}

// Define prot function
std::vector<std::tuple<std::tuple<int, int>, std::tuple<int, int>>> prot(const Edge& ee, int a, int N) {
    std::vector<std::tuple<std::tuple<int, int>, std::tuple<int, int>>> x_powers;
    for (int w = 1; w <= a; ++w) {
        if (a % w == 0) {
            x_powers.push_back(std::make_tuple(std::make_tuple(ee.src, +w), std::make_tuple(ee.dst, -w)));
            x_powers.push_back(std::make_tuple(std::make_tuple(ee.src, -w), std::make_tuple(ee.dst, +w)));
        }
    }
    return x_powers;
}

int main() {
    Edge e1(1, 2);
    int N = 5;

    // Test cons0
    auto result_cons0 = cons0(e1, N);
    std::cout << "cons0:\n";
    for (const auto& t : result_cons0) {
        std::cout << "((" << std::get<0>(std::get<0>(t)) << ", " << std::get<1>(std::get<0>(t)) << "), ";
        std::cout << "(" << std::get<0>(std::get<1>(t)) << ", " << std::get<1>(std::get<1>(t)) << "))\n";
    }

    // Test cons
    auto result_cons = cons(e1, N);
    std::cout << "\ncons:\n";
    for (const auto& t : result_cons) {
        std::cout << "((" << std::get<0>(std::get<0>(t)) << ", " << std::get<1>(std::get<0>(t)) << "), ";
        std::cout << "(" << std::get<0>(std::get<1>(t)) << ", " << std::get<1>(std::get<1>(t)) << "))\n";
    }

    // Test prot
    auto result_prot = prot(e1, 4, N);
    std::cout << "\nprot:\n";
    for (const auto& t : result_prot) {
        std::cout << "((" << std::get<0>(std::get<0>(t)) << ", " << std::get<1>(std::get<0>(t)) << "), ";
        std::cout << "(" << std::get<0>(std::get<1>(t)) << ", " << std::get<1>(std::get<1>(t)) << "))\n";
    }

    return 0;
}
