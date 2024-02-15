#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include "feynman.hpp"

// Function to replace -1 with 0 and 0 with -1 in a vector
std::vector<int> replaces(const std::vector<int>& arr) {
    std::vector<int> result_arr;
    for (int x : arr) {
        result_arr.push_back(x == -1 ? 0 : (x == 0 ? -1 : x));
    }
    return result_arr;
}

// Function to count the number of zeros in a vector
int count_zero(const std::vector<int>& arr) {
    int count_zeros = 0;
    for (int element : arr) {
        if (element == 0) {
            count_zeros++;
        }
    }
    return count_zeros;
}

// Function to find the position of an element in a vector
int preimg(const std::vector<int>& L, int xi) {
    for (int i = 0; i < L.size(); i++) {
        if (L[i] == xi) {
            return i;
        }
    }
    return -1; // Return -1 if the element is not found
}

// Custom hash function for vectors
struct VectorHash {
    size_t operator()(const std::vector<int>& vec) const {
        size_t hash = 0;
        for (int i : vec) {
            hash ^= std::hash<int>()(i);
        }
        return hash;
    }
};

// Custom equality function for vectors
struct VectorEqual {
    bool operator()(const std::vector<int>& lhs, const std::vector<int>& rhs) const {
        return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }
};

// Function to count the occurrences of vectors in a vector of vectors
std::unordered_map<std::vector<int>, int, VectorHash, VectorEqual> count_member(const std::vector<std::vector<int>>& itr) {
    std::unordered_map<std::vector<int>, int, VectorHash, VectorEqual> d;
    for (const auto& vec : itr) {
        d[vec]++;
    }
    return d;
}
std::vector<int> flip_signature(const FeynmanGraph& G, const std::vector<int>& p, const std::vector<int>& a) {
    std::vector<Edge> ee = G.edges();
    std::vector<int> b(a.size(), 0);

    for (size_t i = 0; i < a.size(); ++i) {
        int ai = a[i];
        Edge ev = ee[i];

        if (ai == 0 && ev.src != ev.dst) {
            if (preimg(p, ev.src) < preimg(p, ev.dst)) {
                b[i] = -1;
            } else {
                b[i] = 0;
            }
        } else if (ev.src == ev.dst) {
            b[i] = -2;
        } else if (ai != 0 && ev.src != ev.dst) {
            b[i] = ai;
        }
    }

    return b;
}


int main() {
    std::vector<std::vector<int>> y = {
        {-1, 1, 2, 1, -1, -1},
        {-1, 1, 2, 1, 0, 0},
        {0, 1, 2, 1, -1, -1},
        {0, 1, 2, 1, -1, -1},
        {-1, 1, 2, 1, 0, 0},
        {0, 1, 2, 1, 0, 0}
    };

    auto counts = count_member(y);

    for (const auto& pair : counts) {
        std::cout << " {";
        for (size_t i = 0; i < pair.first.size(); ++i) {
            std::cout << pair.first[i];
            if (i != pair.first.size() - 1)
                std::cout << ", ";
        }
        std::cout << "} : " << pair.second << std::endl;
    }
    
    std::vector<int> yy = {0, 1, 2, 0, -1, -1};
    std::cout << " preimg  "  << preimg(yy, -1) << std::endl;
    std::cout << " count_zero " << count_zero(yy) << std::endl ;
    
    // Print the replaced vector
    std::vector<int> v = replaces(yy);
    std::cout << " replaces: {";
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i];
        if (i != v.size() - 1)
            std::cout << ", ";
    }
    std::cout << "}" << std::endl;

    std::vector<std::pair<int, int>> edges = {{1, 2}, {1, 2}, {1, 3},{2,4},{3,4},{3,4}};
    FeynmanGraph graph(edges);
    FeynmanIntegral integral(graph);
    std::vector<int> o ={ 1,2,3,4};
    std::vector<int> aa ={ 0,1,3,2,4,0};

    std::vector<int> vv=flip_signature(graph,o,aa);
    std::cout <<" flip_signature { ";
    for (size_t i = 0; i < v.size(); ++i) {
    std::cout <<vv[i];
    if (i != vv.back())
        std::cout << ", ";
    }
    std::cout<<"}" << std::endl;

    return 0;
}
