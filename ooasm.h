#ifndef JNP1_6_OOASM_H
#define JNP1_6_OOASM_H

#include "ooasm_instruction.h"
#include <memory>

// TODO indent?
namespace jnp1_6 {

class Executable : public Instruction {
  public:
    void load(Memory &) override {}
};

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

class OneArgOp : public Executable {
  private:
    std::shared_ptr<Lvalue> arg;
  protected:
    virtual word_t op(word_t val) = 0;
  public:
    OneArgOp(Lvalue *arg) : arg(arg) {}

    void execute(Processor &processor, Memory &memory) override {
        word_t res = op(arg->val(memory));
        processor.set_flags(res);
        arg->set(memory, res);
    }
};

class Dec final : public OneArgOp {
  protected:
    word_t op(word_t val) override {
        return val - 1;
    }
  public:
    Dec(Lvalue *arg) : OneArgOp(arg) {}
};

class Inc final : public OneArgOp {
  protected:
    word_t op(word_t val) override {
        return val + 1;
    }
  public:
    Inc(Lvalue *arg) : OneArgOp(arg) {}
};

class TwoArgOp : public Executable {
  private:
    std::shared_ptr<Lvalue> arg1;
    std::shared_ptr<Rvalue> arg2;
  protected:
    virtual word_t op(word_t val1, word_t val2) = 0;
  public:
    TwoArgOp(Lvalue *arg1, Rvalue *arg2) : arg1(arg1), arg2(arg2) {}

    void execute(Processor &processor, Memory &memory) override {
        word_t res = op(arg1->val(memory), arg2->val(memory));
        processor.set_flags(res);
        arg1->set(memory, res);
    }
};

class Add final : public TwoArgOp {
  protected:
    word_t op(word_t val1, word_t val2) override {
        return val1 + val2;
    }
  public:
    Add(Lvalue *arg1, Rvalue *arg2) : TwoArgOp(arg1, arg2) {}
};

class Sub final : public TwoArgOp {
  protected:
    word_t op(word_t val1, word_t val2) override {
        return val1 - val2;
    }
  public:
    Sub(Lvalue *arg1, Rvalue *arg2) : TwoArgOp(arg1, arg2) {}
};

class Mov final : public Executable {
  private:
    std::shared_ptr<Lvalue> dst;
    std::shared_ptr<Rvalue> src;
  public:
    Mov(Lvalue *dst, Rvalue *src) : dst(dst), src(src) {}

    void execute(Processor &, Memory &memory) override {
        dst->set(memory, src->val(memory));
    }
};

class Conditional : public Executable {
  private:
    std::shared_ptr<Lvalue> arg;
  protected:
    virtual bool cond_fulfilled(Processor &) = 0;
  public:
    Conditional(Lvalue *arg) : arg(arg) {}

    void execute(Processor &processor, Memory &memory) override {
        if (cond_fulfilled(processor))
            arg->set(memory, 1);
    }
};

class One final : public Conditional {
  protected:
    bool cond_fulfilled(Processor &) override {
        return true;
    }
  public:
    One(Lvalue *arg) : Conditional(arg) {}
};

class Onez final : public Conditional {
  protected:
    bool cond_fulfilled(Processor &processor) override {
        return processor.get_zero_flag();
    }
  public:
    Onez(Lvalue *arg) : Conditional(arg) {}
};

class Ones final : public Conditional {
  protected:
    bool cond_fulfilled(Processor &processor) override {
        return processor.get_sign_flag();
    }
  public:
    Ones(Lvalue *arg) : Conditional(arg) {}
};

class Loadable : public Instruction {
    void execute(Processor &, Memory &) override {}
};

class Data final : public Loadable {
  private:
    Id id;
    std::shared_ptr<Num> num;
  public:
    Data(const char *id, Num *num) : id(id), num(num) {}
    void load(Memory &memory) override {
        memory.add_new_variable(id, num->val(memory));
    }
};
}

// TODO, czy funkcje powinny być inline?
jnp1_6::Mov *mov(jnp1_6::Lvalue *dst, jnp1_6::Rvalue *src) {
    return new jnp1_6::Mov(dst, src);
}

jnp1_6::Lvalue *mem(jnp1_6::Rvalue *addr) {
    return new jnp1_6::Mem(addr);
}

jnp1_6::Lea *lea(const char *id) {
    return new jnp1_6::Lea(id);
}

jnp1_6::Inc *inc(jnp1_6::Lvalue *lvalue) {
    return new jnp1_6::Inc(lvalue);
}

jnp1_6::Dec *dec(jnp1_6::Lvalue *lvalue) {
    return new jnp1_6::Dec(lvalue);
}

jnp1_6::Add *add(jnp1_6::Lvalue *arg1, jnp1_6::Rvalue *arg2) {
    return new jnp1_6::Add(arg1, arg2);
}

jnp1_6::Sub *sub(jnp1_6::Lvalue *arg1, jnp1_6::Rvalue *arg2) {
    return new jnp1_6::Sub(arg1, arg2);
}

jnp1_6::Data *data(const char *id, jnp1_6::Num *num) {
    return new jnp1_6::Data(id, num);
}

jnp1_6::One *one(jnp1_6::Lvalue *arg) {
    return new jnp1_6::One(arg);
}

jnp1_6::Onez *onez(jnp1_6::Lvalue *arg) {
    return new jnp1_6::Onez(arg);
}

jnp1_6::Ones *ones(jnp1_6::Lvalue *arg) {
    return new jnp1_6::Ones(arg);
}

jnp1_6::Num *num(jnp1_6::word_t value) {
    return new jnp1_6::Num(value);
}

#endif // JNP1_6_OOASM_H
