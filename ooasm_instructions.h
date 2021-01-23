#ifndef JNP1_6_OOASM_INSTRUCTION_H_
#define JNP1_6_OOASM_INSTRUCTION_H_

#include "computer_parts.h"
#include "ooasm_elements.h"

namespace jnp1_6 {
class Instruction {
  public:
    virtual void load(Memory &) = 0;

    virtual void execute(Processor &, Memory &) = 0;

    virtual ~Instruction() = default;
};

class Executable : public Instruction {
  public:
    void load(Memory &) override {}
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
    std::shared_ptr<Num> num;
    Id id;
  public:
    Data(const char *id, Num *num) : num(num), id(id) {}
    void load(Memory &memory) override {
        memory.add_new_variable(id, num->val(memory));
    }
};
}
#endif //JNP1_6_OOASM_INSTRUCTION_H_
