// Author Sean Davis
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "RouterRunner.h"
#include "CPUTimer.h"
#include "router.h"

using namespace std;

void readFile(const char *filename, Map1000 *map, Map1000 *map2, int *width,
  CityPos **cityPos, int *cityCount)
{
  ifstream inf(filename);
  inf >> *cityCount >> *width;
  *cityPos = new CityPos[*cityCount];

  for(int i = 0; i < *cityCount; i++)
    inf >> (*cityPos)[i].x >> (*cityPos)[i].y;

  for(int y = *width - 1; y >= 0; y--)
    for(int x = 0; x < *width; x++)
    {
      inf >> map->map1000[x][y];
      map2->map1000[x][y] = map->map1000[x][y];
    }
}  // readFile()

void printEdge(const Edge &edge)
{
  cout << "startX: " << edge.startX << "\nstartY: " << edge.startY;
  cout << "\nendX: " << edge.endX << "\nendY: " << edge.endY << endl;
} // printEdge()


char** createAdjMap(Edge *paths, int pathCount, int width)
{
  char **map3 = new char*[width];
  for(int i = 0; i < width; i++)
  {
    map3[i] = new char[width];
    for(int j = 0; j < width; j++)
      map3[i][j] = '\0';
  }

  for(int i = 0; i < pathCount; i++)
  {
    Edge edge = paths[i];
    
    if(edge.startX < 0 || edge.endX < 0 || edge.startY < 0
      || edge.endY < 0 || edge.startX >= width || edge.endX >= width
      || edge.startY >= width || edge.endY >= width)
    {
      cout << "Edge " << i << " has coordinate(s) outside the acceptable range:\n";
      printEdge(edge);
      exit(1);
    }

    if(abs(edge.startX - edge.endX) > 1
      || abs(edge.startY - edge.endY) > 1)
    {
      cout << "Edge " << i <<  " tries to connect two vertices that are not adjacent.\n";
      printEdge(edge);
      exit(1);
    }

    if (edge.startX == edge.endX) // NORTH or SOUTH
    {
      if (edge.startY == edge.endY)
      {
        cout << "Edge " << i << " connects a vertice to itself!";
        printEdge(edge);
      }
      else
        if(edge.startY < edge.endY)
        {
          map3[edge.startX][edge.startY] |= NORTH;
          map3[edge.endX][edge.endY] |= SOUTH;
        }
        else // edge.startY > edge.endY
        {
          map3[edge.startX][edge.startY] |= SOUTH;
          map3[edge.endX][edge.endY] |= NORTH;
        }
    } // if edge.startX == edge.endX
    else
      if(edge.startX > edge.endX) // heading West
      {
        if (edge.startY == edge.endY)
        {
          map3[edge.startX][edge.startY] |= WEST;
          map3[edge.endX][edge.endY] |= EAST;
        }
        else
          if(edge.startY < edge.endY)
          {
            map3[edge.startX][edge.startY] |= NORTHWEST;
            map3[edge.endX][edge.endY] |= SOUTHEAST;
          }
          else // edge.startY > edge.endY
          {
            map3[edge.startX][edge.startY] |= SOUTHWEST;
            map3[edge.endX][edge.endY] |= NORTHEAST;
          }
      } // if edge.startX > edge.endX so heading West
      else // edge.startX < edge.endX so heading East
      {
        if (edge.startY == edge.endY)
        {
          map3[edge.startX][edge.startY] |= EAST;
          map3[edge.endX][edge.endY] |= WEST;
        }
        else
          if(edge.startY < edge.endY)
          {
            map3[edge.startX][edge.startY] |= NORTHEAST;
            map3[edge.endX][edge.endY] |= SOUTHWEST;
          }
          else // edge.startY > edge.endY
          {
            map3[edge.startX][edge.startY] |= SOUTHEAST;
            map3[edge.endX][edge.endY] |= NORTHWEST;
          }
      } // else edge.startX < edge.endX so heading East
  } // for each edge

  return map3;
}  // createAdjMap()


