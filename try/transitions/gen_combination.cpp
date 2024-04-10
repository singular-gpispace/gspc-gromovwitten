
#include <iostream>
#include <vector>
#include <numeric>
//#include <algorithm>
//#include <stdexcept>
#include <sstream>
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
    int dd = std::accumulate(a.begin(), a.end(), 0);
    int kk = a.size();
    for (int i = kk - 1; i >= 0; --i) {
        if (i == kk - 1 && a[i] == dd) {
            return a;
        } else {
            for (int j = i - 1; j >= 0; --j) {
                if (a[j] != 0) {
                    --a[j];
                    int ak = a[kk - 1];
                    a[kk - 1] = 0;
                    a[j + 1] = ak + 1;
                    return a;
                }
            }
        }
    }
    return a; 
}

std::vector<std::vector<int> > generate_combination(std::vector<int> x){
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

    int n = 1;
    int nn=d + k - 1;
    int k1=d;
    for (int i = 1; i <= k1; ++i) {
        n *= (nn - i + 1);
        n /= i;
    }
   // int n = binomial(d + k - 1, d);
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

std::vector<std::vector<int>> stringToVector(const std::string& str) {
                std::vector<std::vector<int>> result2;
                std::stringstream ss(str);
                std::string line;
                while (std::getline(ss, line)) {
                    std::vector<int> innerVec;
                    std::stringstream lineStream(line);
                    int num;
                    while (lineStream >> num) {
                        innerVec.push_back(num);
                    }
                    result2.push_back(innerVec);
                }
                return result2;
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
std::vector<std::vector<int> > generate_combination1(std::vector<int> x){
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
    std::vector<int> a = x;    
    std::vector<int> y(k, 0); 
    y[0] = x[0] - 1;
    y[k - 1] = e + 1;
    
    for (int i = 0; i < n; ++i) {
        if (a != y) {
            //a = next_partition(a);
            for (int i = k - 1; i >= 0; --i) {
                if (i == k - 1 && a[i] == d) {
                     //ru.push_back(a);
                    break;
                } else {
                    for (int j = i - 1; j >= 0; --j) {
                        if (a[j] != 0) {
                            --a[j];
                            int ak = a[k - 1];
                            a[k - 1] = 0;
                            a[j + 1] = ak + 1;
                           // ru.push_back(a);
                            break;                       
                            }
                    }
                }
                //ru.push_back(a);
                  break;
            }
            ru.push_back(a);
        } else {
            break;
        }
    }
    return ru;
}

std::string sum_degree(int n, int d) {
    std::vector<std::vector<int>> ru; 

    for (int e = 0; e < d; e++) {
        std::vector<int> x(n, 0);
        x[0] = d - e;
        x[n - 1] = e;
        ru.push_back(x);
    }

    std::stringstream ss;
    for (const auto& innerVec : ru) {
        for (int val : innerVec) {
            ss << val << ' ';
        }
        ss << '\n'; 
    }
    return ss.str();
}


std::vector<std::vector<std::vector<int>> > generate_combination2(std::string& x){
    std::vector<std::vector<int>> result2;
                std::stringstream ss(x);
                std::string line;
                while (std::getline(ss, line)) {
                    std::vector<int> innerVec;
                    std::stringstream lineStream(line);
                    int num;
                    while (lineStream >> num) {
                        innerVec.push_back(num);
                    }
                    result2.push_back(innerVec);
                }
                std::vector<std::vector<int>> xx=result2;
                std::vector<std::vector<std::vector<int>> > ge;
            for( std::vector<int> x:xx){
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
    std::vector<int> a = x;    
    std::vector<int> y(k, 0); 
    y[0] = x[0] - 1;
    y[k - 1] = e + 1;
    
    for (int i = 0; i < n; ++i) {
        if (a != y) {
            //a = next_partition(a);
            for (int i = k - 1; i >= 0; --i) {
                if (i == k - 1 && a[i] == d) {
                     //ru.push_back(a);
                    break;
                } else {
                    for (int j = i - 1; j >= 0; --j) {
                        if (a[j] != 0) {
                            --a[j];
                            int ak = a[k - 1];
                            a[k - 1] = 0;
                            a[j + 1] = ak + 1;
                           // ru.push_back(a);
                            break;                       
                            }
                    }
                }
                //ru.push_back(a);
                  break;
            }
            ru.push_back(a);
        } else {
            break;
        }
    }
         ge.push_back(ru);      
              }
    
    return ge;
}


std::string generate_combination3(std::string& v){
    std::vector<std::vector<int>> result2;
    std::stringstream ss(v);
    std::string line;
    while (std::getline(ss, line)) {
        std::vector<int> innerVec;
        std::stringstream lineStream(line);
        int num;
        while (lineStream >> num) {
            innerVec.push_back(num);
        }
        result2.push_back(innerVec);
    }
    std::vector<std::vector<int>> xx=result2;
    std::vector<std::vector<std::vector<int>> > ge;
    for( std::vector<int> x:xx){
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
        int n = 1;
        int nn=d + k - 1;
        int k1=d;
        for (int i = 1; i <= k1; ++i) {
            n *= (nn - i + 1);
            n /= i;
        }
        int e = d - x[0];
        std::vector<int> a = x;    
        std::vector<int> y(k, 0); 
        y[0] = x[0] - 1;
        y[k - 1] = e + 1;
        
        for (int i = 0; i < n; ++i) {
            if (a != y) {
                //a = next_partition(a);
                for (int i = k - 1; i >= 0; --i) {
                    if (i == k - 1 && a[i] == d) {
                        //ru.push_back(a);
                        break;
                    } else {
                        for (int j = i - 1; j >= 0; --j) {
                            if (a[j] != 0) {
                                --a[j];
                                int ak = a[k - 1];
                                a[k - 1] = 0;
                                a[j + 1] = ak + 1;
                            // ru.push_back(a);
                                break;                       
                                }
                        }
                    }
                    //ru.push_back(a);
                    break;
                }
                ru.push_back(a);
            } else {
                break;
            }
        }
        ge.push_back(ru);      
    }
        std::stringstream sss;

        for (const auto& innerVec1 : ge) {
            for (const auto& innerVec2 : innerVec1) {
                for (int val : innerVec2) {
                    sss << val << ' ';
                }
                sss << '\n'; 
            }
            sss << '\n'; 
        }
        return sss.str();
}
int main() {
    /*std::vector<int> partition = {1, 0, 2, 0, 0, 1};
    std::vector<int> next = next_partition(partition);
    for (int num : next) {
            std::cout << num << " ";
        }
    std::cout<<std::endl;
 
   

    std::vector<int> x = {1, 0, 3};
    std::vector<std::vector<int> > result1 = generate_combination1(x);//combinations(next_partition, x);
    for (const auto& partition : result1) {
        for (int val : partition) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
            std::cout << std::endl;

        std::cout <<"sum_degree" <<std::endl;

  int n = 3; 
    int d = 4; 
    std::string xx = sum_degree(n, d);
    
    // Print the result
    std::cout << xx;
    std::cout << std::endl;

        std::cout <<"all_gen" <<std::endl;

    std::vector<std::vector<std::vector<int> > > result2 = generate_combination2(xx);//combinations(next_partition, x);
    for (const auto& partition : result2) {
        for (const auto& val : partition) {
             for (int v : val) {
            std::cout << v << " ";
            }
             std::cout << std::endl;
        }
       
    }
    */
   int n = 3; 
    int d = 4; 
    std::string xxx = sum_degree(n, d);
    std::cout << std::endl;

        std::cout <<"all_gen1" <<std::endl;
      std::string xx1 = generate_combination3(xxx);
    
    // Print the result
    std::cout << xx1;
    
    return 0;
}