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
    PROCESSED,
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
    bool BEOverlap = false;
    std::vector<MazeGraphNode> vertexList;
    MazeGraph(CellType *imageData, int width, int height)
    {
        image = imageData;
        int minBx = INT_MAX;
        int maxBx = INT_MIN;
        int minBy = INT_MAX;
        int maxBy = INT_MIN;
        int minEx = INT_MAX;
        int maxEx = INT_MIN;
        int minEy = INT_MAX;
        int maxEy = INT_MIN;
        for (int i = 0; i < height; ++i)
            for (int j = 0; j < width; ++j)
            {
                switch (imageData[i * width + j])
                {
                    case UNSCANNED_PATH:
                        break;
                    case OBSTACLE:
                        break;
                    case BEGINNING_POINT:
                        minBx = std::min(j, minBx);
                        minBy = std::min(i, minBy);
                        maxBx = std::max(j, maxBx);
                        maxBy = std::max(i, minBy);
                        break;
                    case ENDING_POINT:
                        minEx = std::min(j, minEx);
                        minEy = std::min(i, minEy);
                        maxEx = std::max(j, maxEx);
                        maxEy = std::max(i, minEy);
                        break;
                    default:
                        break;
                }
            }
        auto fill = [imageData, width](int minx, int miny, int maxx, int maxy, CellType type)
        {
            for (int i = 0; i <= maxy - miny; ++i)
                for (int j = 0; j <= maxx - minx; ++j)
                    imageData[(i + miny) * width + j + minx] = ANSWER_PATH;
        };
        if (minBx <= maxEx && minEx <= maxBx)
            if (minBy <= maxEy && minEy <= maxBy)
            {
                // fill(minBx, minBy, maxBx, maxBy, ANSWER_PATH);
                // fill(minEx, minEy, maxEx, maxEy, ANSWER_PATH);
                for (int i = 0; i <= maxBy - minBy; ++i)
                    for (int j = 0; j <= maxBx - minBx; ++j)
                        imageData[(i + minBy) * width + j + minBx] = ANSWER_PATH;
                for (int i = 0; i <= maxEy - minEy; ++i)
                    for (int j = 0; j <= maxEx - minEx; ++j)
                        imageData[(i + minEy) * width + j + minEx] = ANSWER_PATH;
                BEOverlap = true;
                return;
            }
        for (int i = 0; i <= maxBy - minBy; ++i)
            for (int j = 0; j <= maxBx - minBx; ++j)
                imageData[(i + minBy) * width + j + minBx] = PROCESSED;
        for (int i = 0; i <= maxEy - minEy; ++i)
            for (int j = 0; j <= maxEx - minEx; ++j)
                imageData[(i + minEy) * width + j + minEx] = PROCESSED;
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
    int arrayPos;
    int filesize;
    int width;
    int height;
    MazeGraph *graph;
    InputBitmap(const std::string& ifn)
    {
        std::ifstream ifs(ifn, std::ios::binary);
        ifs.seekg(2);
        ifs.read(reinterpret_cast<char*>(&filesize), 4);
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
    void write(std::string filename)
    {
        std::ofstream ofs(filename, std::ios::binary);
        ofs.write(header, headerSize);
        int rowSize = (24 * width + 31) / 32 * 4;
        for (int i = 0; i < height; ++i)
        {
            ofs.seekp(arrayPos + i * rowSize);
            for (int j = 0; j < width; ++j)
            {
                unsigned char red = 0;
                unsigned char green = 0;
                unsigned char blue = 0;
                CellType tp = graph->image[i * width + j];
                switch (tp) {
                    case UNSCANNED_PATH:
                        red = green = blue = 255;
                        break;
                    case ENDING_POINT:
                        green = 255;
                        break;
                    case BEGINNING_POINT:
                        red = 255;
                        break;
                    case ANSWER_PATH:
                        red = 255;
                        blue = 255;
                        break;
                    case SCANNED_PATH:
                        green = 255;
                        blue = 255;
                        break;
                    default:
                        break;
                }
                ofs.put(blue);
                ofs.put(green);
                ofs.put(red);
            }
        }
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
        return -1;
    }
    if (argc > 2)
    {
        outputFilename = argv[2];
    }
    
    // benchmark
    auto begin = std::chrono::high_resolution_clock::now();
    InputBitmap bitmap((std::string(argv[1])));
    bitmap.write(outputFilename);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << std::endl;
    return 0;
}

