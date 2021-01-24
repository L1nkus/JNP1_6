#ifndef JNP1_6_PROGRAM_H
#define JNP1_6_PROGRAM_H

#include "computer_parts.h"
#include "ooasm_instructions.h"
#include <initializer_list>

namespace jnp1_6 {
class Program {
  private:
    const std::vector<InstructionPtr> vec;
  public:
    Program(const std::vector<InstructionPtr> &&init) :
        vec(std::move(init)) {};

    void run(jnp1_6::Processor &processor, jnp1_6::Memory &memory) const {
        memory.reset_memory();
        for (auto &i: vec) {
            i->load(memory);
        }
        memory.finish_loading_variables();

        for (auto &i: vec) {
            i->execute(processor, memory);
        }
    }
};
}

jnp1_6::Program program(std::initializer_list<jnp1_6::InstructionPtr> init);
#endif // JNP1_6_PROGRAM_H
