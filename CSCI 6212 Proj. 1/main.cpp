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

enum CellType
{
    UNSCANNED_PATH,
    SCANNED_PATH,
    ANSWER_PATH,
    OBSTACLE,
    BEGINNING_POINT,
    ENDING_POINT,
};

class MazeGraphNode
{
public:
    CellType type;
    int x;
    int y;
    int width;
    int height;
};

class MazeGraph
{
public:
    CellType *image;
    MazeGraph(CellType *imageData, int width, int height)
    {
        image = imageData;
    }
    ~MazeGraph()
    {
        delete[] image;
    }
};

class InputBitmap
{
public:
    char *header;
    int headerSize;
    int filesize;
    int width;
    int height;
    MazeGraph *graph;
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
        int rowSize = (24 * width + 31) / 32 * 4;
        
        CellType* graphData = new CellType[width * height];
        for (int i = 0; i < height; ++i)
        {
            ifs.seekg(arrayPos + i * rowSize);
            for (int j = 0; j < width; ++j)
            {
                char red;
                char green;
                char blue;
                ifs.read(&blue,1);
                ifs.read(&green,1);
                ifs.read(&red,1);
                CellType tp;
                if (red&&green&&blue)
                    tp = UNSCANNED_PATH;
                else if (green)
                    tp = ENDING_POINT;
                else if (red)
                    tp = BEGINNING_POINT;
                else
                    tp = OBSTACLE;
                graphData[i*width + j] = tp;
            }
        }
        graph = new MazeGraph(graphData, width, height);
    }
    ~InputBitmap()
    {
        delete[] header;
        delete graph;
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

