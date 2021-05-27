// Author Sean Davis
#include "router.h"
#include <iostream>
#include <cstring>
#include <climits>
using namespace std;


void Router::printMap()
{
    for(int i = 0; i < width_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            cout << map_[i][j] << " ";
        }
    }
}

Router::Router(const Map1000 *map2, int width) : heap(width * width)
{
    map_ = new short*[width];
    for (int i = 0; i < width; i++)
    {
        map_[i] = new short[width];
        for (int j = 0; j < width; j++)
        {
            map_[i][j] = map2->map1000[i][j];
        }
    }
    width_ = width;
    printMap(); 
} // Router()

Router::~Router()
{
}  // ~Router()

Plot Plot::getAdjacentPlot(int i, int width_)
{
    Plot invalidPlot(-1,-1);

    if (i == 0)
    {
        // top
        if(!isUpperLeftCorner(width_) && !isUpperRightCorner(width_) && !isTopEdge(width_))
            return getTopVertex();
    }
    else if (i == 1)
    {
        // top right
        if (!isUpperLeftCorner(width_) && !isRightEdge(width_) && !isUpperRightCorner(width_) && !isTopEdge(width_) && !isLowerRightCorner(width_))
            return getTopRightVertex();
    }
    else if (i == 2)
    {
        // right
        if (!isUpperRightCorner(width_) && !isLowerRightCorner(width_) && !isRightEdge(width_))
            return getRightVertex();
    }
    else if (i == 3)
    {
        // bottom right
        if(!isBottomEdge(width_) && !isRightEdge(width_) && !isLowerLeftCorner(width_) && !isLowerRightCorner(width_) && !isUpperRightCorner(width_))            return getBottomRightVertex();
    }
    else if (i == 4)
    {
        // bottom
        if (!isBottomEdge(width_) && !isLowerLeftCorner(width_) && !isLowerRightCorner(width_))
            return getBottomVertex();
    }
    else if (i == 5)
    {
        // bottom left
        if(!isBottomEdge(width_) && !isLeftEdge(width_) && !isLowerLeftCorner(width_) && !isLowerRightCorner(width_) && !isUpperLeftCorner(width_))
            return getBottomLeftVertex();
    }
    else if (i == 6)
    {
        // left 
        if (!isLeftEdge(width_) && !isUpperLeftCorner(width_) && !isLowerLeftCorner(width_))
            return getLeftVertex();
    }
    else if (i == 7)
    {
        // top left
        if(!isLeftEdge(width_) && !isTopEdge(width_) && !isUpperLeftCorner(width_) && !isLowerLeftCorner(width_) && !isUpperRightCorner(width_))
        return getTopLeftVertex();
    }
    
    return invalidPlot;
}

void Router::findRoutes(const CityPos *cityPos, int cityCount, Edge *paths, int &pathCount)
{
    int numCitiesConnected = 0;
    Plot heapPlot;

    int shortestDistance[width_][width_];
    bool knownArray[width_][width_];

    for (int i = 0; i < width_; i++)
    {
        for (int j = 0; j < width_; j++)
        {
            shortestDistance[i][j] = INT_MAX;
            knownArray[i][j] = false;
        }
    }
    
    // default Plot constructor implicitly sets weight to 0
    heap.insert(Plot(cityPos[0].x,cityPos[0].y));

    while (numCitiesConnected < cityCount)
    {
        heap.deleteMin(heapPlot);
        int x = heapPlot.x;
        int y = heapPlot.y;
        int elevation = map_[heapPlot.x][heapPlot.y];

        if(knownArray[x][y] == false)
            knownArray[x][y] = true;
        else   
            continue;
        // int adjacentVertexCount = 8;
        
        // if(heapPlot.isLeftEdge || heapPlot.isRightEdge || heapPlot.isTopEdge || heapPlot.isBottomEdge)
        // {
        //     adjacentVertexCount = 3;
        // }
        // else if(heapPlot.isLowerLeftCorner || heapPlot.isLowerRightCorner || heapPlot.isUpperLeftCorner || heapPlot.isUpperRightCorner)
        // {
        //     adjacentVertexCount = 2;
        // }
        
        for (int i = 0; i < 7; i++)
        {
            Plot adjacentPlot = heapPlot.getAdjacentPlot(i, width_);
            int adjacentPlotElevation = map_[adjacentPlot.x][adjacentPlot.y];
            
            int weight = pow(adjacentPlotElevation - elevation, 2) + 10;

            if (adjacentPlot.x == -1 && adjacentPlot.y == -1)
                continue;
            
            if (weight + heapPlot.weight < shortestDistance[adjacentPlot.x][adjacentPlot.y])
            {

                adjacentPlot.weight = weight;
                shortestDistance[adjacentPlot.x][adjacentPlot.y] = adjacentPlot.weight;
                
                paths[pathCount].startX = x;
                paths[pathCount].startY = y;
                paths[pathCount].endX = adjacentPlot.x;
                paths[pathCount].endY = adjacentPlot.y;
                pathCount++;

                for (int i = 0; i < cityCount; i++)
                {
                    if (adjacentPlot.x == cityPos[i].x && adjacentPlot.y == cityPos[i].y)
                    {
                        adjacentPlot.weight = 0;
                        numCitiesConnected++;
                    }
                }
                
                heap.insert(adjacentPlot);
            }
        }
    }
} // findRoutes()
