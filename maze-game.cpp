#include <fstream>
#include <iostream>
#include <string>

int main()
{
  std::ifstream inf{"mazes/maze-1.txt"};

  if (!inf)
  {
    std::cerr << "maze file failed to open\n";
    return 1;
  }

  std::string strInput{};
  while (std::getline(inf, strInput))
  {
    std::cout << strInput << '\n';
  }

  return 0;
}