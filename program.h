#ifndef JNP1_6_PROGRAM_H_
#define JNP1_6_PROGRAM_H_

#include "computer_parts.h"
#include "ooasm_instructions.h"

namespace jnp1_6 {
class Program {
  private:
    std::vector<InstructionPtr> vec;
  public:
    Program(std::vector<InstructionPtr> &&init) :
        vec(std::move(init)) {};

    void run(jnp1_6::Processor &processor, jnp1_6::Memory &memory) {
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

jnp1_6::Program program(const std::initializer_list<jnp1_6::InstructionPtr> &&init);
#endif //JNP1_6_PROGRAM_H_
