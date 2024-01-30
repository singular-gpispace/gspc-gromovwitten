#include <iostream>
#include <vector>
#include "print.hpp"
#include "parti.hpp"

using namespace std;

// Function to generate partitions
vector<vector<int>> generatePartitions(int d)
{
    vector<vector<int>> partitions;
    vector<int> p(d, 0);

    int k = 0;
    p[k] = d;

    while (true)
    {
        // Store the current partition
        partitions.push_back(vector<int>(p.begin(), p.begin() + k + 1));

        int rem_val = 0;
        while (k >= 0 && p[k] == 1)
        {
            rem_val += p[k];
            k--;
        }

        if (k < 0)
            break;

        p[k]--;
        rem_val++;

        while (rem_val > p[k])
        {
            p[k + 1] = p[k];
            rem_val = rem_val - p[k];
            k++;
        }

        p[k + 1] = rem_val;
        k++;
    }

    return partitions;
}

vector<vector<int>> parti(int d, int n)
{
    vector<vector<int>> result;

    // Generate partitions
    vector<vector<int>> partitions = generatePartitions(d);

    for (const auto &p : partitions)
    {
        if (p.size() < n)
        {
            // Add zeros to the end of the partition to make it length n
            int zeros_to_add = n - p.size();
            vector<int> temp = p;
            temp.insert(temp.end(), zeros_to_add, 0);
            result.push_back(temp);
        }
    }

    return result;
}

int main()
{
    int d = 4;
    int n = 6;
    auto result = parti(d, n);

    // Print the result
    for (const auto &vec : result)
    {
        printVector(vec);
    }

    return 0;
}
