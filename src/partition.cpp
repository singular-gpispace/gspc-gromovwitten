#include <iostream>
#include <vector>
#include "print.hpp"

using namespace std;
// Define the list type as a vector of vectors of integers
typedef vector<vector<int> > list;

list partition(int d, int N) {
    list L;

    if (N == 1) {
        vector<int> tmp;
        tmp.push_back(d);
        L.push_back(tmp);
        return L;
    } else {
        int count = 1;
        for (int i = 0; i <= d; ++i) {
            list tmp = partition(d - i, N - 1);
            for (int j = 0; j < tmp.size(); ++j) {
                vector<int> partitionList;
                partitionList.push_back(i);
                partitionList.insert(partitionList.end(), tmp[j].begin(), tmp[j].end());
                L.push_back(partitionList);
                count++;
            }
        }
        return L;
    }
}

int main() {
    int d, N;
    cout << "Enter the number of elements (d): ";
    cin >> d;
    cout << "Enter the size of the list (N): ";
    cin >> N;

    list result = partition(d, N);

    // Display the result
    cout << "Partitions of " << d << " elements in a list of size " << N << " are:\n";
    for (list::const_iterator it = result.begin(); it != result.end(); ++it) {
        cout << "[ ";
        for (vector<int>::const_iterator jt = it->begin(); jt != it->end(); ++jt) {
            cout << *jt << " ";
        }
        cout << "]\n";
    }

    return 0;
}
