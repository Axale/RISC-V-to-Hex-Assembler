#include <iostream>
#include <string>
#include <vector>
#define private public
#include "../include/filein.hpp"
#include "../include/translator.hpp"
using namespace std;

class Test {
  public:
  string test_name;
  template <typename T>
  void comp_func(T output, T expected_output) {
    if(output == expected_output) {
      return;
    } else {
      cout << "\nTest '" << test_name << "' Failed on test number #";
    }
  }

  void test_filein(vector<string>& expected_file_input, bool expected_success, string inputPath) {
    filein *input_object = new filein(inputPath);
    vector<string> received_input;
    bool success = input_object->open_input(received_input);
    comp_func(success, expected_success);
    if(expected_success) {
      comp_func(received_input, expected_file_input);
    }

    delete input_object;

  }
};


int main(void){
  Test test;

  test.test_name = "Basic File Read";
  vector<string> expected_output = {"Hello, World!"};
  test.test_filein(expected_output, true, "example/helloworld.txt");
  return 0;
}