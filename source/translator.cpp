#include "../include/translator.hpp"
#include <vector>
#include <string>
#include <cctype>
#include <cstring>
#include <charconv>

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

bool Translator::translate_immediate(uint32_t& op) {
  enum num_base {DEC, HEX, BIN};
  uint32_t base = DEC;
  uint32_t imm = 0;
  if(!isalnum(*strit)) return false;

  for(;*strit == '0'; strit++) {
    if(*(strit + 1) == 'x' || *(strit + 1) == 'h') {
      if(base == HEX || base == BIN) {
        return false;
      }
      base = HEX;
      strit++;
    } else if((*strit + 1) == 'b') {
      if(base == HEX || base == BIN) {
        return false;
      }
      base = HEX;
      strit++;
    }
  }
  char* start = &(*strit);
  
  for(;isalnum(*strit); strit++);
  char* end = &(*(strit - 1));

  switch (base)
  {
  case HEX:
    from_chars(start, end, (int&)imm, 16);
    break;
  
  case BIN:
    from_chars(start, end, (int&)imm, 2);
    break;

  default:
    from_chars(start, end, (int&)imm);
    break;
  }

  switch (op) {
    S:
      instruction |= ((imm << 27) >> 20) | ((imm >> 5) << 25);
    break;
    B:
      instruction |=  ((0x800 & imm) >> 4)| (((imm >> 1) << 28) >> 20) | ((imm >> 5) << 25) | ((imm & 0x1000) << 19);
    break;
    U:
      instruction |= imm & 0xFFFFF000;
    break;
    J:
      instruction |= imm & 0x000FF000 | (0x800 & imm << 9) | (imm & 0x7FE << 20) | (0x0010000 & imm << 11);
    break;
    default:
      instruction |= imm << 20;
      break;
  }
  return true;
}

bool Translator::read_arg(uint32_t& op, uint32_t shft) {
  op = ERR;
  string operation = "";
  for(; isalnum(*strit); strit++) {
    operation.append(1, *strit);
  }

  if(operation.empty())
  {
    return false;
  };
  //lowercase the whole thing

  instruction |= (argVector->parse_instruction(operation, op) << shft);
  if(op == ERR) 
  {
    return false;
  }
  
  return true;
}
bool Translator::read_opcode(uint32_t& op){
  op = ERR;
  string operation = "";
  for(; isalpha(*strit); strit++) {
    operation.append(1, *strit);
  }
  if(operation.empty())
  {
    return false;
  };
  //lowercase the whole thing
  

  instruction = opVector->parse_instruction(operation, op);
  if(op == ERR)  
  {
    return false;
  }  

  return true;

}

Translator::Instr_Op::Instr_Op(string instructionString, uint32_t insturctionNum, uint32_t instructionType) {
  instrString = instructionString;
  instrNum = insturctionNum;
  instrType = instructionType;
}

Translator::Instr_Op::~Instr_Op(){
  return;
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
      new Instr_Op("sra", 0x00000033 + (0x5 << 12) + (0x20 << 25), R),
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
      new Instr_Op("x0", 0x00000000, ARG);
      new Instr_Op("zero", 0x00000000, ARG);
      new Instr_Op("x1", 0x00000001, ARG);
      new Instr_Op("ra", 0x00000001, ARG);
      new Instr_Op("x2", 0x00000002, ARG);
      new Instr_Op("sp", 0x00000002, ARG);
      new Instr_Op("x3", 0x00000003, ARG);
      new Instr_Op("gp", 0x00000003, ARG);
      new Instr_Op("x4", 0x00000004, ARG);
      new Instr_Op("rp", 0x00000004, ARG);
      new Instr_Op("x5", 0x00000005, ARG);
      new Instr_Op("t0", 0x00000005, ARG);
      new Instr_Op("x6", 0x00000006, ARG);
      new Instr_Op("t1", 0x00000006, ARG);
      new Instr_Op("x7", 0x00000007, ARG);
      new Instr_Op("t2", 0x00000007, ARG);
      new Instr_Op("x8", 0x00000008, ARG);
      new Instr_Op("s0", 0x00000008, ARG);
      new Instr_Op("fp", 0x00000008, ARG);
      new Instr_Op("x9", 0x00000009, ARG);
      new Instr_Op("s1", 0x00000009, ARG);
      new Instr_Op("x10", 0x0000000A, ARG);
      new Instr_Op("a0", 0x0000000A, ARG);
      new Instr_Op("x11", 0x0000000B, ARG);
      new Instr_Op("a1", 0x0000000B, ARG);
      new Instr_Op("x12", 0x0000000C, ARG);
      new Instr_Op("a2", 0x0000000C, ARG);
      new Instr_Op("x13", 0x0000000D, ARG);
      new Instr_Op("a3", 0x0000000D, ARG);
      new Instr_Op("x14", 0x0000000E, ARG);
      new Instr_Op("a4", 0x0000000E, ARG);
      new Instr_Op("x15", 0x0000000F, ARG);
      new Instr_Op("a5", 0x0000000F, ARG);
      new Instr_Op("x16", 0x00000010, ARG);
      new Instr_Op("a6", 0x00000010, ARG);
      new Instr_Op("x17", 0x00000011, ARG);
      new Instr_Op("a7", 0x00000011, ARG);
      new Instr_Op("x18", 0x00000012, ARG);
      new Instr_Op("s2", 0x00000012, ARG);
      new Instr_Op("x19", 0x00000013, ARG);
      new Instr_Op("s3", 0x00000013, ARG);
      new Instr_Op("x20", 0x00000014, ARG);
      new Instr_Op("s4", 0x00000014, ARG);
      new Instr_Op("x21", 0x00000015, ARG);
      new Instr_Op("s5", 0x00000015, ARG);
      new Instr_Op("x22", 0x00000016, ARG);
      new Instr_Op("s6", 0x00000016, ARG);
      new Instr_Op("x23", 0x00000017, ARG);
      new Instr_Op("s7", 0x00000017, ARG);
      new Instr_Op("x24", 0x00000018, ARG);
      new Instr_Op("s8", 0x00000018, ARG);
      new Instr_Op("x25", 0x00000019, ARG);
      new Instr_Op("s9", 0x00000019, ARG);
      new Instr_Op("x26", 0x0000001A, ARG);
      new Instr_Op("s10", 0x0000001A, ARG);
      new Instr_Op("x27", 0x0000001B, ARG);
      new Instr_Op("s11", 0x0000001B, ARG);
      new Instr_Op("x28", 0x0000001C, ARG);
      new Instr_Op("t3", 0x0000001C, ARG);
      new Instr_Op("x29", 0x0000001D, ARG);
      new Instr_Op("t4", 0x0000001D, ARG);
      new Instr_Op("x30", 0x0000001E, ARG);
      new Instr_Op("t5", 0x0000001E, ARG);
      new Instr_Op("x31", 0x0000001F, ARG);
      new Instr_Op("t6", 0x0000001F, ARG);
  }
  indexIt = begin(instrIndex);
}
Translator::InstrVector::~InstrVector() {
  uint32_t max_size = instrIndex.size();
  for(uint32_t i = 0; i < max_size; i++)
    delete instrIndex[i];  
}

uint32_t Translator::InstrVector::parse_instruction(string instrString, uint32_t& op) {
  op = ERR;
  uint32_t instruction = 0x0;
  for(indexIt = instrIndex.begin(); indexIt != instrIndex.end(); indexIt++) {
    if(instrString == (*indexIt)->instrString) {
      op = (*indexIt)->instrType;
      return ((*indexIt)->instrNum);
    }
  }

  return 0x0;
}