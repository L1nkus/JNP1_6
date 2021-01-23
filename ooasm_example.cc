/* #include "ooasm.h" */
#include "computer.h"
#include "ooasm.h"
#include <string>
#include <sstream>
#include <cassert>

namespace {
    std::string memory_dump(Computer const& computer) {
        std::stringstream ss;
        computer.memory_dump(ss);
        return ss.str();
    }
}

#include <vector>
int main() {
    /* auto sth = mov(mem(num(0)), num(42)); */
    /* std::vector<Instruction*> init{sth}; */
    /* std::vector<std::unique_ptr<Instruction>> init3{sth}; */
    auto ooasm_move = program({
        mov(mem(num(0)), num(42))
    });

    /* auto sth2 = mem(num(0)); */
    /* auto ooasm_move = program({ */
    /*     mem(num(0)) */
    /* }); */
    Computer computer1(1);
    computer1.boot(ooasm_move);
    std::cout << memory_dump(computer1) << std::endl;
    assert(memory_dump(computer1) == "42 ");

    auto ooasm_ones = program({
        dec(mem(num(1))),
        ones(mem(num(0)))
    });
    Computer computer2(2);
    computer2.boot(ooasm_ones);
    whatis(memory_dump(computer2))
    assert(memory_dump(computer2) == "1 -1 ");

    auto ooasm_data = program({
        inc(mem(lea("a"))),
        data("a", num(0)),
        data("b", num(2)),
        data("c", num(3))
    });
    Computer computer3(4);
    computer3.boot(ooasm_data);
    whatis(memory_dump(computer3))
    assert(memory_dump(computer3) == "1 2 3 0 ");

    auto ooasm_operations = program({
        data("a", num(4)),
        data("b", num(3)),
        data("c", num(2)),
        data("d", num(1)),
        add(mem(lea("a")), mem(lea("c"))),
        sub(mem(lea("b")), mem(lea("d"))),
        mov(mem(lea("c")), num(0)),
        mov(mem(lea("d")), num(0))
    });
    Computer computer4(5);
    computer4.boot(ooasm_operations);
    whatis(memory_dump(computer4))
    assert(memory_dump(computer4) == "6 2 0 0 0 ");
}
