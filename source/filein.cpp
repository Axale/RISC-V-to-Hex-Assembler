#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <iostream>
#include "../include/filein.hpp"

using namespace std;

filein::filein(string inputString)
{
  this->inputPath = inputString;
}

filein::~filein()
{
    inputFile.close();
}

bool filein::open_input(vector<string>& inputVector) {
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