void dfs(int x, int y, char **map3, Map1000 *map2, bool **visited, int &total)
{
  if(visited[x][y]) // should never be true
    return;

  int elevationChange;

  visited[x][y] = true;
  if((map3[x][y] & NORTH) && !visited[x][y + 1])
  {
    elevationChange = abs(map2->map1000[x][y] - map2->map1000[x][y + 1]);
    total += elevationChange * elevationChange + 10;
    dfs(x, y + 1, map3, map2, visited, total);
  } // if NORTH unvisited

  if((map3[x][y] & NORTHWEST) && !visited[x - 1][y + 1])
  {
    elevationChange = abs(map2->map1000[x][y] - map2->map1000[x - 1][y + 1]);
    total += elevationChange * elevationChange + 10;
    dfs(x - 1, y + 1, map3, map2, visited, total);
  } // if Northwest unvisited

  if((map3[x][y] & WEST) && !visited[x - 1][y])
  {
    elevationChange = abs(map2->map1000[x][y] - map2->map1000[x - 1][y]);
    total += elevationChange * elevationChange + 10;
    dfs(x - 1, y, map3, map2, visited, total);
  } // if West unvisited

  if((map3[x][y] & SOUTHWEST) && !visited[x - 1][y - 1])
  {
    elevationChange = abs(map2->map1000[x][y] - map2->map1000[x - 1][y - 1]);
    total += elevationChange * elevationChange + 10;
    dfs(x - 1, y - 1, map3, map2, visited, total);
  } // if Southwest unvisited

  if((map3[x][y] & SOUTH) && !visited[x][y - 1])
  {
    elevationChange = abs(map2->map1000[x][y] - map2->map1000[x][y - 1]);
    total += elevationChange * elevationChange + 10;
    dfs(x, y - 1, map3, map2, visited, total);
  } // if South unvisited

  if((map3[x][y] & SOUTHEAST) && !visited[x + 1][y - 1])
  {
    elevationChange = abs(map2->map1000[x][y] - map2->map1000[x + 1][y - 1]);
    total += elevationChange * elevationChange + 10;
    dfs(x + 1, y - 1, map3, map2, visited, total);
  } // if Southeast unvisited

  if((map3[x][y] & EAST) && !visited[x + 1][y])
  {
    elevationChange = abs(map2->map1000[x][y] - map2->map1000[x + 1][y]);
    total += elevationChange * elevationChange + 10;
    dfs(x + 1, y, map3, map2, visited, total);
  } // if East unvisited

  if((map3[x][y] & NORTHEAST) && !visited[x + 1][y + 1])
  {
    elevationChange = abs(map2->map1000[x][y] - map2->map1000[x + 1][y + 1]);
    total += elevationChange * elevationChange + 10;
    dfs(x + 1, y + 1, map3, map2, visited, total);
  } // if Northeast unvisited
} // dfs()


void checkRoute(Map1000 *map2, const CityPos *cityPos, int cityCount, Edge *paths,
  int pathCount, int width)
{
  char **map3 = createAdjMap(paths, pathCount, width);
  bool **visited = new bool*[width];
  int total = 0;
  for(int i = 0; i < width; i++)
  {
    visited[i] = new bool[width];
    for(int j = 0; j < width; j++)
      visited[i][j] = false;
  } // for i

  if(map3[cityPos[0].x][cityPos[0].y])
  {
    dfs(cityPos[0].x, cityPos[0].y, map3, map2, visited, total);
    cout << " Total cost: " << total << endl;

    for(int i = 0; i < cityCount; i++)
      if(!visited[cityPos[i].x][cityPos[i].y])
        cout << "City " << i << " was not connected.\n";
  } // if city 0 connected
  else
    cout << "City 0 was not connected.\n";

  for(int i = 0; i < width; i++)
  {
    delete [] visited[i];
    delete [] map3[i];
  } // for i

  delete [] visited;
  delete [] map3;
}  // checkRoute()

int main(int argc, char* argv[])
{
  Map1000 *map, *map2;
  int width, cityCount, pathCount;
  CityPos *cityPos;
  Router *router;
  Edge *paths;
  CPUTimer ct;
  map = new Map1000;
  map2 = new Map1000;
  readFile(argv[1], map, map2, &width, &cityPos, &cityCount);
  paths = new Edge[4 * width * width];  // maximum number of edges possible
  ct.reset();
  router = new Router((const Map1000*) map, width);
  delete map;
  router->findRoutes((const CityPos*) cityPos, cityCount, paths, pathCount);
  cout << "CPU time: " << ct.cur_CPUTime() ;
  checkRoute(map2, cityPos, cityCount, paths, pathCount, width);
  delete map2;
  delete [] cityPos;
  delete [] paths;
  delete router;
  return 0;
}

