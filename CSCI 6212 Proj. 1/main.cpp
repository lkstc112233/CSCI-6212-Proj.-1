//
//  main.cpp
//  CSCI 6212 Proj. 1
//
//  Created by 煦 柯 on 2017/8/29.
//  Copyright © 2017年 Must Studio. All rights reserved.
//

#include <iostream>
#include <utility>

int gcd(int n, int m)
{
    if (n < m)
        std::swap(n,m);
    int r = n % m;
    if (r == 0)
        return m;
    return gcd(m, r);
}


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
