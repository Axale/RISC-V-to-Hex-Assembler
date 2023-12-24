#include "../include/translator.hpp"
#include <vector>
#include <string>
#include <cctype>

using namespace std;
TranslatedInstruction::TranslatedInstruction(uint32_t instruction, uint32_t address){
  this->instruction = instruction;
  this->address = address;
  return;
};

uint32_t TranslatedInstruction::get_instruction() { return(this->instruction);};
void TranslatedInstruction::set_instruction(uint32_t instruction) {this->instruction = instruction;};

uint32_t TranslatedInstruction::get_address() { return(this->address);};
void TranslatedInstruction::set_address(uint32_t address) {this->address = address;};

Translator::Translator() {
  instruction = 0;
  line = "";
  strit = line.begin();
}
bool Translator::read_opcode(uint32_t& op){
  string operation = "";
  string::iterator start = strit;
  for(; isalpha(*strit); strit++) {
    operation.append(1, *strit);
  }
  if(operation.empty())
  {
    return false;
  };

}