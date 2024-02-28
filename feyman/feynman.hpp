#ifndef FEYNMAN_HPP
#define FEYNMAN_HPP

#include <vector>
#include <tuple>
#include <iostream>
#include <unordered_set>

// Define Edge class
class Edge {
public:
    int src;
    int dst;

    Edge(int src, int dst);

};

// Define FeynmanGraph class
class FeynmanGraph {
private:
    std::vector<Edge> _edges;

public:
    FeynmanGraph(const std::vector<std::pair<int, int> >& edges);

    std::vector<Edge> getEdges() const;

    int nv() const;

    int ne() const;

    friend std::ostream& operator<<(std::ostream& os, const FeynmanGraph& G);

    std::vector<Edge> edges() const;

};

// Define FeynmanIntegral class
class FeynmanIntegral {
private:
    FeynmanGraph G;

public:
    FeynmanIntegral(const FeynmanGraph& G);

    const FeynmanGraph& getGraph() const;

};
std::vector<std::vector<int>>  partition(int d, int N) ;
std::vector<std::vector<int>> combination(std::vector<int> (*f)(std::vector<int>), int k, int d) ;
std::vector<int> next_partition(std::vector<int> a);

using Element = std::pair<std::pair<int, int>, std::pair<int, int>>;
using Sequence = std::vector<Element>;

Sequence  cons0(const FeynmanGraph& G, int j, int N);
Sequence  cons(const FeynmanGraph& G, int j, int N);
Sequence  prot(const FeynmanGraph& G, int j, int a, int N);

std::vector<int> replaces(const std::vector<int>& arr);
int count_zero(const std::vector<int>& arr);
int preimg(const std::vector<int>& L, int xi);


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

std::unordered_map<std::vector<int>, int, VectorHash, VectorEqual> count_member(const std::vector<std::vector<int> >& itr);
std::vector<std::tuple<int, std::vector<int>>> signature_and_multiplicitie(const FeynmanGraph& G, const std::vector<int>& a);
std::vector<std::vector<int>> permutation(std::vector<int>& vec) ;

std::vector<int> flip_signature(const FeynmanGraph& G, const std::vector<int>& p, const std::vector<int>& a);
void sum_exp(std::vector<int>& res, const std::vector<std::vector<std::pair<int, int>>>& uu);

void CartesianRecurse(std::vector<Sequence>);
std::vector<Sequence > CartesianProduct(const std::vector<Sequence > &sequences);
bool m(const Sequence& u, std::vector<int>& v) ;
std::vector<Sequence> mergetuple(const std::vector<Sequence>& uu);
double sum_absolute_products(std::vector<Sequence> tt);
double feynman_integral(const & G, const std::vector<int>& a);
std::vector<double> feynman_integral_degree(const FeynmanGraph& G,int d);
#endif // FEYNMAN_HPP