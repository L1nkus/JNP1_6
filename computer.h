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

// duzo virtual powinno byc?

// namespace

class Computer;

using data_vec_t = std::vector<std::pair<std::string, size_t>>; // {klucz stringowy, nr komórki}

// inna nazwa na to?
struct Pc {
    std::vector<int64_t> arr;
    bool zf;
    bool sf;
};

// wsm jakieś ASMelement zamiast Instruction?

// virtualne destruktory, dziedziczenie

/* int destr_cnt; */

// dużo protected?
class Instruction {
public:
/* protected: */
    friend class program;
    // przekazywanie klasy, czy wektora?
    virtual void load(Pc &, data_vec_t &) {
        /* whatis("other load") */
    }
    /* virtual void set_val(Pc &pc, data_vec_t &data_vec) { */
    /*     whatis("other set_val") */
    /* } */

    /* virtual void execute(Pc &pc) { */
    virtual void execute(Pc &, data_vec_t &) {
        /* whatis("insexec") */
    }
    // lol, wystarczy tylko w tym miejscu ten jeden virtual destructor, i
    // nigdzie niżej nie trzeba, i jest git

    // = default zamiast pustych klamr?
    virtual ~Instruction() {
        /* whatis("~instruction") */
        /* ++destr_cnt; */
    }
    /* virtual void execute(Pc &pc) = 0; */
};

