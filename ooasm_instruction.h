#ifndef JNP1_6_OOASM_INSTRUCTION_H_
#define JNP1_6_OOASM_INSTRUCTION_H_

#include "computer_parts.h"

namespace jnp1_6 {
class Instruction {
  public:
    virtual void load(Memory &) = 0;

    virtual void execute(Processor &, Memory &) = 0;

    virtual ~Instruction() = default;
};
}
#endif //JNP1_6_OOASM_INSTRUCTION_H_
