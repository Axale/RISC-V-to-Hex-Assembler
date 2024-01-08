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
      cout << "Test '" << test_name << "' Failed.\n";
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

    // cout << received_input.size();
    // for(int i = 0; i < received_input.size(); i++) {
    //   if(received_input[i] == expected_file_input[i]) cout << "true";
    //   if(received_input[i] != expected_file_input[i]) cout << "false";
    //   cout << received_input[i];
    // }

    delete input_object;

  }

  void test_read_opcode(Translator* translator, string& input, uint32_t expected_instr, uint32_t expected_op, bool expected_success) {
    translator->line = input;
    translator->strit = translator->line.begin();
    uint32_t op;


    comp_func(translator->read_opcode(op), expected_success);
    comp_func(op, expected_op);
    comp_func(translator->instruction, expected_instr);

  }
};


int main(void){
  Test test;

  test.test_name = "FileIn - Basic File Read";
  vector<string> expected_output = {"Hello, World!"};
  test.test_filein(expected_output, true, "example/helloworld.asm");
  
  test.test_name = "FileIn - Long File Read";
  expected_output.clear();
  expected_output = {"Hello, World!\r", "Happy New Year!"};
  test.test_filein(expected_output, true, "example/longer.asm");
  return 0;

  enum test_op_type {R, I, S, B, U, J, ERR, ADDR, PSEU, MEM};
  Translator* translator = new Translator();
  test.test_name = "Translate - Basic";
  string input_line = "add";
  test.test_read_opcode(translator, input_line, 0x00000033, true, R);


}