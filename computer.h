#ifndef JNP1_6_COMPUTER_H
#define JNP1_6_COMPUTER_H

#include <algorithm>
#include <cstdint>
#include <cstddef>
#include <initializer_list>
#include <utility>
#include <vector>
#include <list>
#include <tuple>
#include <functional>
#include <type_traits>
#include <stdexcept>
#include <limits>
#include <iostream> // do uproszczenia
#include <memory>

#define whatis(x) std::cerr << #x << " is " << (x) << std::endl;

// część rzeczy do przeniesienia do ooasm.h

// execute, set_val/load

namespace jnp1_6 {

    class Computer;

    // [todo] move it
    using word_t = int64_t;
    using unsigned_word_t = std::make_unsigned_t<word_t>;

    class Id {
      private:
        const std::string id;
      public:
        Id(const char *id) : id(id) {
            if (this->id.empty() || this->id.size() > 10) {
                throw std::invalid_argument("Invalid id length");
            }
        }

        const std::string &get_string() const {
            return id;
        }
    };

    // [fixme]
    class Processor {
      private:
        bool zf;
        bool sf;
      public:
        Processor() : zf(false), sf(false) {}

        bool get_zero_flag() const {
            return zf;
        }

        bool get_sign_flag() const {
            return sf;
        }

        void set_flags(word_t res) {
            zf = res == 0;
            sf = res < 0;
        }
    };

    class Memory {
      private:
        // holds pair {variable name, memory address}
        using data_t = std::pair<std::string, size_t>;

        std::vector<data_t> variables_register;
        std::vector<word_t> variables;

      public:
        Memory() = default;

        word_t value_at(unsigned_word_t pos) const {
            return variables[pos];
        }

        unsigned_word_t find_variable_address(const Id &id) const {
            // Because ids are short, it is faster than using an unordered map.
            auto it = std::lower_bound(variables_register.begin(),
                                       variables_register.end(),
                                       std::pair<std::string,size_t>{id.get_string(),0});
            if (it == variables_register.end() || it->first != id.get_string()) {
                throw std::invalid_argument("Variable " + id.get_string() +
                    " is not defined.");
            }
            else {
                return it->second;
            }
        }
    };

    class Instruction {
    public:
        virtual void load(Memory &) = 0;

        virtual void execute(Processor &, Memory &) = 0;

        virtual ~Instruction() = 0;
    };

    class Executable : public Instruction {
        void load(Memory &) override {} //[todo] test if works
    };

    class Loadable : public Instruction {
        void execute(Processor &, Memory &) override {}//[todo] test if works
    };

    class Rvalue {
      public:
        virtual word_t val(Memory &) const = 0;
    };

    // Note: This is based on the observation that all Lvalues are also Pvalues.
    // Since the definitions of those are not specified enough,
    // we assume that it is true for all Lvalues.
    // Otherwise, another interface combining those two should be made.
    class Lvalue : public Rvalue {
      public:
        virtual void set(Memory &, word_t) = 0;
    };

    class Num : public Rvalue {
      private:
        const word_t value;
      public:
        Num(word_t value) : value(value) {}

        word_t val(Memory &memory) const override {
            return value;
        }
    };

    class Lea : public Rvalue {
      private:
        const Id id;
      public:
        Lea(const char *id) : id(id) {}

        word_t val(Memory &memory) const override {
            return memory.find_variable_address(id);
        }
    };

    class Mem : public Lvalue {
      public:
        Mem(Rvalue *addr) : addr(addr) {}

        word_t val(Memory &memory) const override {
            return addr->val(memory);
        }

        void set(Memory &, word_t) override {

        }
        // TODO: zmieniłem z unique na shared ~ab
        std::shared_ptr<Rvalue> addr;
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

    class Dec : public OneArgOp {
      protected:
        word_t op(word_t val) override {
            return val - 1;
        }
      public:
        Dec(Lvalue *arg) : OneArgOp(arg) {}
    };

    class Inc : public OneArgOp {
      protected:
        word_t op(word_t val) override {
            return val + 1;
        }
      public:
        Inc(Lvalue *arg) : OneArgOp(arg) {}
    };

    class TwoArgOp : public Executable {
    public:
        TwoArgOp(Lvalue *arg1, Rvalue *arg2) : arg1(arg1), arg2(arg2) {}
        void execute(Pc &pc, data_vec_t &data_vec) override {
            arg1->execute(pc, data_vec);
            arg2->execute(pc, data_vec);
            word_t res = op(arg1->val(), arg2->val());
            setflags(pc, res);
            arg1->set(pc, res);
        }
        virtual word_t op(word_t val1, word_t val2) = 0;
        std::unique_ptr<Lvalue> arg1;
        std::unique_ptr<Rvalue> arg2;
    };

    class Add: public TwoArgOp {
    public:
        Add(Lvalue *arg1, Rvalue *arg2) : TwoArgOp(arg1, arg2) {}
        word_t op(word_t val1, word_t val2) override {
            return val1 + val2;
        }
    };

    class Sub : public TwoArgOp {
    public:
        Sub(Lvalue *arg1, Rvalue *arg2) : TwoArgOp(arg1, arg2) {}
        word_t op(word_t val1, word_t val2) override {
            return val1 - val2;
        }
    };

