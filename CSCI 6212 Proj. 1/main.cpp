//
//  main.cpp
//  CSCI 6212 Proj. 1
//
//  Created by 煦 柯 on 2017/8/29.
//  Copyright © 2017年 Must Studio. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <chrono>
#include <algorithm>

std::string outputFilename = "output.bmp";

enum Cell
{
    UNSCANNED_PATH,
    SCANNED_PATH,
    ANSWER_PATH,
    OBSTACLE,
    BEGINNING_POINT,
    END_POINT,
};

class MazeGraphNode
{
public:
    
};

class MazeGraph
{
    
};

class InputBitmap
{
public:
    char *header;
    int headerSize;
    int filesize;
    int width;
    int height;
    InputBitmap(std::string& ifn)
    {
        std::ifstream ifs(ifn, std::ios::binary);
        ifs.seekg(2);
        ifs.read(reinterpret_cast<char*>(&filesize), 4);
        int arrayPos;
        ifs.seekg(10);
        ifs.read(reinterpret_cast<char*>(&arrayPos), 4);
        ifs.seekg(14);
        ifs.read(reinterpret_cast<char*>(&headerSize), 4);
        ifs.read(reinterpret_cast<char*>(&width), 4);
        ifs.read(reinterpret_cast<char*>(&height), 4);
        headerSize += 14;
        header = new char[headerSize];
        ifs.seekg(0);
        ifs.read(header, headerSize);
    }
    ~InputBitmap()
    {
        delete[] header;
    }
    
};

// Run the Option 3 with size n, outputing the time elapsed in nanoseconds.
int main(int argc, const char * argv[]) {
    if (argc < 2)
    {
        std::cerr << "INPUT MAZE MISSING" << std::endl;
    }
    if (argc > 2)
    {
        outputFilename = argv[2];
    }
    
    
    // benchmark
    auto begin = std::chrono::high_resolution_clock::now();
    for (int i = 1; i <= m; ++i)
    {
        for (int j = 1; j <= n; ++j)
            METF(i, j);
        if (finiate[n].first != -1)
            break;
    }
    METF(m, n);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
    return 0;
}

