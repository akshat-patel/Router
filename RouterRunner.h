#ifndef ROUTERRUNNER_H
  #define ROUTERRUNNER_H

typedef struct{
  int x;
  int y;
} CityPos;

typedef struct {  // start and end should be adjacent!
  short startX;
  short startY;
  short endX;
  short endY;
} Edge;

typedef struct {
  short map1000[1000][1000];
} Map1000;

const int NORTH = 1;
const int NORTHEAST = 2;
const int EAST = 4;
const int SOUTHEAST = 8;
const int SOUTH = 16;
const int SOUTHWEST = 32;
const int WEST = 64;
const int NORTHWEST = 128;
#endif