    class Mov : public Executable {
    public:
        // TODO: const przekazywanie wskaźników w konstruktorach?
        Mov(Lvalue *dst, Rvalue *src) : dst(dst), src(src) {}
        void execute(Pc &pc, data_vec_t &data_vec) override {
            dst->execute(pc, data_vec);
            src->execute(pc, data_vec);
            dst->set(pc, src->val());
        }
        std::unique_ptr<Lvalue> dst;
        std::unique_ptr<Rvalue> src;
    };

    class Conditional : public Executable {
    public:
        Conditional(Lvalue *arg) : arg(arg) {}
        virtual void execute(Pc &pc, data_vec_t &data_vec) override {
            arg->execute(pc, data_vec);
            if (cond_fulfilled(pc))
                arg->set(pc, 1);
        }
        virtual bool cond_fulfilled(Pc &pc) = 0;
        std::unique_ptr<Lvalue> arg;
    };

    class One : public Conditional {
    public:
        One(Lvalue *arg) : Conditional(arg) {}
        virtual bool cond_fulfilled(Pc &) override {
            return true;
        }
    };

    class Onez : public Conditional {
    public:
        Onez(Lvalue *arg) : Conditional(arg) {}
        virtual bool cond_fulfilled(Pc &pc) override {
            return pc.zf;
        }
    };

    class Ones : public Conditional {
    public:
        Ones(Lvalue *arg) : Conditional(arg) {}
        virtual bool cond_fulfilled(Pc &pc) override {
            return pc.sf;
        }
    };

    class Data : public Loadable {
    public:
        Data (const std::string &id, Num *num) : id(id), num(num) {
            if(id.size() < 1 || id.size() > 10){
                throw std::invalid_argument("Nieprawidłowa długość identyfikatora");
            }
        }
        void load(Pc &pc, data_vec_t &data_vec) override {
            pc.arr.at(data_vec.size()) = num->val();
            data_vec.push_back({id, data_vec.size()});
        }
        std::string id;
        std::unique_ptr<Num> num;
    };
}

jnp1_6::Mov *mov(jnp1_6::Lvalue *dst, jnp1_6::Rvalue *src) {
    return new jnp1_6::Mov(dst, src);
}

jnp1_6::Lvalue *mem(jnp1_6::Rvalue *addr) {
    return new jnp1_6::Mem(addr);
}

jnp1_6::Lea *lea(const char *id) {
    return new jnp1_6::Lea(id);
}

jnp1_6::Inc *inc(jnp1_6::Lvalue *mem) {
    return new jnp1_6::Inc(mem);
}

jnp1_6::Dec *dec(jnp1_6::Lvalue *mem) {
    return new jnp1_6::Dec(mem);
}

jnp1_6::Add *add(jnp1_6::Lvalue *arg1, jnp1_6::Rvalue *arg2) {
    return new jnp1_6::Add(arg1, arg2);
}

jnp1_6::Sub *sub(jnp1_6::Lvalue *arg1, jnp1_6::Rvalue *arg2) {
    return new jnp1_6::Sub(arg1, arg2);
}

jnp1_6::Data *data(const std::string &id, jnp1_6::Num *num) {
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

jnp1_6::Num *num(jnp1_6::word_t num) {
    return new jnp1_6::Num(num);
}

// funkcja vs struct??

// bo to niby trochę odbiega od konwencji, że funkcje są od małej litery, a
// klasy od dużej - czy może powinnśmy mieć też funkcję program(), która zwraca
// klasę Program(), aby była spójność stylowa?

class program {
public:
    program(const std::vector<jnp1_6::Instruction*> &init) {
        vec.reserve(init.size());
        for(auto i: init){
            vec.push_back(std::unique_ptr<jnp1_6::Instruction>(i));
        }
    }
private:
    friend class Computer;
    void run(jnp1_6::Pc &pc) {
        jnp1_6::data_vec_t label_vec;
        for (auto &i: vec){
            i->load(pc, label_vec);
        }
        // Abyśmy mogli wyszukiwać binarnie klucz.
        std::sort(label_vec.begin(), label_vec.end());
        for (auto &i: vec){
            i->execute(pc, label_vec);
        }
    }
    std::vector<std::unique_ptr<jnp1_6::Instruction>> vec;
};

class Computer {
public:
    Computer(jnp1_6::unsigned_word_t mem_size) {
        this->mem_size = mem_size;
        pc.arr.resize(mem_size);
    }
    void boot(program &p) {
        std::fill(pc.arr.begin(), pc.arr.end(), 0);
        pc.sf = pc.zf = false;
        // przekazywanie klasy, czy wektora?
        p.run(pc);
    }
    void memory_dump(std::ostream &os) const {
        /* for (auto const &num: arr) */
        // funkcja tego structa może?
        for (auto const &num: pc.arr)
            os << num << ' ';
    }
private:
    // oddzielne "Memory"?
    jnp1_6::unsigned_word_t mem_size;
    // inna nazwa?
    jnp1_6::Pc pc;
};

#endif // JNP1_6_COMPUTER_H