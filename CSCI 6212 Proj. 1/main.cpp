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
#include <list>
#include <chrono>
#include <algorithm>
#include <string>
#include <functional>
#include <climits>

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

class MazeGraphNode;
class CellNode
{
public:
    CellType type;
    MazeGraphNode* belonging;
    CellNode():belonging(nullptr){}
};

class MazeGraphNode
{
public:
    CellType type;
    int x1;
    int y1;
    int x2;
    int y2;
    MazeGraphNode* directFrom;
    MazeGraphNode(int minx, int miny, int maxx, int maxy, CellType tp, MazeGraphNode* ori = nullptr)
    : x1(minx)
    , x2(maxx)
    , y1(miny)
    , y2(maxy)
    , type(tp)
    , directFrom(ori)
    {}
};

enum EdgeGrowDirection
{
    X_INCREASE,
    Y_INCREASE,
    X_DECREASE,
    Y_DECREASE,
};

class Edge
{
public:
    EdgeGrowDirection grow;
    int minbound;
    int maxbound;
    int line;
    MazeGraphNode* origin;
    Edge(int min, int max, EdgeGrowDirection dir, int baseline, MazeGraphNode* ori)
    : minbound(min)
    , maxbound(max)
    , grow(dir)
    , line(baseline)
    , origin(ori)
    {}
};

