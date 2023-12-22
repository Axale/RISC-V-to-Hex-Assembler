#include <fstream>
#include <string>
#include <vector>

using namespace std;
class fileio
{
private:
  string inputPath;
  ifstream inputFile;
  ofstream outputFile;
  string outputPath;
public:
  fileio(char* inputString);
  ~fileio();
  bool open_input(vector<string>& inputBuf);
};

fileio::fileio(char* inputString)
{
  inputPath = inputString;
  outputPath = "meminit.hex";
}

fileio::~fileio()
{
    inputFile.close();
}

bool fileio::open_input(vector<string>& inputVector) {
  inputFile.open(inputPath, ios::in);

  // Checks if the file successfully opened, then copies it line by line into a vector of strings
  if(inputFile.is_open()) {
    for(string line; getline(inputFile, line);) {
      inputVector.push_back(line);
    }

    return true;
  }

  return false;
}