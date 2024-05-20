#include <iostream>
#include <vector>
#include <numeric>
//#include <algorithm>
//#include <stdexcept>
#include <sstream>


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

std::vector<std::vector<int>> flattenVector(const std::vector<std::vector<std::vector<int>>>& ge) {
    std::vector<std::vector<int>> flat_vector;
    for (const auto& innerVec1 : ge) {
        for (const auto& innerVec2 : innerVec1) {
            flat_vector.push_back(innerVec2);
        }
    }
    return flat_vector;
}
std::string vectorToStringv(const std::vector<std::vector<std::vector<int>>>& vec) {
    std::stringstream ss;
    for (const auto& innerVec1 : vec) {
        for (const auto& innerVec2 : innerVec1) {
            for (int val : innerVec2) {
                ss << val << ' ';
            }
            ss << '\n'; 
        }
        ss << '\n'; 
    }
    return ss.str();
}

std::vector<std::vector<std::vector<int>>> stringToVectorv(const std::string& a) {
    std::vector<std::vector<std::vector<int>>> result;
    std::stringstream ss(a);
    std::string line;
    std::vector<std::vector<int>> innerVec1;
    while (std::getline(ss, line)) {
        if (line.empty()) {
            if (!innerVec1.empty()) {
                result.push_back(innerVec1);
                innerVec1.clear();
            }
        } else {
            std::vector<int> innerVec2;
            std::stringstream lineStream(line);
            int num;
            while (lineStream >> num) {
                innerVec2.push_back(num);
            }
            innerVec1.push_back(innerVec2);
        }
    }
    if (!innerVec1.empty()) {
        result.push_back(innerVec1);
    }
    return result;
}

std::string vectorToString(const std::vector<std::vector<int>>& vec) {
    std::stringstream ss;
    for (const auto& innerVec : vec) {
        for (int val : innerVec) {
            ss << val << ' ';
        }
        ss << '\n'; 
    }
    return ss.str();
}

std::string vectorToStringd(const std::vector<std::vector<double>>& vec) {
    std::stringstream ss;
    for (const auto& innerVec : vec) {
        for (int val : innerVec) {
            ss << val << ' ';
        }
        ss << '\n'; 
    }
    return ss.str();
}

std::vector<std::vector<int>> stringToVector(const std::string& str) {
    std::vector<std::vector<int>> result;
    std::stringstream ss(str);
    std::string line;
    while (std::getline(ss, line)) {
        std::vector<int> innerVec;
        std::stringstream lineStream(line);
        int num;
        while (lineStream >> num) {
            innerVec.push_back(num);
        }
        result.push_back(innerVec);
    }
    return result;
}
std::string vectorToStringInt(const std::vector<int>& vec) {
    std::stringstream ss;
    for (int val : vec) {
        ss << val << ' ';
    }
    return ss.str();
}

std::vector<int> stringToVectorInt(const std::string& str) {
    std::vector<int> result;
    std::stringstream ss(str);
    int num;
    while (ss >> num) {
        result.push_back(num);
    }
    return result;
}
// Calculate binomial coefficient (n choose k)
int binomial(int n, int k) {
    if (k < 0 || k > n) {
        throw std::invalid_argument("Invalid arguments for binomial coefficient");
    }
    
    int result = 1;
    for (int i = 1; i <= k; ++i) {
        result *= (n - i + 1);
        result /= i;
    }
    return result;
}

std::vector<int> next_partition(std::vector<int> a) {
    int n = std::accumulate(a.begin(), a.end(), 0);
    int k = a.size();
    for (int i = k - 1; i >= 0; --i) {
        if (i == k - 1 && a[i] == n) {
            return a;
        } else {
            for (int j = i - 1; j >= 0; --j) {
                if (a[j] != 0) {
                    --a[j];
                    int ak = a[k - 1];
                    a[k - 1] = 0;
                    a[j + 1] = ak + 1;
                    return a;
                }
            }
        }
    }
    return a; 
}

std::vector<std::vector<int> > combination(std::vector<int> (*f)(std::vector<int>), int k, int d) {
    if (k == 0) {
        throw std::invalid_argument("k should be nonzero");
    }
   
    std::vector<int> x(k , 0);
    x[0] = d;
    int n = binomial(d + k - 1, d);
    std::vector<std::vector<int> > ru;
    std::vector<int> result = x;
    ru.push_back(x);
    for (int i = 1; i < n; ++i) {
        result = f(result);
        ru.push_back(result);
    }
    return ru;
}
std::vector<std::vector<int> > combinations(std::vector<int> (*f) (std::vector<int>), std::vector<int> x){
    int k=x.size();
    
    if (k == 0) {
        throw std::invalid_argument("k should be nonzero");
    }
    std::vector<std::vector<int> > ru;
    int d= std::accumulate(x.begin(),x.end(),0);
    std::vector<int> v(k , 0);
    v[0] = d;

    if (v==x){
     ru.push_back(v);
    }
    int n = binomial(d + k - 1, d);
    int e = d - x[0];
    std::vector<int> result = x;    
    std::vector<int> y(k, 0); 
    y[0] = x[0] - 1;
    y[k - 1] = e + 1;
    
    for (int i = 0; i < n; ++i) {
        if (result != y) {
            result = next_partition(result);
            ru.push_back(result);
        } else {
            break;
        }
    }
    return ru;
}
std::vector<std::vector<int> > generate_combination(std::vector<int> x) {
    return combinations(next_partition, x);
}

std::vector<std::vector<int> > sum_degree(int n, int d){
    std::vector<std::vector<int> > ru;
    for(int e=0; e<d; e++){
        std::vector<int> x(n,0);
        x[0]=d-e; 
        x[n-1]=e;
        ru.push_back(x);
    }
    return ru;
}
std::string generate_combinations_string(const std::string& v) {
    std::vector<std::vector<int>> ru = stringToVector(v);
    std::string combinationsString;
    for (std::vector<int>& x : ru) {
       std::vector<std::vector<int> > comb = generate_combination(x);
        std::string combinationString = vectorToString(comb);
        combinationsString += combinationString + '\n'; // Append combination to output string
    }
    return combinationsString;
}

/* 
int main() {
    std::vector<int> partition = {1, 0, 2, 0, 0, 1};
    std::vector<int> next = next_partition(partition);
    for (int num : next) {
            std::cout << num << " ";
        }
    std::cout<<std::endl;
    
    std::vector<std::vector<int> > partitions = combination(next_partition, 6, 4);
    for (const auto& partition : partitions) {
        for (int num : partition) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    std::vector<int> x = {4,0,0,0};
    std::vector<std::vector<int> > result = generate_combination(x);//combinations(next_partition, x);
    for (const auto& partition : result) {
        for (int val : partition) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
            std::cout << " sum_degree : "<<std::endl;
            std::vector<std::vector<int> > sum_deg=sum_degree(3,8);
            for (std::vector<int> v:sum_deg){
                for (int i :v){
                    std::cout<<i<< " " ;
                }
                        std::cout << std::endl;

            }

    return 0;
}
*/