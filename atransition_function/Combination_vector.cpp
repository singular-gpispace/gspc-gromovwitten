#include <iostream>
#include <vector>
#include <numeric>
#include <sstream>
int main(){
     std::vector<std::vector<int>>
    std::vector<std::vector<int> > result2;
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
        }        int e = d - x[0];
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
         sss.str();

        return 0;
}