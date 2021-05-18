// Author: Sean Davis

#ifndef ROUTER_H
  #define ROUTER_H

#include "RouterRunner.h"

class Router
{
public:
  Router(const Map1000 *map, int width);
  ~Router();
  void findRoutes(const CityPos *cityPos, int cityCount, Edge *paths, int &pathCount);
};


#endif