class MazeGraph
{
public:
    CellNode *image;
    bool BEOverlap = false;
    std::vector<MazeGraphNode*> vertexes;
    MazeGraph(CellNode *imageData, int width, int height)
    {
        std::vector<MazeGraphNode*>& vertexList = vertexes;
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
                switch (imageData[i * width + j].type)
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
        auto fill = [imageData, width](int minx, int miny, int maxx, int maxy, CellType type, MazeGraphNode* belong = nullptr)
        {
            for (int i = 0; i <= maxy - miny; ++i)
                for (int j = 0; j <= maxx - minx; ++j)
                {
                    imageData[(i + miny) * width + j + minx].type = type;
                    imageData[(i + miny) * width + j + minx].belonging = belong;
                }
        };
        if (minBx <= maxEx && minEx <= maxBx)
            if (minBy <= maxEy && minEy <= maxBy)
            {
                fill(minBx, minBy, maxBx, maxBy, BEGINNING_POINT);
                fill(minEx, minEy, maxEx, maxEy, ENDING_POINT);
                BEOverlap = true;
                return;
            }
        auto addVertex = [&vertexList, &fill](int minx, int miny, int maxx, int maxy, CellType type, MazeGraphNode* ori = nullptr) -> MazeGraphNode*
        {
            vertexList.emplace_back(new MazeGraphNode(minx, miny, maxx, maxy, type, ori));
            auto p = vertexList.back();
            fill(minx, miny, maxx, maxy, PROCESSED, p);
            return p;
        };
        auto beginVertexNode = addVertex(minBx, minBy, maxBx, maxBy, BEGINNING_POINT);
        addVertex(minEx, minEy, maxEx, maxEy, ENDING_POINT);
        std::list<Edge> edges;
        auto addEdges = [&edges, imageData, width, height](int min, int max, MazeGraphNode* node,EdgeGrowDirection direction, int line) -> bool
        {
            // Edge check
            if (min < 0) min = 0;
            if (line < 0) line = 0;
            switch (direction) {
                case X_DECREASE:
                case X_INCREASE:
                    if (max >= height)
                        max = height - 1;
                    if (line >= width)
                        line = width - 1;
                    break;
                case Y_DECREASE:
                case Y_INCREASE:
                    if (max >= width)
                        max = width - 1;
                    if (line >= height)
                        line = height - 1;
                    break;
                default:
                    break;
            }
            if (min > max) throw -1;
            int current = min;
            for (int i = 0; i <= max - min; ++i)
            {
                int index;
                switch (direction) {
                    case X_DECREASE:
                    case X_INCREASE:
                        index = width * (min + i) + line;
                        break;
                    case Y_DECREASE:
                    case Y_INCREASE:
                        index = width * line + (min + i);
                        break;
                    default:
                        break;
                }
                if (imageData[index].type == UNSCANNED_PATH)
                    imageData[index].type = PROCESSED;
                else if (imageData[index].belonging && imageData[index].belonging->type == ENDING_POINT)
                    return true;
                else
                {
                    if (current != i + min)
                    {
                        edges.emplace_back(current, min + i - 1, direction, line, node);
                    }
                    current = i + min + 1;
                }
            }
            if (current <= max)
                edges.emplace_back(current, max, direction, line, node);
            return false;
        };
        bool isEnd = false;
        isEnd |= addEdges(minBx, maxBx, beginVertexNode, Y_INCREASE, maxBy + 1);
        isEnd |= addEdges(minBx, maxBx, beginVertexNode, Y_DECREASE, minBy - 1);
        isEnd |= addEdges(minBy, maxBy, beginVertexNode, X_INCREASE, maxBx + 1);
        isEnd |= addEdges(minBy, maxBy, beginVertexNode, X_DECREASE, minBx - 1);
        if (isEnd)
        {
            vertexList[1]->directFrom = beginVertexNode;
            return;
        }
        auto expandEdge = [&addEdges, &addVertex, imageData, width, height, &vertexList](const Edge& e) -> bool
        {
            int line = e.line;
            bool expanding = true;
            while (expanding)
            {
                int x = -1;
                int y = -1;
                switch (e.grow) {
                    case X_DECREASE:
                        line -= 2;
                    case X_INCREASE:
                        line += 1;
                        if (line >= width || line < 0)
                        {
                            expanding = false;
                            break;
                        }
                        y = e.minbound;
                        x = line;
                        break;
                    case Y_DECREASE:
                        line -= 2;
                    case Y_INCREASE:
                        line += 1;
                        if (line >= height || line < 0)
                        {
                            expanding = false;
                            break;
                        }
                        x = e.minbound;
                        y = line;
                        break;
                    default:
                        break;
                }
                if (!expanding)
                    break;
                auto checkCanContinue = [imageData, &expanding](int index)
                {
                    if (imageData[index].type != UNSCANNED_PATH)
                        expanding = false;
                };
                auto markCheckedDots = [imageData](int index)
                {
                    imageData[index].type = PROCESSED;
                };
                auto iterate = [x, y, &e, width, &expanding](std::function<void(int)> f)
                {
                    for (int i = 0; expanding && i <= e.maxbound - e.minbound; ++i)
                    {
                        int index;
                        int xl = x;
                        int yl = y;
                        switch (e.grow) {
                            case X_DECREASE:
                            case X_INCREASE:
                                yl += i;
                                break;
                            case Y_DECREASE:
                            case Y_INCREASE:
                                xl += i;
                                break;
                            default:
                                break;
                        }
                        index = width * yl + xl;
                        f(index);
                    }
                };
                iterate(checkCanContinue);
                if (expanding)
                    iterate(markCheckedDots);
            }
            int lineold = line;
            bool isEnd = false;
            switch (e.grow) {
                case X_DECREASE:
                case Y_DECREASE:
                    line += 1;
                    break;
                case X_INCREASE:
                case Y_INCREASE:
                    line -= 1;
                    break;
            }
            MazeGraphNode* newVertex;
            switch (e.grow) {
                case X_DECREASE:
                case X_INCREASE:
                    newVertex = addVertex(std::min(line, e.line), e.minbound, std::max(line, e.line), e.maxbound, SCANNED_PATH, e.origin);
                    isEnd |= addEdges(std::min(line, e.line), std::max(line, e.line), newVertex, Y_INCREASE, e.maxbound + 1);
                    isEnd |= addEdges(std::min(line, e.line), std::max(line, e.line), newVertex, Y_DECREASE, e.minbound - 1);
                    break;
                case Y_DECREASE:
                case Y_INCREASE:
                    newVertex = addVertex(e.minbound, std::min(line, e.line), e.maxbound, std::max(line, e.line), SCANNED_PATH, e.origin);
                    isEnd |= addEdges(std::min(line, e.line), std::max(line, e.line), newVertex, X_INCREASE, e.maxbound + 1);
                    isEnd |= addEdges(std::min(line, e.line), std::max(line, e.line), newVertex, X_DECREASE, e.minbound - 1);
                    break;
                default:
                    newVertex = nullptr;
                    break;
            }
            isEnd |= addEdges(e.minbound, e.maxbound, newVertex, e.grow, lineold);
            if (isEnd)
            {
                vertexList[1]->directFrom = newVertex;
                return true;
            }
            return false;
        };
        while (edges.size())
        {
            auto e = edges.front();
            edges.pop_front();
            if (expandEdge(e))
                break;
        }
        auto iterator = vertexes[1]->directFrom;
        while (iterator && iterator != beginVertexNode)
        {
            iterator->type = ANSWER_PATH;
            iterator = iterator->directFrom;
        }
    }
    ~MazeGraph()
    {
        for (auto p : vertexes)
            delete p;
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
        
        CellNode* graphData = new CellNode[width * height];
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
                graphData[i*width + j].type = tp;
            }
        }
        graph = new MazeGraph(graphData, width, height);
    }
    void write(std::string filename)
    {
        for (auto v : graph->vertexes)
        {
            for (int i = v->x1; i <= v->x2; ++i)
                for (int j = v->y1; j <= v->y2; ++j)
                {
                    graph->image[width * j + i].type = v->type;
                }
        }
        std::ofstream ofs(filename, std::ios::binary);
        ofs.write(header, headerSize);
        int rowSize = (24 * width + 31) / 32 * 4;
        for (int i = 0; i < height; ++i)
        {
            int charCounter = 0;
            ofs.seekp(arrayPos + i * rowSize);
            for (int j = 0; j < width; ++j)
            {
                unsigned char red = 0;
                unsigned char green = 0;
                unsigned char blue = 0;
                CellType tp = graph->image[i * width + j].type;
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
                    case PROCESSED:
                        blue = 255;
                    default:
                        break;
                }
                ofs.put(blue);
                ofs.put(green);
                ofs.put(red);
                charCounter += 3;
            }
            switch(charCounter % 4)
            {
                case 1:
                    ofs.put(0);
                case 2:
                    ofs.put(0);
                case 3:
                    ofs.put(0);
                case 0:
                    break;
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

