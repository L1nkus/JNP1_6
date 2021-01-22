// Przerobiony test 301 do zadania TMPAsm

#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>

namespace {
    std::string memory_dump(Computer const &computer) {
        std::stringstream ss;
        computer.memory_dump(ss);
        return ss.str();
    }
} // namespace

int main() {
    auto ooasm_move_mem = program({
            mov(mem(num(1)), num(2)),
            mov(mem(num(2)), num(9)),
            mov(mem(num(3)), mem(num(1))),
            mov(mem(num(4)), mem(num(2))),
            });


    auto ooasm_move_mem_mem = program({
            mov(mem(num(0)), num(1)),
            mov(mem(num(1)), num(2)),
            mov(mem(num(2)), num(9)),
            mov(mem(num(3)), mem(num(1))),
            mov(mem(num(4)), mem(num(2))),
            mov(mem(num(5)), mem(mem(num(0)))),
            mov(mem(mem(num(0))), num(42))
            });

    Computer computer1(8);
    computer1.boot(ooasm_move_mem);
    assert(memory_dump(computer1) == "0 2 9 2 9 0 0 0 ");

    Computer computer2(8);
    computer2.boot(ooasm_move_mem_mem);
    assert(memory_dump(computer2) == "1 42 9 2 9 2 0 0 ");
}
