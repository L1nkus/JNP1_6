#ifndef JNP1_6_OOASM_INSTRUCTION_H
#define JNP1_6_OOASM_INSTRUCTION_H

#include "computer_parts.h"
#include "ooasm_elements.h"

namespace jnp1_6 {

class Instruction {
  public:
    virtual void load(Memory &) = 0;

    virtual void execute(Processor &, Memory &) = 0;

    virtual ~Instruction() = default;
};

using InstructionPtr = std::shared_ptr<Instruction>;

class Executable : public Instruction {
  public:
    void load(Memory &) override {}
};

class OneArgOp : public Executable {
  private:
    LvaluePtr arg;
  protected:
    virtual word_t op(word_t val) = 0;
  public:
    OneArgOp(LvaluePtr &&arg) : arg(std::move(arg)) {}

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
    Dec(LvaluePtr &&arg) : OneArgOp(std::move(arg)) {}
};

class Inc final : public OneArgOp {
  protected:
    word_t op(word_t val) override {
        return val + 1;
    }
  public:
    Inc(LvaluePtr &&arg) : OneArgOp(std::move(arg)) {}
};

class TwoArgOp : public Executable {
  private:
    LvaluePtr arg1;
    RvaluePtr arg2;
  protected:
    virtual word_t op(word_t val1, word_t val2) = 0;
  public:
    TwoArgOp(LvaluePtr &&arg1, RvaluePtr &&arg2) : arg1(std::move(arg1)), arg2(std::move(arg2)) {}

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
    Add(LvaluePtr &&arg1, RvaluePtr &&arg2) : TwoArgOp(std::move(arg1), std::move(arg2)) {}
};

class Sub final : public TwoArgOp {
  protected:
    word_t op(word_t val1, word_t val2) override {
        return val1 - val2;
    }
  public:
    Sub(LvaluePtr &&arg1, RvaluePtr &&arg2) : TwoArgOp(std::move(arg1), std::move(arg2)) {}
};

class Mov final : public Executable {
  private:
    LvaluePtr dst;
    RvaluePtr src;
  public:
    Mov(LvaluePtr &&dst, RvaluePtr &&src) : dst(std::move(dst)), src(std::move(src)) {}

    void execute(Processor &, Memory &memory) override {
        dst->set(memory, src->val(memory));
    }
};

class Conditional : public Executable {
  private:
    LvaluePtr arg;
  protected:
    virtual bool cond_fulfilled(Processor &) = 0;
  public:
    Conditional(LvaluePtr &&arg) : arg(std::move(arg)) {}

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
    One(LvaluePtr &&arg) : Conditional(std::move(arg)) {}
};

class Onez final : public Conditional {
  protected:
    bool cond_fulfilled(Processor &processor) override {
        return processor.get_zero_flag();
    }
  public:
    Onez(LvaluePtr &&arg) : Conditional(std::move(arg)) {}
};

class Ones final : public Conditional {
  protected:
    bool cond_fulfilled(Processor &processor) override {
        return processor.get_sign_flag();
    }
  public:
    Ones(LvaluePtr &&arg) : Conditional(std::move(arg)) {}
};

class Loadable : public Instruction {
    void execute(Processor &, Memory &) override {}
};

class Data final : public Loadable {
  private:
    Id id;
    NumPtr num;
  public:
    Data(const char *id, NumPtr &&num) : id(id), num(std::move(num)) {}
    void load(Memory &memory) override {
        memory.add_new_variable(id, num->val(memory));
    }
};
}
#endif //JNP1_6_OOASM_INSTRUCTION_H
