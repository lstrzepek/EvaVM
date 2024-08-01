#ifndef EvaVM_h
#define EvaVM_h

#include <array>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "../Logger.h"
#include "../bytecode/OpCode.h"
#include "./EvaValue.h"

#define STACK_LIMIT 512
#define READ_BYTE() *ip++
#define GET_CONST() constants[READ_BYTE()]
#define BIN_OP(op)                                                             \
  do {                                                                         \
    auto op2 = AS_NUMBER(pop());                                               \
    auto op1 = AS_NUMBER(pop());                                               \
    auto result = op1 op op2;                                                  \
    push(NUMBER(result));                                                      \
  } while (false)

class EvaVM {
public:
  EvaVM() {}

  void push(const EvaValue &value) {
    if ((size_t)(sp - stack.begin()) == STACK_LIMIT) {
      DIE << "push(): Stack overflow.\n";
    }
    *sp = value;
    sp++;
  }

  EvaValue pop() {
    if (sp <= stack.begin()) {
      DIE << "pop(): Stack underflow.\n";
    }
    --sp;
    return *sp;
  }

  EvaValue exec(const std::string &program) {

    constants.push_back(NUMBER(42));
    constants.push_back(NUMBER(27));
    code = {OP_CONST, 0, OP_CONST, 1, OP_SUB, OP_HALT};

    ip = &code[0];
    sp = &stack[0];

    return eval();
  }

  EvaValue eval() {
    for (;;) {
      auto opcode = READ_BYTE();
      switch (opcode) {
      case OP_CONST:
        push(GET_CONST());
        break;
      case OP_ADD: {
        BIN_OP(+);
        break;
      }
      case OP_SUB: {
        BIN_OP(-);
        break;
      }
      case OP_MUL: {
        BIN_OP(*);
        break;
      }
      case OP_DIV: {
        BIN_OP(/);
        break;
      }
      case OP_HALT:
        return pop();
      default:
        DIE << "Unknown opcode: " << std::showbase << std::hex << opcode;
        break;
      }
    }
  }
  uint *ip;
  EvaValue *sp;
  std::array<EvaValue, STACK_LIMIT> stack;
  std::vector<uint> code;
  std::vector<EvaValue> constants;
};

#endif
