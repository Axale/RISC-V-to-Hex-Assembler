#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP
#include <stdint.h>
#include <string>
#include <vector>


class TranslatedInstruction 
{
  private:
    uint32_t instruction;
    uint32_t address;
  public:
  TranslatedInstruction(uint32_t instruction, uint32_t opcode);
  uint32_t get_instruction();
  uint32_t get_address();
  void set_instruction(uint32_t instruction);
  void set_address(uint32_t address);
};

class Translator
{
  private:
    uint32_t instruction;
    std::string::iterator strit;
    std::string line;
    
    bool read_opcode(uint32_t &op);
    int32_t op_recurse();
    bool read_arg(uint32_t op);
    bool read_line(std::vector<TranslatedInstruction>& instructionVector);
    enum op_type {R, I, S, B, U, J, ERR};
    void invalid_syntax();
  public:
    bool translate(std::vector<TranslatedInstruction>& instructionVector, std::vector<std::string> stringVector);
    Translator();
    ~Translator();
};
#endif
