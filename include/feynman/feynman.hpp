#ifndef FEYNMAN_HPP
#define FEYNMAN_HPP
#include <unordered_map>
#include <vector>
#include <tuple>
#include <iostream>
#include <unordered_set>
/*
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


//Sequence  cons0(const FeynmanGraph& G, int j, int N);
//Sequence  cons(const FeynmanGraph& G, int j, int N);
//Sequence  prot(const FeynmanGraph& G, int j, int a, int N);
*/
using Element = std::pair<std::pair<int, int>, std::pair<int, int>>;
using Sequence = std::vector<Element>;

std::vector<int> replaces(const std::vector<int>& arr);
int count_zero(const std::vector<int>& arr);
int preimg(const std::vector<int>& L, int xi);


//  hash function for vectors
struct VectorHash {
    size_t operator()(const std::vector<int>& vec) const {
        size_t hash = 0;
        for (int i : vec) {
            hash ^= std::hash<int>()(i);
        }
        return hash;
    }
};

//  equality  for vectors
struct VectorEqual {
    bool operator()(const std::vector<int>& lhs, const std::vector<int>& rhs) const {
        return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }
};

std::unordered_map<std::vector<int>, int, VectorHash, VectorEqual> count_member(const std::vector<std::vector<int> >& itr);
//std::vector<std::tuple<int, std::vector<int>>> signature_and_multiplicitie(const FeynmanGraph& G, const std::vector<int>& a);
std::vector<std::vector<int>> permutation(std::vector<int>& vec) ;
std::vector<std::tuple<int,std::vector<int>>> signature_and_multiplicitie(const std::vector<std::pair<int, int>>& G, const std::vector<int>& a);
//std::vector<int> flip_signature(const FeynmanGraph& G, const std::vector<int>& p, const std::vector<int>& a);
void sum_exp(std::vector<int>& res, const std::vector<std::vector<std::pair<int, int>>>& uu);

void CartesianRecurse(std::vector<Sequence>);
std::vector<Sequence > CartesianProduct(const std::vector<Sequence > &sequences);
bool m(const Sequence& u, std::vector<int>& v) ;
std::vector<Sequence> mergetuple(const std::vector<Sequence>& uu);
int sum_absolute_products(std::vector<Sequence> tt);
int feynman_integral(std::vector<std::pair<int, int>>& G, const std::vector<int>& a);
//std::vector<int> feynman_integral_degree(const FeynmanGraph& G,int d);
std::vector<std::vector<int> > sum_degree(int n, int d);

std::string vectorToString(const std::vector<std::vector<int>>& vec);
std::string vectorToStringd(const std::vector<std::vector<int>>& vec);

std::vector<std::vector<int>> stringToVector(const std::string& str);

std::string vectorToStringv(const std::vector<std::vector<std::vector<int>>>& vec);
std::vector<std::vector<std::vector<int>>> stringToVectorv(const std::string& str);

std::string vectorToStringInt(const std::vector<int>& vec);
std::vector<int> stringToVectorInt(const std::string& str);

std::vector<std::vector<int> > generate_combination(std::vector<int> x);
std::vector<std::vector<int>> flattenVector(const std::vector<std::vector<std::vector<int>>>& ge) ;
std::vector<std::vector<int>> pairsToVectorOfVectors(const std::vector<std::pair<int, int>>& pairs);
std::vector<std::pair<int, int>> vectorOfVectorsToPairs(const std::vector<std::vector<int>>& vecOfVec);
int binomial(int n, int k);



#endif // FEYNMAN_HPP