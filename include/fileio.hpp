#ifndef FILEIO_HPP
#define FILEIO_HPP
#include <fstream>
#include <string>
#include <vector>
class fileio
{
private:
  std::string inputPath;
  std::ifstream inputFile;
  std::ofstream outputFile;
  std::string outputPath;
public:
  fileio(char* inputString);
  ~fileio();
  bool open_input(std::vector<std::string>& inputBuf);
};
#endif