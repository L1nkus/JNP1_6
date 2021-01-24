#ifndef JNP1_6_COMPUTER_H
#define JNP1_6_COMPUTER_H

#include "program.h"
#include <memory>

class Computer {
  private:
    jnp1_6::Processor processor;
    jnp1_6::Memory memory;
  public:
    Computer(jnp1_6::unsigned_word_t mem_size) : memory(mem_size) {}

    void boot(const jnp1_6::Program &p) {
        p.run(processor, memory);
    }

    void memory_dump(std::ostream &os) const {
        memory.dump(os);
    }
};

#endif // JNP1_6_COMPUTER_H
