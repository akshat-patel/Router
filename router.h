// Author: Sean Davis

#ifndef ROUTER_H
  #define ROUTER_H

#include "RouterRunner.h"
#include "BinaryHeap.h"
#include <cmath>

class PreviousVertex
{
  public:
    PreviousVertex(int x_, int y_) : x(x_), y(y_) {}
    PreviousVertex() : x(-1), y(-1) {}
    int x;
    int y;
};
class Plot
{
  public:
    Plot(): previousVertex(-1, -1), weight(-1), x(-1), y(-1), isCity(false) {}
    Plot(int x_, int y_, int change_in_elevation) : previousVertex(-1, -1), x(x_), y(y_), isCity(false)  
    {
      weight = (int) pow(change_in_elevation, 2) + 10;
    }
    Plot(int x_, int y_) : previousVertex(-1, -1), weight(0), x(x_), y(y_), isCity(false)  {} // for source vertex
    bool operator<(const Plot& rhs) const
    {
      return weight < rhs.weight;
    }
    bool isLeftEdge(int width)
    {
      return (x == 0 && y > 0 && y < width - 1);
    }
    bool isRightEdge(int width)
    {   
      return (x == width-1 && y > 0 && y < width - 1);
    }
    bool isTopEdge(int width)
    {
      return (y == width-1 && x > 0 && x < width - 1);
    }
    bool isBottomEdge(int width)
    {
      return (y == 0 && x > 0 && x < width - 1);
    }
    bool isUpperLeftCorner(int width)
    {
      return (x == 0 && y == width - 1);
    }
    bool isUpperRightCorner(int width)
    {
      return (x == width - 1 && y == width - 1);
    }
    bool isLowerLeftCorner(int width)
    {
      return (x == 0 && y == 0);
    }
    bool isLowerRightCorner(int width)
    {
      return (x == width - 1 && y == 0);
    }
    Plot getLeftVertex()
    {
        return Plot(x - 1, y);
    }
    Plot getRightVertex()
    {
        return Plot(x + 1, y);    
    }
    Plot getTopVertex()
    {
        return Plot(x, y + 1);
    }
    Plot getBottomVertex()
    {
        return Plot(x, y - 1);
    }
    Plot getTopLeftVertex()
    {
        return Plot(x - 1, y + 1);
    }
    Plot getTopRightVertex()
    {
        return Plot(x + 1, y + 1);    
    }
    Plot getBottomLeftVertex()
    {
        return Plot(x - 1, y - 1);
    }
    Plot getBottomRightVertex()
    {
        return Plot(x + 1, y - 1);
    }
    Plot getAdjacentPlot(int i, int width_);

    PreviousVertex previousVertex; 
    int weight;
    int x;
    int y;
    bool isCity;
};
// class AdjListNode
// {
  // public:
    // AdjListNode();
    // bool operator<(const AdjListNode& rhs)
    // {
      // return (plot.x < rhs.plot.x) || ((plot.x == rhs.plot.x) && (plot.y < rhs.plot.y));
    // }
    // Plot plot;
    // Plot adjacentPlots[4];
// };

class Router
{
public:
  int width_;
  BinaryHeap<Plot> heap;
  // Map1000 map_; // has a double array of shorts called map1000 
  short** map_;
  Router(const Map1000 *map, int width);
  ~Router();
  void printMap();
  void findRoutes(const CityPos *cityPos, int cityCount, Edge *paths, int &pathCount);
};


#endif
