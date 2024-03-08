#include "feynman.hpp"
Edge::Edge(int src, int dst) : src(src), dst(dst) {}

// Constructor  for FeynmanGraph
FeynmanGraph::FeynmanGraph(const std::vector<std::pair<int, int>>& edges) {
    for (const auto& e : edges) {
        _edges.emplace_back(e.first, e.second);
    }
}

std::vector<Edge> FeynmanGraph::getEdges() const {
    return _edges;
}

int FeynmanGraph::nv() const {
    std::unordered_set<int> vertices;
    for (const auto& e : _edges) {
        vertices.insert(e.src);
        vertices.insert(e.dst);
    }
    return vertices.size();
}

int FeynmanGraph::ne() const {
    return _edges.size();
}

std::ostream& operator<<(std::ostream& os, const FeynmanGraph& G) {
    os << "FeynmanGraph(";
    for (const auto& e : G._edges) {
        os << "(" << e.src << ", " << e.dst << ") ";
    }
    os << ")";
    return os;
}

std::vector<Edge> FeynmanGraph::edges() const {
    return _edges;
}

// Constructor   FeynmanIntegral
FeynmanIntegral::FeynmanIntegral(const FeynmanGraph& G) : G(G) {}

const FeynmanGraph& FeynmanIntegral::getGraph() const {
    return G;
}

/* Main function for testing
int main() {
    std::vector<std::pair<int, int>> edges = {{1, 2}, {2, 3}, {3, 4}};
    FeynmanGraph graph(edges);
    FeynmanIntegral integral(graph);

    std::cout << "Number of vertices: " << integral.getGraph().nv() << std::endl;
    std::cout << "Number of edges: " << integral.getGraph().ne() << std::endl;
    std::cout << "Edges: ";
    for (const auto& edge : integral.getGraph().edges()) {
        std::cout << "(" << edge.src << ", " << edge.dst << ") ";
    }
    std::cout << std::endl;

    return 0;
}
*/