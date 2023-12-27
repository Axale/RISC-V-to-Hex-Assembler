#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP
#include <stdint.h>
#include <string>
#include <vector>
#include <iterator>
#include <memory>


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
    class Instr_Op {
      public:
      std::string instrString;
      uint32_t instrNum;
      uint32_t instrType;
      Instr_Op(std::string instructionString, uint32_t instructionNum, uint32_t instr_type);
      ~Instr_Op();
    };

    class InstrVector {
      private: 
      uint32_t mode;
      std::vector<Instr_Op*> instrIndex;
      std::vector<Instr_Op*>::iterator indexIt;
      
      public:
      InstrVector(uint32_t mode);
      ~InstrVector();
      uint32_t parse_instruction(std::string instrString, uint32_t& op);
    };
    
    InstrVector* opVector;
    InstrVector* argVector;

    bool read_opcode(uint32_t& op);
    bool read_arg(uint32_t& op, uint32_t shft);
    bool translate_immediate(uint32_t& op);
    bool read_line(std::vector<TranslatedInstruction>& instructionVector);
    enum op_type {R, I, S, B, U, J, ERR, ARG, PSEU, MEM};

    void invalid_syntax();
  public:
    bool translate(std::vector<TranslatedInstruction>& instructionVector, std::vector<std::string> stringVector);
    Translator();
    ~Translator();
};
#endif
