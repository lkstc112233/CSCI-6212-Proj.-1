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
#include <random>

// Run the Option 3 with size n, outputing the time elapsed in nanoseconds.
int main(int argc, const char * argv[]) {
    int n = 1024;
    int m = 1024;
    if (argc > 2)
    {
        n = atoi(argv[1]);
        m = atoi(argv[2]);
    }
    if (n == 0)
        n = 1024;
    if (m == 0)
        m = 1024;
    // benchmark
    auto begin = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "\t";
    return 0;
}

