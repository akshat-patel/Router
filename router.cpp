// Author Sean Davis
#include "router.h"
#include <iostream>
#include <cstring>
using namespace std;

void Router::printMap()
{
    for(int i = 0; i < 1000; i++)
    {
        for(int j = 0; j < 1000; j++)
        {
            cout << map_[i][j] << " ";
        }
    }
}

Router::Router(const Map1000 *map, int width)
{
    memcpy(&(map_), &(map->map1000), sizeof(map_));   
    printMap(); 
} // Router()

Router::~Router()
{
}  // ~Router()


void Router::findRoutes(const CityPos *cityPos, int cityCount, Edge *paths, int &pathCount)
{
} // findRoutes()
