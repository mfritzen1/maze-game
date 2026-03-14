#include <fstream>
#include <iostream>
#include <string>

int main()
{
  struct coord // coordinates of a given point in the maze, [0,0] is in the top left corner
  {
    int x{}; // x-coordinate
    int y{}; // y-coordinate
  };

  int height{0};          // number of rows in maze grid
  int width{};            // number of columns in maze grid
  int startX{};           // x-coordinate of the starting position, used for intermediate step
  int endX{};             // x-coordinate of the ending position, used for intermediate step
  coord startPos{};       // coordinates of the starting position
  coord endPos{};         // coordinates of the ending position
  std::string strInput{}; // a given line of the maze file

  // get maze file
  std::ifstream mazeFile{"mazes/maze-1.txt"};

  if (!mazeFile)
  {
    std::cerr << "maze file failed to open\n";
    return 1;
  }

  // read the maze file, find coordinates of starting and ending positions
  while (std::getline(mazeFile, strInput))
  {
    std::cout << strInput << '\n';
    startX = strInput.find('S');
    endX = strInput.find('E');
    if (startX < strInput.length())
    {
      startPos.x = startX;
      startPos.y = height;
    }
    if (endX < strInput.length())
    {
      endPos.x = endX;
      endPos.y = height;
    }
    height++;
  }
  width = strInput.length();

  // output
  std::cout << "height: " << height << '\n';
  std::cout << "width: " << width << '\n';
  std::cout << "S position: [" << startPos.x << ", " << startPos.y << "]" << '\n';
  std::cout << "E position: [" << endPos.x << ", " << endPos.y << "]" << '\n';

  return 0;
}