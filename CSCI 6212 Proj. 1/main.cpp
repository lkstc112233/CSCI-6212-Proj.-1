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

int gcd(int n, int m)
{
    if (n < m)
        std::swap(n,m);
    int r = n % m;
    if (r == 0)
        return m;
    return gcd(m, r);
}

// Run the Option 3 with size n, outputing the time elapsed in nanoseconds.
int main(int argc, const char * argv[]) {
    int n = 100000;
    if (argc > 1)
        n = atoi(argv[1]);
    if (n == 0)
        n = 1000;
    // creates some n-item arrays, filling with 0.
    std::vector<int> a(n, 0);
    std::vector<int> b(n, 0);
    std::vector<int> c(n, 0);
    int Sum = 0;
    // benchmark
    auto begin = std::chrono::high_resolution_clock::now();
    for (int i = 1; i <= n; ++i)
        for (int j = i; j <= n; ++j)
        {
            for (int k = j; k <= n; ++k)
                Sum += a[i] * b[j] * c[k];
            if (gcd(i,j) == 1)
                j = n;
        }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << std::endl;
    return 0;
}

