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

    using data_vec_t = std::vector<std::pair<std::string, size_t>>; // {klucz stringowy, nr komórki}
    using word_type = int64_t;
    using unsigned_word_type = std::make_unsigned_t<word_type>;

    // inna nazwa na to?
    struct Pc {
        std::vector<word_type> arr;
        bool zf;
        bool sf;
    };

    // wsm jakieś ASMelement zamiast Instruction?

    // dużo protected?
    class Instruction {
    public:
        friend class program;
        // przekazywanie klasy, czy wektora?
        virtual void load(Pc &, data_vec_t &) {
        }

        virtual void execute(Pc &, data_vec_t &) {
        }

        // = default zamiast pustych klamr?
        virtual ~Instruction() {
        }
    };

    class Executable : public virtual Instruction {
    protected:
        virtual void execute(Pc &pc, data_vec_t &data_vec) = 0;
    };

    // nazwa?
    // TODO: czy tutaj zostawić virtual? Bo nie musi być, gdyż Load tylko od tego
    // Loadable dziedziczy, czyli nie ma żadnego diamentu
    class Loadable : public virtual Instruction {
    protected:
        virtual void load(Pc &pc, data_vec_t &data_vec) = 0;
    };

    class Rvalue : public virtual Instruction {
    public:
        word_type val(){
            return _val;
        }
        word_type _val;
    };

    class Lea : public Rvalue, public Executable {
    public:
        Lea(const std::string &id) : id(id) {
            if(id.size() < 1 || id.size() > 10){
                throw std::invalid_argument("Nieprawidłowa długość identyfikatora");
            }
        }
        // Ifowanie, jak już był ustawiony?
        // Bo możemy dany program wiele razy odpalić w teorii
        void execute(Pc &, data_vec_t &data_vec) override {
            auto it = std::lower_bound(data_vec.begin(), data_vec.end(), std::pair<std::string,size_t>{id,0});
            if (it == data_vec.end() || it->first != id) {
                throw std::invalid_argument("Brak deklaracji zmiennej, do której "
                        "odwołuje się instrukcja Lea");
            }
            else {
                _val = it->second;
            }

        }
        std::string id;
    };

    class Lvalue : public Rvalue, public Executable {
    public:
        Lvalue(Rvalue *addr) : addr(addr) {}
        void execute(Pc &pc, data_vec_t &data_vec) override {
            // TODO upewnij się, że all rvalue mają execute (bo mem może być jako
            // rvalue, a on potrzebuje execute)
            addr->execute(pc, data_vec);
            _val = pc.arr.at(cell_pos());
        }
        unsigned_word_type cell_pos() {
            return addr->val();
        }
        void set(Pc &pc, word_type nval) {
            pc.arr.at(cell_pos()) = nval;
        }
        // TODO: unique_ptr, czy shared_ptr? (Jak programy mają być kopiowalne,
        // to chyba musi być shared_ptr?)
        std::unique_ptr<Rvalue> addr;
    };

    void setflags(Pc &pc, word_type res) {
        pc.zf = res == 0;
        pc.sf = res < 0;
    }

    class OneArgOp : public Executable {
    public:
        OneArgOp(Lvalue *arg) : arg(arg) {}
        // still virtual? jak z tym
        void execute(Pc &pc, data_vec_t &data_vec) override {
            arg->execute(pc, data_vec);
            word_type res = op(arg->val());
            setflags(pc, res);
            arg->set(pc, res);
        }
        virtual word_type op(word_type val) = 0;
        std::unique_ptr<Lvalue> arg;
    };

    class Dec : public OneArgOp {
    public:
        Dec(Lvalue *arg) : OneArgOp(arg) {}
        word_type op(word_type val) override {
            return val - 1;
        }
    };

    class Inc : public OneArgOp {
    public:
        Inc(Lvalue *arg) : OneArgOp(arg) {}
        word_type op(word_type val) override {
            return val + 1;
        }
    };

    class TwoArgOp : public Executable {
    public:
        TwoArgOp(Lvalue *arg1, Rvalue *arg2) : arg1(arg1), arg2(arg2) {}
        void execute(Pc &pc, data_vec_t &data_vec) override {
            arg1->execute(pc, data_vec);
            arg2->execute(pc, data_vec);
            word_type res = op(arg1->val(), arg2->val());
            setflags(pc, res);
            arg1->set(pc, res);
        }
        virtual word_type op(word_type val1, word_type val2) = 0;
        std::unique_ptr<Lvalue> arg1;
        std::unique_ptr<Rvalue> arg2;
    };

    class Add: public TwoArgOp {
    public:
        Add(Lvalue *arg1, Rvalue *arg2) : TwoArgOp(arg1, arg2) {}
        word_type op(word_type val1, word_type val2) override {
            return val1 + val2;
        }
    };

    class Sub : public TwoArgOp {
    public:
        Sub(Lvalue *arg1, Rvalue *arg2) : TwoArgOp(arg1, arg2) {}
        word_type op(word_type val1, word_type val2) override {
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


    class Num : public Rvalue {
    public:
        Num(word_type val) {
            _val = val;
        }
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
    return new jnp1_6::Lvalue(addr);
}

jnp1_6::Lea *lea(const std::string &id) {
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

jnp1_6::Num *num(jnp1_6::word_type num) {
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
    Computer(jnp1_6::unsigned_word_type mem_size) {
        this->mem_size = mem_size;
        pc.arr.resize(mem_size);
    }
    void boot(program &p) {
        std::fill(pc.arr.begin(), pc.arr.end(), 0);
        pc.sf = pc.zf = 0;
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
    jnp1_6::unsigned_word_type mem_size;
    // inna nazwa?
    jnp1_6::Pc pc;
};

#endif // JNP1_6_COMPUTER_H
