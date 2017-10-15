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

#include "Matrix.hpp"

int hob (int num)
{
    if (!num)
        return 0;
    
    int ret = 1;
    
    while (num >>= 1)
        ret <<= 1;
    
    return ret;
}

// Run the Option 3 with size n, outputing the time elapsed in nanoseconds.
int main(int argc, const char * argv[]) {
    int n = 2048;
    if (argc > 1)
        n = atoi(argv[1]);
    if (n == 0)
        n = 400;
    // creates some n-matrixes, filling with random numbers.
    Matrix A(n);
    Matrix B(n);
    std::uniform_real_distribution<double> unif(-1,1);
    std::default_random_engine re;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
        {
            A.set(i, j, unif(re));
            B.set(i, j, unif(re));
        }
    {
        Matrix AN(A);
        Matrix BN(B);
        Matrix CN(n);
        // benchmark: Naive method
        auto begin = std::chrono::high_resolution_clock::now();
        AN.Multiply1(BN, CN);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "\t";
    }
    {
        int RS = hob(n - 1);
        RS <<= 1;
        Matrix AS(RS);
        Matrix BS(RS);
        Matrix CS(RS);
        for (int i = 0; i < RS; ++i)
            for (int j = 0; j < RS; ++j)
            {
                if (i < n && j < n)
                {
                    AS.set(i, j, A.get(i, j));
                    BS.set(i, j, B.get(i, j));
                }
                else
                {
                    AS.set(i, j, 0);
                    BS.set(i, j, 0);
                }
            }
        // benchmark: Strassen’s algorithm
        auto begin = std::chrono::high_resolution_clock::now();
        AS.Multiply2(BS, CS);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << std::endl;
    }
    return 0;
}

