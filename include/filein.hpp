#ifndef FILEIO_HPP
#define FILEIO_HPP
#include <fstream>
#include <string>
#include <vector>
class filein
{
private:
  std::string inputPath;
  std::ifstream inputFile;
public:
  filein(std::string inputString);
  ~filein();
  bool open_input(std::vector<std::string>& inputBuf);
};
#endif