#ifndef JNP1_6_OOASM_ELEMENTS_H_
#define JNP1_6_OOASM_ELEMENTS_H_

#include "ooasm_types.h"

namespace jnp1_6 {

class Rvalue {
  public:
    virtual word_t val(Memory &) const = 0;

    virtual ~Rvalue() = default;
};

// Note: This is based on the observation that all Lvalues are also Rvalues.
// Since the definitions of those are not specified enough,
// we assume that it is true for all Lvalues.
// Otherwise, another interface combining those two should be made.
class Lvalue : public Rvalue {
  public:
    virtual void set(Memory &, word_t) = 0;
};

class Num final : public Rvalue {
  private:
    const word_t value;
  public:
    Num(word_t value) : value(value) {}

    word_t val(Memory &) const override {
        return value;
    }
};

class Lea final : public Rvalue {
  private:
    const Id id;
  public:
    Lea(const char *id) : id(id) {}

    word_t val(Memory &memory) const override {
        return memory.find_variable_address(id);
    }
};

class Mem final : public Lvalue {
  private:
    std::shared_ptr<Rvalue> addr;
  public:
    Mem(Rvalue *addr) : addr(addr) {}

    word_t val(Memory &memory) const override {
        return memory.value_at(addr->val(memory));
    }

    void set(Memory &memory, word_t value) override {
        memory.set_value(addr->val(memory), value);
    }
};
}

#endif //JNP1_6_OOASM_ELEMENTS_H_
