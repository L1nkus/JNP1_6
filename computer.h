#ifndef JNP1_4_COMPUTER_H
#define JNP1_4_COMPUTER_H

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

// wsm jakieś ASMelement zamiast Instruction?

// dużo protected?
class Instruction {
/* public: */
protected:
    friend class program;
    // przekazywanie klasy, czy wektora?
    virtual void load(std::vector<int64_t> &arr) {
    }
    virtual void execute(std::vector<int64_t> &arr) {
        whatis("insexec")
    }
    /* virtual void execute(std::vector<int64_t> &arr) = 0; */
};

class Executable : public Instruction {
protected:
    virtual void execute(std::vector<int64_t> &arr) = 0;
};

// nazwa?
class Loadable : public Instruction {
};

class OneArgOp : public Executable {
};

class Dec : public OneArgOp {
};

class Inc : public OneArgOp {
};

class TwoArgOp : public Executable {
};

class Add: public TwoArgOp {
};

/* class value : public Instruction { */
/* }; */

/* class lvalue : public value { */
/* }; */

/* class rvalue : public value { */
/* }; */

class Rvalue : public Instruction {
// tmp
public:
/* protected: */
    virtual void execute(std::vector<int64_t> &arr) {
    }
    int64_t val(){
        return _val;
    }
/* private: */
    // protected?
    int64_t _val;
};

/* class lvalue : public rvalue { */
/* }; */

class Mem : public Rvalue {
public:
    Mem(const Rvalue &addr) : addr(addr) {}
/* protected: */
/* private: */
    void execute(std::vector<int64_t> &arr) override {
    }
    uint64_t cell_pos() {
        return addr.val();
    }
    // nval nie jako rvalue, licz int64_t?
    void set(std::vector<int64_t> &arr, int64_t nval) {
        arr.at(cell_pos()) = nval;
    }
    Rvalue addr;
};

class Mov : public Executable {
public:
    Mov(const Mem &dst, const Rvalue &src) : dst(dst), src(src) {}
/* protected: */
/* private: */
    void execute(std::vector<int64_t> &arr) override {
        whatis("mov exec")
        dst.execute(arr);
        src.execute(arr);
        whatis(src.val())
        dst.set(arr, src.val());
    }
    /* lvalue dst; */
    Mem dst;
    Rvalue src;
};

class Lea : public Rvalue, public Loadable {
    void load(std::vector<int64_t> &arr) override {
    }
};

class Num : public Rvalue {
public:
    // ok tak?
    /* Num(int64_t val) : _val(val) {} */
    Num(int64_t val) {
        _val = val;
    }
protected:
    void execute(std::vector<int64_t> &arr) override {
    }
/* private: */
/*     int64_t val; */
};

// jedynie funkcje, które mogą być pierwszymi w grupie, muszą/powinny zwracać
// wskaźnik do obiektu?
/* Mov *mov(Mem *dst, Rvalue *src) { */
/*     return new Mov(*dst, *src); */
/* } */
Mov *mov(Mem dst, Rvalue src) {
    return new Mov(dst, src);
}

/* std::unique_ptr<Instruction> mem(Rvalue addr) { */
/* std::unique_ptr<Mem> mem(std::unique_ptr<Rvalue> addr) { */
/*     return std::make_unique<Mem>(*addr); */
/* } */

Mem mem(Rvalue addr) {
    return Mem(addr);
}

/* std::unique_ptr<Rvalue> num(int64_t num) { */
/*     return std::make_unique<Num>(num); */
/* } */

Num num(int64_t num) {
    return Num(num);
}

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
        for(auto i: init){
            // or explicit constructor?
            vec.push_back(std::unique_ptr<Instruction>(i));
            // lol, z konstruktorem działa overridowanie funkcji, poniżej jest
            // object slicing chyba
            /* vec.push_back(std::make_unique<Instruction>(*i)); */
        }
    }
private:
    friend class Computer;
    // czy mozemy tak przekazywac :?
    /* void run(Computer &pc) { */
    void run(std::vector<int64_t> &arr) {
        for (auto &i: vec){
            i->load(arr);
        }
        for (auto &i: vec){
            i->execute(arr);
        }
    }
    std::vector<std::unique_ptr<Instruction>> vec;
};

class Computer {
public:
    Computer(uint64_t mem_size) {
        this->mem_size = mem_size;
        arr.resize(mem_size);
    }
    // duży error
    /* void boot(program p) { */
    void boot(program &p) {
        std::fill(arr.begin(), arr.end(), 0);
        // przekazywanie klasy, czy wektora?
        /* p.run(*this); */
        p.run(arr);
    }
    void memory_dump(std::ostream &os) const {
        for (auto const &num: arr)
            os << num << ' ';
    }
private:
    // oddzielne "Memory"?
    uint64_t mem_size;
    // inna nazwa?
    std::vector<int64_t> arr;
    bool zf;
    bool sf;
};

#endif // JNP1_4_COMPUTER_H
