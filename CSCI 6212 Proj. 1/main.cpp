//
//  main.cpp
//  CSCI 6212 Proj. 1
//
//  Created by 煦 柯 on 2017/8/29.
//  Copyright © 2017年 Must Studio. All rights reserved.
//

#include <iostream>
#include <utility>
#include <vector>
#include <chrono>
#include <algorithm>

// Table holding all values.
std::vector<std::vector<int>> results;

// m eggs, n floors
int METF(int m, int n)
{
    int reges = results[m][n];
    if (reges != -1)
        return reges;
    if (n == 1)
        return results[m][n] = 0;
    if (m == 1)
        return results[m][n] = m - 1;
    if (m <= 0) throw -1;
    int minimal = INT_MAX;
    for (int k = 1; k < n; ++k)
    {
        int temp = std::max(METF(m, n - k),METF(m - 1,k)) + 1;
        minimal = std::min(temp, minimal);
    }
    return results[m][n] = minimal;
}


// Run the Option 3 with size n, outputing the time elapsed in nanoseconds.
int main(int argc, const char * argv[]) {
    int n = 1024;
    int m = 1024;
    if (argc > 2)
    {
        m = atoi(argv[1]);
        n = atoi(argv[2]);
    }
    if (n == 0)
        n = 1024;
    if (m == 0)
        m = 1024;
    results = std::vector<std::vector<int>>(m + 1, std::vector<int>(n + 1, -1));
    // benchmark
    auto begin = std::chrono::high_resolution_clock::now();
    METF(m, n);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << std::endl;
    return 0;
}

