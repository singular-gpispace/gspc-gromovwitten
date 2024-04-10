#include <iostream>
#include <unordered_set>
#include <vector>
#include <utility>
// Function to get the number of vertices in a FeynmanGraph
int getVerticesCount(const std::vector<std::pair<int, int>>& edges) {
    std::unordered_set<int> vertices;
    for (const auto& e : edges) {
        vertices.insert(e.first);
        vertices.insert(e.second);
    }
    return vertices.size();
}

// Function to get the number of edges in a FeynmanGraph
int getEdgesCount(const std::vector<std::pair<int, int>>& edges) {
    return edges.size();
}

// Function to print a FeynmanGraph
void printFeynmanGraph(const std::vector<std::pair<int, int>>& edges) {
    std::cout << "FeynmanGraph(";
    for (const auto& e : edges) {
        std::cout << "(" << e.first << ", " << e.second << ") ";
    }
    std::cout << ")";
}
int preimg(const std::vector<int>& L, int xi) {
    for (int i = 0; i < L.size(); i++) {
        if (L[i] == xi) {
            return i;
        }
    }
    return -1; // Return -1 if the element is not found
}
std::vector<int> flip_signature(const std::vector<std::pair<int, int>>& G, const std::vector<int>& p, const std::vector<int>& a) {
    std::vector<int> b(a.size(), 0);

    for (size_t i = 0; i < a.size(); ++i) {
        int ai = a[i];
        std::pair<int, int> ev = G[i];

        if (ai == 0 && ev.first != ev.second) {
            if (preimg(p, ev.first) < preimg(p, ev.second)) {
                b[i] = -1;
            } else {
                b[i] = 0;
            }
        } else if (ev.first == ev.second) {
            b[i] = -2;
        } else if (ai != 0 && ev.first != ev.second) {
            b[i] = ai;
        }
    }

    return b;
}


std::vector<int> flip_signature1(const std::vector<std::pair<int, int>>& G, const std::vector<int>& p, const std::vector<int>& a) {
    std::vector<int> b(a.size(), 0);

    for (size_t i = 0; i < a.size(); ++i) {
        int ai = a[i];
        std::pair<int, int> ev = G[i];

        if (ai == 0 && ev.first != ev.second) {
            int ii = -1;
            int ij = -1;
            for (size_t j = 0; j < p.size(); ++j) {
                if (p[j] == ev.first) {
                    ii = j;
                }
                if (p[j] == ev.second) {
                    ij = j;
                }
            }
                
            if (ii != -1 && ij != -1) {
                if (ii < ij) {
                    b[i] = -1;
                } else {
                    b[i] = 0;
                }
            }
        } else if (ev.first == ev.second) {
            b[i] = -2;
        } else if (ai != 0 && ev.first != ev.second) {
            b[i] = ai;
        }
    }

    return b;
}

// Main function for testing
int main() {
    std::vector<std::pair<int, int>> edges = {{1, 2}, {2, 3}, {3, 4}};

    std::cout << "Number of vertices: " << getVerticesCount(edges) << std::endl;
    std::cout << "Number of edges: " << edges.size() << std::endl;
    std::cout << "Edges: ";
    printFeynmanGraph(edges);
    std::cout << std::endl;

std::vector<int> a={0,1,0,0,3,1};
std::vector<int> o={2,3,4,1};
std::vector<int> fl=flip_signature(edges,o,a);
    std::cout << std::endl;

std::cout<<"flip_signature"<<std::endl;
for (int fi :fl){
    std::cout<<fi;
    std::cout << std::endl;

}
std::cout << std::endl;
std::vector<int> fl1=flip_signature1(edges,o,a);

std::cout<<"flip_signature1"<<std::endl;
for (int fi :fl1){
    std::cout<<fi;
    std::cout << std::endl;

}

    return 0;
}