class Executable : public virtual Instruction {
/* class Executable : public Instruction { */
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

/* class Rvalue : public Instruction { */
class Rvalue : public virtual Instruction {
// tmp
public:
/* protected: */
    /* virtual void execute(Pc &pc) { */
    /* } */
    int64_t val(){
        return _val;
    }
/* private: */
    // protected?
    int64_t _val;
};

/* class Lea : public Rvalue, public Settable { */
// -> Executable jednak
/* class Lea : public Rvalue { */
class Lea : public Rvalue, public Executable {
public:
    Lea(const std::string &id) : id(id) {
        if(id.size() < 1 || id.size() > 10){
            throw std::invalid_argument("Nieprawidłowa długość identyfikatora");
        }
    }
    // wywalenie set_val, i zamiast tego ifowanie tego
    /* void set_val(Pc &pc, data_vec_t &data_vec) override { */
    void execute(Pc &, data_vec_t &data_vec) override {
        whatis("lea set_val")
        // make optymalniejsze def
        /* for (size_t i = 0; i < data_vec.size(); ++i) { */
        /*     if(data_vec */
        /* } */
        /* auto it = std::find(data_vec.begin(), data_vec.end(), id); */
        // -> optymalniejsze
        auto it = std::lower_bound(data_vec.begin(), data_vec.end(), std::pair<std::string,size_t>{id,0});
        if (it == data_vec.end() || it->first != id) {
            throw std::invalid_argument("Brak deklaracji zmiennej, do której "
                    "odwołuje się instrukcja Lea");
        }
        else {
            /* _val = it - data_vec.begin(); */
            _val = it->second;
        }

    }
    std::string id;
};

/* class value : public Instruction { */
/* }; */

/* class lvalue : public value { */
/* }; */

/* class rvalue : public value { */
/* }; */

/* class lvalue : public rvalue { */
/* }; */

// Lvalue zamiast Mem?

// executable też
/* class Mem : public Rvalue { */
class Mem : public Rvalue, public Executable {
public:
    /* Mem(const Rvalue &addr) : addr(addr) {} */
    // nie możemy mieć kopiowania I guess?
    Mem(Rvalue *addr) : addr(addr) {}
    /* Mem(std::unique_ptr<Rvalue> &addr) : addr(std::move(addr)) {} */
    /* Mem(std::unique_ptr<Rvalue> &&addr) : addr(std::move(addr)) {} */
/* protected: */
/* private: */
    void execute(Pc &pc, data_vec_t &data_vec) override {
        // TODO upewnij się, że all rvalue mają execute (bo mem może być jako
        // rvalue, a on potrzebuje execute)
        addr->execute(pc, data_vec);
        _val = pc.arr.at(cell_pos());
    }
    /* virtual void set_val(Pc &pc, data_vec_t &data_vec) override { */
    /*     whatis("mem set_val") */
    /*     addr->set_val(pc, data_vec); */
    /* } */
    // zamiana na std::make_unsigned_t<word_type>
    // a word_type jako using z int64_t
    uint64_t cell_pos() {
        return addr->val();
    }
    // nval nie jako rvalue, licz int64_t?
    void set(Pc &pc, int64_t nval) {
        pc.arr.at(cell_pos()) = nval;
    }
    /* Rvalue addr; */
    // ^^slicing w przypadku Lea
    // w sumie wszędzie lepiej jednak nie trzymać kopii w takim razie?
    /* Rvalue &addr; */
    /* Rvalue *addr; */
    std::unique_ptr<Rvalue> addr;
    // wkaźniki chyba lepszym pomysłem
    // -> zwykłe, czy unique_ptr, czy shared_ptr? (aby można było kopiować
    // ewentualnie jeśli to wymagane)
};

void setflags(Pc &pc, int64_t res) {
    pc.zf = res == 0;
    pc.sf = res < 0;
}

// todo: ustawianie flag
// -> już zrobione
class OneArgOp : public Executable {
public:
    OneArgOp(Mem *arg) : arg(arg) {}
    // still virtual? jak z tym
    void execute(Pc &pc, data_vec_t &data_vec) override {
        arg->execute(pc, data_vec);
        int64_t res = op(arg->val());
        setflags(pc, res);
        arg->set(pc, res);
    }
    /* virtual void set_val(Pc &pc, data_vec_t &data_vec) override { */
    /*     whatis("oneargop set_val") */
    /*     arg->set_val(pc, data_vec); */
    /* } */
    virtual int64_t op(int64_t val) = 0;
    std::unique_ptr<Mem> arg;
};

class Dec : public OneArgOp {
public:
    Dec(Mem *arg) : OneArgOp(arg) {}
    int64_t op(int64_t val) override {
        return val - 1;
    }
};

class Inc : public OneArgOp {
public:
    Inc(Mem *arg) : OneArgOp(arg) {}
    int64_t op(int64_t val) override {
        return val + 1;
    }
};

// todo: ustawianie flag
class TwoArgOp : public Executable {
public:
    TwoArgOp(Mem *arg1, Rvalue *arg2) : arg1(arg1), arg2(arg2) {}
    void execute(Pc &pc, data_vec_t &data_vec) override {
        whatis("twoargop execute")
        arg1->execute(pc, data_vec);
        arg2->execute(pc, data_vec);
        whatis(arg1->val())
        whatis(arg2->val())
        int64_t res = op(arg1->val(), arg2->val());
        setflags(pc, res);
        arg1->set(pc, res);
    }
    /* virtual void set_val(Pc &pc, data_vec_t &data_vec) override { */
    /*     whatis("twoargop set_val") */
    /*     arg1->set_val(pc, data_vec); */
    /*     arg2->set_val(pc, data_vec); */
    /* } */
    virtual int64_t op(int64_t val1, int64_t val2) = 0;
    std::unique_ptr<Mem> arg1;
    std::unique_ptr<Rvalue> arg2;
};

class Add: public TwoArgOp {
public:
    Add(Mem *arg1, Rvalue *arg2) : TwoArgOp(arg1, arg2) {}
    int64_t op(int64_t val1, int64_t val2) override {
        return val1 + val2;
    }
};

class Sub : public TwoArgOp {
public:
    Sub(Mem *arg1, Rvalue *arg2) : TwoArgOp(arg1, arg2) {}
    int64_t op(int64_t val1, int64_t val2) override {
        return val1 - val2;
    }
};

class Mov : public Executable {
public:
    // const?
    Mov(Mem *dst, Rvalue *src) : dst(dst), src(src) {}
/* protected: */
/* private: */
    void execute(Pc &pc, data_vec_t &data_vec) override {
        whatis("mov exec")
        /* static_cast<Instruction>(dst).execute(arr); */
        dst->execute(pc, data_vec);
        src->execute(pc, data_vec);
        whatis(src->val())
        dst->set(pc, src->val());
    }
    /* virtual void set_val(Pc &pc, data_vec_t &data_vec) override { */
    /*     whatis("mov set_val") */
    /*     dst->set_val(pc, data_vec); */
    /*     src->set_val(pc, data_vec); */
    /* } */
    /* lvalue dst; */
    std::unique_ptr<Mem> dst;
    std::unique_ptr<Rvalue> src;
};


class Num : public Rvalue {
public:
    // ok tak?
    /* Num(int64_t val) : _val(val) {} */
    Num(int64_t val) {
        _val = val;
    }
protected:
    /* void execute(Pc &pc, data_vec_t &data_vec) override { */
    /* } */
/* private: */
/*     int64_t val; */
};

class Conditional : public Executable {
public:
    Conditional(Mem *arg) : arg(arg) {}
    virtual void execute(Pc &pc, data_vec_t &data_vec) override {
        arg->execute(pc, data_vec);
        if (cond_fulfilled(pc))
            arg->set(pc, 1);
    }
    virtual bool cond_fulfilled(Pc &pc) = 0;
    std::unique_ptr<Mem> arg;
};

class One : public Conditional {
public:
    One(Mem *arg) : Conditional(arg) {}
    virtual bool cond_fulfilled(Pc &) override {
        return true;
    }
};

class Onez : public Conditional {
public:
    Onez(Mem *arg) : Conditional(arg) {}
    virtual bool cond_fulfilled(Pc &pc) override {
        return pc.zf;
    }
};

class Ones : public Conditional {
public:
    Ones(Mem *arg) : Conditional(arg) {}
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

// jedynie funkcje, które mogą być pierwszymi w grupie, muszą/powinny zwracać
// wskaźnik do obiektu?
/* Mov *mov(Mem *dst, Rvalue *src) { */
/*     return new Mov(*dst, *src); */
/* } */

// te funkcje mogą być friend?

Mov *mov(Mem *dst, Rvalue *src) {
    return new Mov(dst, src);
}

/* std::unique_ptr<Instruction> mem(Rvalue addr) { */
/* std::unique_ptr<Mem> mem(std::unique_ptr<Rvalue> addr) { */
/*     return std::make_unique<Mem>(*addr); */
/* } */

// ref aby nie było slicingu

// jednak najlepiej jak wszędzie wskaźniki są zwracane def
Mem *mem(Rvalue *addr) {
/* std::unique_ptr<Mem> mem(std::unique_ptr<Rvalue> addr) { */
/* std::unique_ptr<Mem> mem(std::unique_ptr<Rvalue> &&addr) { */
    return new Mem(addr);
    /* return std::make_unique<Mem>(addr); */
    /* return std::make_unique<Mem>(std::move(addr)); */
    /* return std::move(std::make_unique<Mem>(std::move(addr))); */
}

Lea *lea(const std::string &id) {
    return new Lea(id);
}

Inc *inc(Mem *mem) {
    return new Inc(mem);
}

Dec *dec(Mem *mem) {
    return new Dec(mem);
}

Add *add(Mem *arg1, Rvalue *arg2) {
    return new Add(arg1, arg2);
}

Sub *sub(Mem *arg1, Rvalue *arg2) {
    return new Sub(arg1, arg2);
}

Data *data(const std::string &id, Num *num) {
    return new Data(id, num);
}

One *one(Mem *arg) {
    return new One(arg);
}

Onez *onez(Mem *arg) {
    return new Onez(arg);
}

Ones *ones(Mem *arg) {
    return new Ones(arg);
}

/* std::unique_ptr<Rvalue> num(int64_t num) { */
/*     return std::make_unique<Num>(num); */
/* } */

Num *num(int64_t num) {
    return new Num(num);
}
/* std::unique_ptr<Num> num(int64_t num) { */
/*     /1* return std::make_unique<Num>(num); *1/ */
/*     return std::move(std::make_unique<Num>(num)); */
/* } */

/* template<typename... Args> */
/* std::unique_ptr<Instruction> num(Args&&... args) { */
/*     return std::make_unique<Num>(std::forward(args...)); */
/* } */

/* template<typename... Args> */
/* std::unique_ptr<Instruction> mov(Args&&... args) { */
/*     return std::make_unique<Mov>(std::forward(args...)); */
/* } */

/* template<typename... Args> */
/* std::unique_ptr<Instruction> mem(Args&&... args) { */
/*     return std::make_unique<Mem>(std::forward(args...)); */
/* } */

// funkcja vs struct??
class program {
public:
    // Musi być jakoś przez wskaźnik / referencję, bo inaczej jest object
    // slicing, i wszystkie dziedziczone klasy tracą swoje właściwości.

