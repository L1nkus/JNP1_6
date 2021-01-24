#ifndef JNP1_6_COMPUTER_H
#define JNP1_6_COMPUTER_H

#include "computer_parts.h"
#include "ooasm_instructions.h"
#include <memory>

// [TODO] przesunac gdzies?
namespace jnp1_6 {
class Program {
  private:
    std::vector<std::shared_ptr<jnp1_6::Instruction>> vec;
  public:
    Program(std::vector<std::shared_ptr<jnp1_6::Instruction>> &&init) :
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

inline jnp1_6::Program program(const std::initializer_list<jnp1_6::Instruction *> &init) {
    std::vector<std::shared_ptr<jnp1_6::Instruction>> vec;
    vec.reserve(init.size());
    for (auto &i: init) {
        vec.push_back(std::shared_ptr<jnp1_6::Instruction>(i));
    }
    return jnp1_6::Program(std::move(vec));
}

class Computer {
  private:
    jnp1_6::Processor processor;
    jnp1_6::Memory memory;
  public:
    Computer(jnp1_6::unsigned_word_t mem_size) : memory(mem_size) {}

    void boot(jnp1_6::Program &p) {
        p.run(processor, memory);
    }

    void memory_dump(std::ostream &os) const {
        memory.dump(os);
    }
};

#endif // JNP1_6_COMPUTER_H
