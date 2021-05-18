// Author: Sean Davis
#include <iostream>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<algorithm>
#include <iomanip>

using namespace std;

void createMountainRanges(short **map, int width)
{
  int numMountainRanges, startX, startY, endX, endY, maxElevation,
    maxElevationDelta, x, y, deltaY, deltaX, temp;
  double slope;
  short elevation;

  numMountainRanges = int (1 + 0.4 * width);

  for(int i = 0; i < numMountainRanges; i++)
  {
    maxElevation = rand() % 100;
    maxElevationDelta = maxElevation / 5;
    if(maxElevationDelta < 2)
      maxElevationDelta = 2;  // allows for some change in low ranges.

    startX = rand() % width + 1;
    startY = rand() % width + 1;
    endX = rand() % width + 1;
    endY = rand() % width + 1;
    if(startY > endY)
    {
      temp = startY;
      startY = endY;
      endY = temp;
    }
  /*
    // average half way across map
    deltaY = abs(startY - endY);
    if(deltaY < 5)
      deltaY = 5;

    // endX = startX + rand() % (deltaY / 5) - deltaY / 10;

    if(endX > width)
      endX = width;

    if(endX < 1)
      endX = 1;
  */
    if(startX != endX)
      slope = (startY - endY) / double(startX - endX);
    else
      slope = 10000;  // same as infinity when y cannot be larger than 100.

    for(int y = startY; y <= endY; y++)
    {
      x = int((y - startY) / slope + startX);

      if(x < 0)
        x = 0;
      if(x >= width)
        x = width - 1;

      elevation = max(1, maxElevation - maxElevationDelta + rand() % maxElevationDelta);
      if(elevation > map[x][y])
        map[x][y] = elevation;
    } // for each y
  }  // for each mountain range
}  // createMountainRanges()

void fillMap(short** map, int width)
{
  int endX, endY, slopeX, slopeY, startX, x;
  for(int i = 0; i < width + 2; i++)
  {  // create unseen border of 1's.
    map[i][0] = 1;
    map[0][i] = 1;
    map[width + 1][i] = 1;
    map[i][width + 1] = 1;
  } // for i


  for(int y = 1; y <= width; y++) // Set low points in valleys.
  {
    endX = startX = 0;
    while(endX < width + 1)
    {
      for(endX = startX + 1; map[endX][y] == 0; endX++);  // find none zero
      x = (startX + endX) / 2;
      if(map[x][y] == 0)
        map[x][y] = (map[startX][y] + map[endX][y]) / 2 - 50;
      if(map[x][y] < 1)
        map[x][y] = rand() % 5 + 1;
      if(map[x][y] > 99)
        map[x][y] = 99;
      startX = endX;
    } // while have reached end of line
  } // for y

  for(int x = 1; x <= width; x++)
    for(int y = 1; y <= width; y++)
    {
      if(map[x][y] == 0)
      {
        for(endX = x + 1; map[endX][y] == 0; endX++);  // find none zero
        for(endY = y + 1; map[x][endY] == 0; endY++);  // find none zero
        slopeX = (map[endX][y] - map[x - 1][y]) / (endX - x + 1) ;
        slopeY = (map[x][endY] - map[x][y - 1]) / (endY - y + 1);
        map[x][y] = (map[x - 1][y] + slopeX + map[x][y - 1] + slopeY) / 2 + rand() % 5;
        if(map[x][y] > 99)
          map[x][y] = 99;
      }
    } // for y
}  // fillMap()



short** createMap(int width)
{
  short **map = new short* [width + 2];


  for(int i = 0; i < width + 2; i++)
  {
    map[i] = new short[width + 2];

    for(int j = 0; j < width + 2; j++)
      map[i][j] = 0;  // unassigned an elevation
  } // for each width

  createMountainRanges(map, width);
  fillMap(map, width);


  return map;
} // createMap()

void printMap(short **map, int width)
{
  for(int i = 1; i < 26; i++)
  {
    for(int j = 1; j < 26; j++)
      cout << setw(3) << map[j][i];
    cout << endl;
  } // for i
}  // printMap()

void writeCities(ofstream &outf, int cities, int width)
{
  for(int i = 0; i < cities; i++)
    outf << rand() % width << ' ' << rand() % width << endl;
} // writeCities()

int main()
{
  int seed;
  int width, cities;
  char filename[80], process;
  short **map;

  cout << "Number of cities > ";
  cin >> cities;
  cout << "Width > ";
  cin >> width;
  cout << "Seed> ";
  cin >> seed;
  srand(seed);
  sprintf(filename, "map-%d-%d-%d.txt", cities, width, seed);
  ofstream outf(filename);
  outf << cities << " " << width << endl;
  writeCities(outf, cities, width);
  map = createMap(width);
 // printMap(map, width);

  for(int y = 1; y <= width; y++)
  {
    for(int x = 1; x <= width; x++)
      outf << setw(3) << map[x][y];
    outf << endl;
  } // for i

  outf.close();
  for(int i = 0; i < width + 2; i++)
    delete [] map[i];

  delete [] map;
  return 0;
}

