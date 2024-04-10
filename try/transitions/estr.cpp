#include <iostream>
#include <vector>
#include <utility>

std::vector<std::vector<int>> pairsToVectorOfVectors(const std::vector<std::pair<int, int>>& pairs) {
    std::vector<std::vector<int>> result;
    for (const auto& pair : pairs) {
        std::vector<int> temp;
        temp.push_back(pair.first);
        temp.push_back(pair.second);
        result.push_back(temp);
    }
    return result;
}

std::vector<std::pair<int, int>> vectorOfVectorsToPairs(const std::vector<std::vector<int>>& vecOfVec) {
    std::vector<std::pair<int, int>> result;
    for (const auto& vec : vecOfVec) {
        if (vec.size() >= 2) {
            result.emplace_back(vec[0], vec[1]);
        }
    }
    return result;
}

int main() {
    std::vector<std::pair<int, int>> edges = {{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}};
    
    std::vector<std::vector<int>> result = pairsToVectorOfVectors(edges);

    // Printing the result
    for (const auto& vec : result) {
        for (int val : vec) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // Convert the result back to pairs
    std::vector<std::pair<int, int>> convertedEdges = vectorOfVectorsToPairs(result);

    // Printing the converted result
    for (const auto& pair : convertedEdges) {
        std::cout << "{" << pair.first << ", " << pair.second << "} ";
    }
    std::cout << std::endl;

    return 0;
}



void CartesianRecurse(std::vector<Sequence>& accum,
                      Sequence stack,
                      const std::vector<Sequence>& sequences,
                      int index) {
    std::stack<std::tuple<Sequence, int>> stackIndex;
    stackIndex.push(std::make_tuple(stack, index));
    
    while (!stackIndex.empty()) {
        auto [curStack, curIndex] = stackIndex.top();
        stackIndex.pop();
        
        Sequence sequence = sequences[curIndex];
        
        for (const auto& tuple : sequence) {
            curStack.push_back(tuple);
            
            if (curIndex == 0) {
                accum.push_back(curStack);
            } else {
                stackIndex.push(std::make_tuple(curStack, curIndex - 1));
            }
            
            curStack.pop_back();
        }
    }
}
std::vector<Sequence > CartesianProduct(const std::vector<Sequence > &sequences) {
    
    std::vector<Sequence > accum;
    Sequence  stack;
    
    if (!sequences.empty())
        CartesianRecurse(accum, stack, sequences, sequences.size() - 1);
    
    return accum;
}
bool m(const Sequence& u, std::vector<int>& v) {
    for (const auto& pp : u) {
        v[pp.first.first] += pp.first.second;
        v[pp.second.first] += pp.second.second;
    }
    return std::all_of(v.begin(), v.end(), [](int val) { return val == 0; });
}

std::vector<Sequence> mergetuple(const std::vector<Sequence>& uu) {
    std::vector<Sequence> res;
    int N = 0;
    for (const auto& subvec : uu) {
        for (const auto& elem : subvec) {
            N = std::max({N, elem.first.first, elem.second.first});
        }
    }
    bool muv=false;
    std::vector<int> v(N + 1, 0);

        for (const auto& pp : u) {
        v[pp.first.first] += pp.first.second;
        v[pp.second.first] += pp.second.second;
    }
     std::all_of(v.begin(), v.end(), [](int val) { return val == 0; });
        if (muv) {
            res.push_back(u);
        }
        std::fill(v.begin(), v.end(), 0);

 std::vector<Sequence > uu;
    Sequence  stack;
    
    if (!sequences.empty())
        CartesianRecurse(uu, stack, sequences, sequences.size() - 1);
        
    
    for (const auto& u : cartesian_product) {
        m_wrapper(u, v);
    }
    return res;
}