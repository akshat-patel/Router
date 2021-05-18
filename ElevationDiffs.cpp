#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[])
{
  char filename[80];

  cout << "Filename: ";
  cin >> filename;
  int x, y, cityCount, width, counts[100];

  ifstream inf(filename);
  inf >> cityCount >> width;
  short **map = new short* [width];
  for(int i = 0; i < width; i++)
    map[i] = new short[width];

  for(int i = 0; i < cityCount; i++)
    inf >> x >> y;

  for(int y = 0; y < width; y++)
    for(int x = 0; x < width; x++)
      inf >> map[x][y];

  for(int i = 0; i < 100; i++)
    counts[i] = 0;

  for(int y = 0; y < width - 1; y++)
    for(int x = 0; x < width - 1; x++)
    {
      counts[abs(map[x][y] - map[x + 1][y])]++;
      counts[abs(map[x][y] - map[x][y + 1])]++;
    }

  for(int i = 0; i < 100; i++)
    cout << i << " " << counts[i] << endl;



  return 0;
}
 
