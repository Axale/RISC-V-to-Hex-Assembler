#include "../include/translator.hpp"
#include <vector>
#include <string>
#include <cctype>
#include <cstring>

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
  opVector = new InstrVector(1);
  argVector = new InstrVector(0);
}

Translator::~Translator() {
  delete opVector;
  delete argVector;
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
  //lowercase the whole thing

  

}

Translator::Instr_Op::Instr_Op(string instructionString, uint32_t insturctionNum, uint32_t instructionType) {
  instrString = instructionString;
  instrNum = insturctionNum;
  instrType = instructionType;
}

Translator::InstrVector::InstrVector(uint32_t mode) {
  if(mode) {
    instrIndex = {
      // R-Types
      new Instr_Op("add", 0x00000033, R),
      new Instr_Op("sub", 0x00000033 + 0x20 << 25, R),
      new Instr_Op("xor", 0x00000033 + 0x4 << 12, R),
      new Instr_Op("or", 0x00000033 + 0x6 << 12, R),
      new Instr_Op("and", 0x00000033 + 0x7 << 12, R),
      new Instr_Op("sll", 0x00000033 + 0x1 << 12, R),
      new Instr_Op("srl", 0x00000033 + 0x5 << 12, R),
      new Instr_Op("sra", 0x00000033 + 0x5 << 12 + 0x20 << 25, R),
      new Instr_Op("slt", 0x00000033 + 0x2 << 12, R),
      new Instr_Op("sltu", 0x00000033 + 0x3 << 12, R),

      //I-Types
      new Instr_Op("addi", 0x00000013, I),
      new Instr_Op("xori", 0x00000013 + 0x4 << 12, I),
      new Instr_Op("ori", 0x00000013 + 0x6 << 12, I),
      new Instr_Op("andi", 0x00000013 + 0x7 << 12, I),
      new Instr_Op("slli", 0x00000013 + 0x1 << 12, I),
      new Instr_Op("srli", 0x00000013 + 0x5 << 12, I),
      new Instr_Op("srai", 0x00000013 + 0x5 << 12, I),
      //Loads
      new Instr_Op("lb", 0x00000003, I),
      new Instr_Op("lh", 0x00000003 + 0x1 << 12, I),
      new Instr_Op("lw", 0x00000003 + 0x2 << 12, I),
      new Instr_Op("lbu", 0x00000003 + 0x4 << 12, I),
      new Instr_Op("lwu", 0x00000003 + 0x5 << 12, I),
      //Other I-Types
      new Instr_Op("jalr", 0x00000067 + 0x0 << 12, I),
      new Instr_Op("ecall", 0x00000073 + 0x0 << 12, I),
      new Instr_Op("ebreak", 0x00000073 + 0x1 << 20, I),
      //S-Types
      new Instr_Op("sb", 0x00000023, S),
      new Instr_Op("sh", 0x00000023 + 0x1<< 12, S),
      new Instr_Op("sw", 0x00000023 + 0x2<< 12, S),
      //B-Types
      new Instr_Op("beq", 0x00000063 + 0x0<< 12, B),
      new Instr_Op("bne", 0x00000063 + 0x1<< 12, B),
      new Instr_Op("blt", 0x00000063 + 0x4<< 12, B),
      new Instr_Op("bge", 0x00000063 + 0x5<< 12, B),
      new Instr_Op("bltu", 0x00000063 + 0x6<< 12, B),
      new Instr_Op("bgeu", 0x00000063 + 0x7<< 12, B),
      //J-Type
      new Instr_Op("jal", 0x0000006F + 0x0<< 12, J), // All Alone :(
      //U-Type
      new Instr_Op("lui", 0x00000037, U),
      new Instr_Op("auipc", 0x00000017, U)

    };
  } else {

  }
  indexIt = begin(instrIndex);
}