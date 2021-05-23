// Author: Sean Davis

#ifndef ROUTER_H
  #define ROUTER_H

#include "RouterRunner.h"

class Router
{
public:
  short map_[1000][1000];
  Router(const Map1000 *map, int width);
  ~Router();
  void printMap();
  void findRoutes(const CityPos *cityPos, int cityCount, Edge *paths, int &pathCount);
};


#endif
