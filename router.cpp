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
    int numCitiesConnected = 0;
    Plot heapPlot;

    int shortestDistance[width_][width_];
    bool knownArray[width_][width_];
    char isCityArray[width_][width_];
    memset(isCityArray, '0', sizeof(isCityArray));
    for (int i = 0; i < cityCount; i++)
    {
        isCityArray[cityPos[i].x][cityPos[i].y] = '1';
    }
    Plot **plots;

    plots = new Plot*[width_];
    for (int i = 0; i < width_; i++)
    {
        plots[i] = new Plot[width_];
        for (int j = 0; j < width_; j++)
        {
            shortestDistance[i][j] = INT_MAX;
            knownArray[i][j] = false;
        }
    }
    
    // default Plot constructor implicitly sets weight to 0
    heap.insert(Plot(cityPos[0].x,cityPos[0].y));

    // int lastCityX;
    // int lastCityY;
    
    while (numCitiesConnected < cityCount)
    {
        heap.deleteMin(heapPlot);
        int x = heapPlot.x;
        int y = heapPlot.y;
        short elevation = map_[heapPlot.x][heapPlot.y];

        if(x == cityPos[0].x && y == cityPos[0].y)
        {
            heapPlot.isCity = true;
            plots[x][y] = heapPlot;
        }

        if(knownArray[x][y] == false)
            knownArray[x][y] = true;
        else   
            continue;

        // create 1k by 1k array of chars, set them to zero using memset, set 20 cities to true, check using the array 
        // put edges in paths in dijkstras
        if (isCityArray[x][y] == '1')
        {
            // adjacentPlot.weight = 0;           
            // lastCityX = x;
            // lastCityY = y;

            if(x != cityPos[0].x && y != cityPos[0].y)
            {
                heapPlot.isCity = true;
                Plot cityPlot = heapPlot;

                // Change city weight and path node weights to 0 and reinsert
                cityPlot.weight = 0;
                heap.insert(cityPlot);
            }

            if(heapPlot.previousVertex.x != -1 && heapPlot.previousVertex.y != -1)
            {
                Plot previousPlot = plots[heapPlot.previousVertex.x][heapPlot.previousVertex.y];

                paths[pathCount].startX = x;
                paths[pathCount].startY = y;
                paths[pathCount].endX = previousPlot.x;
                paths[pathCount].endY = previousPlot.y;
                pathCount++;

                while(isCityArray[previousPlot.x][previousPlot.y] != '1')
                {
                    paths[pathCount].startX = previousPlot.x;
                    paths[pathCount].startY = previousPlot.y;
                    paths[pathCount].endX = previousPlot.previousVertex.x;
                    paths[pathCount].endY = previousPlot.previousVertex.y;
                    pathCount++;

                    previousPlot.weight = 0;

                    heap.insert(previousPlot);

                    previousPlot = plots[previousPlot.previousVertex.x][previousPlot.previousVertex.y];

                }
            }

            numCitiesConnected++;
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

            // Put right after getAdjacentPlot and change to OR
            if (adjacentPlot.x == -1 || adjacentPlot.y == -1)
                continue;

            if (knownArray[adjacentPlot.x][adjacentPlot.y] == true)
                continue;
            adjacentPlot.weight = shortestDistance[adjacentPlot.x][adjacentPlot.y];

            short adjacentPlotElevation = map_[adjacentPlot.x][adjacentPlot.y];
            
            // Delete pow, replace with *, delete abs
            int weight = (int)((adjacentPlotElevation - elevation)*(adjacentPlotElevation - elevation) + 10);
            
            // change to adjacentPlot.weight after <
            if (weight + heapPlot.weight < shortestDistance[adjacentPlot.x][adjacentPlot.y])
            {

                adjacentPlot.weight = weight;
                shortestDistance[adjacentPlot.x][adjacentPlot.y] = adjacentPlot.weight;
                // update pv
                adjacentPlot.previousVertex = PreviousVertex(x,y);
                plots[adjacentPlot.x][adjacentPlot.y] = adjacentPlot;
                // heap should have x, y, and weight only  
                heap.insert(adjacentPlot);
            }
        }
    }
    // Traceback
    // Plot lastCityPlot = plots[lastCityX][lastCityY];

    // while(lastCityPlot.previousVertex.x != -1 && lastCityPlot.previousVertex.y != -1)
    // {
    //     paths[pathCount].startX = lastCityPlot.x;
    //     paths[pathCount].startY = lastCityPlot.y;
    //     paths[pathCount].endX = lastCityPlot.previousVertex.x;
    //     paths[pathCount].endY = lastCityPlot.previousVertex.y;
    //     pathCount++;

    //     lastCityPlot = plots[lastCityPlot.previousVertex.x][lastCityPlot.previousVertex.y];
    // }

    // for (int i = 0; i < pathCount; i++)
    // {
        // cout << "{startX = " << paths[i].startX << ", startY = " << paths[i].startY << ", endX = " << paths[i].endX << ", endY = " << paths[i].endY << "}" << endl;
    // }

    for (int i = 0; i < width_; i++)
    {
        delete [] plots[i];
    }

    delete [] plots;
} // findRoutes()
