// Author Sean Davis
#include "router.h"
#include <iostream>
#include <cstring>
#include <climits>
#include <cmath>
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

Router::Router(const Map1000 *map2, int width) : heap(width * width * 2)
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
    for (int i = 0; i < width_; i++)
    {
        delete [] map_[i];
    }

    delete [] map_;
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
    pathCount = 0;
    int numCitiesConnected = 1;
    Plot heapPlot;

    int shortestDistance[width_][width_];
    bool knownArray[width_][width_];
    Plot plots[width_][width_];

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

    int lastCityX;
    int lastCityY;
    
    while (numCitiesConnected < cityCount)
    {
        heap.deleteMin(heapPlot);
        int x = heapPlot.x;
        int y = heapPlot.y;
        short elevation = map_[heapPlot.x][heapPlot.y];

        if(x == cityPos[0].x && y == cityPos[0].y)
        {
            heapPlot.isCity = true;
        }

        if(knownArray[x][y] == false)
            knownArray[x][y] = true;
        else   
            continue;

        for (int i = 1; i < cityCount; i++)
        {
            if (x == cityPos[i].x && y == cityPos[i].y)
            {
                // adjacentPlot.weight = 0;           
                lastCityX = cityPos[i].x;
                lastCityY = cityPos[i].y;

                heapPlot.isCity = true;
                // Plot cityPlot = heapPlot;

                // Change city weight and path node weights to 0 and reinsert
             //   cityPlot.weight = 0;
             //   heap.insert(cityPlot);
    
                if(heapPlot.previousVertex.x != -1 && heapPlot.previousVertex.y != -1)
                {
                    Plot previousPlot = plots[heapPlot.previousVertex.x][heapPlot.previousVertex.y];

                    while(!previousPlot.isCity)
                    {
                     //   previousPlot.weight = 0;

                        previousPlot = plots[previousPlot.previousVertex.x][previousPlot.previousVertex.y];

                    //    heap.insert(previousPlot);
                    }
                }

                numCitiesConnected++;
            }
        }
                
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
            if (knownArray[adjacentPlot.x][adjacentPlot.y] == true)
                continue;
            adjacentPlot.weight = shortestDistance[adjacentPlot.x][adjacentPlot.y];

            
            if (adjacentPlot.x == -1 && adjacentPlot.y == -1)
                continue;

            short adjacentPlotElevation = map_[adjacentPlot.x][adjacentPlot.y];
            
            int weight = (int)(pow(abs(adjacentPlotElevation - elevation), 2) + 10);
            
            if (weight + heapPlot.weight < shortestDistance[adjacentPlot.x][adjacentPlot.y])
            {

                adjacentPlot.weight = weight;
                shortestDistance[adjacentPlot.x][adjacentPlot.y] = adjacentPlot.weight;
                // update pv
                adjacentPlot.previousVertex = PreviousVertex(x,y);
                plots[adjacentPlot.x][adjacentPlot.y] = adjacentPlot;
                
                heap.insert(adjacentPlot);
            }
        }
    }

    // Traceback
    Plot lastCityPlot = plots[lastCityX][lastCityY];

    while(lastCityPlot.previousVertex.x != -1 && lastCityPlot.previousVertex.y != -1)
    {
        paths[pathCount].startX = lastCityPlot.previousVertex.x;
        paths[pathCount].startY = lastCityPlot.previousVertex.y;
        paths[pathCount].endX = lastCityPlot.x;
        paths[pathCount].endY = lastCityPlot.y;
        pathCount++;

        lastCityPlot = plots[lastCityPlot.previousVertex.x][lastCityPlot.previousVertex.y];
    }

    cout << "hi" << endl;
} // findRoutes()
