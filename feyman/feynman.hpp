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

#endif // FEYNMAN_HPP