    // wait, to nie musi być przecie std::initializer_list?
    /* program(std::initializer_list<Instruction > init) : vec(init) {} */
    // wait, tuple? idk
    /* program(std::vector<Instruction *> init) : vec(init) {} */

    // nvm, możemy po prostu dać je jako funkcje zwracające unique_ptr
    // odpowiedniej klasy xd
    /* program(std::tuple<T> init) : vec(init) {} */
    /* program(std::vector<std::unique_ptr<Instruction>> init) : vec(init) {} */
    program(const std::vector<Instruction*> &init) {
        /* vec = std::vector<std::unique_ptr<Instruction>>(init); */
        // &?
        for(auto i: init){
            // or explicit constructor?
            vec.push_back(std::unique_ptr<Instruction>(i));
            // lol, z konstruktorem działa overridowanie funkcji, poniżej jest
            // object slicing chyba
            /* vec.push_back(std::make_unique<Instruction>(*i)); */
        }
    }
    /* program(const std::vector<std::unique_ptr<Instruction>> &init) { */
    /*     for(auto &i: init){ */
    /*         // or explicit constructor? */
    /*         vec.push_back(std::move(i)); */
    /*         // lol, z konstruktorem działa overridowanie funkcji, poniżej jest */
    /*         // object slicing chyba */
    /*         /1* vec.push_back(std::make_unique<Instruction>(*i)); *1/ */
    /*     } */
    /* } */
private:
    friend class Computer;
    // czy mozemy tak przekazywac :?
    /* void run(Computer &pc) { */
    void run(Pc &pc) {
    /* void run(std::vector<int64_t> &arr) { */
        /* std::vector<std::string> label_vec; */
        data_vec_t label_vec;
        for (auto &i: vec){
            /* i->load(arr, label_vec); */
            i->load(pc, label_vec);
        }
        // abysmy mogli wyszukiwać binarnie klucz
        std::sort(label_vec.begin(), label_vec.end());
        /* for (auto &i: vec){ */
        /*     /1* i->set_val(arr, label_vec); *1/ */
        /*     i->set_val(pc, label_vec); */
        /* } */
        for (auto &i: vec){
            /* i->execute(arr); */
            /* i->execute(pc); */
            i->execute(pc, label_vec);
        }
    }
    std::vector<std::unique_ptr<Instruction>> vec;
};

class Computer {
public:
    Computer(uint64_t mem_size) {
        this->mem_size = mem_size;
        pc.arr.resize(mem_size);
    }
    // duży error
    /* void boot(program p) { */
    void boot(program &p) {
        std::fill(pc.arr.begin(), pc.arr.end(), 0);
        pc.sf = pc.zf = 0;
        // przekazywanie klasy, czy wektora?
        /* p.run(*this); */
        /* p.run(arr); */
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
    uint64_t mem_size;
    // inna nazwa?
    Pc pc;
    /* Pc pc{}; */
    /* std::vector<int64_t> arr; */
    /* bool zf; */
    /* bool sf; */
};

#endif // JNP1_6_COMPUTER_H
