// Przerobiony test 302 do zadania TMPAsm

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
    auto ooasm_move_lea_1var = program({
            data("a", num(42)),
            mov(mem(num(1)), lea("a")),
            mov(mem(num(2)), mem(lea("a"))),
            mov(mem(lea("a")), num(5))
            });
    auto ooasm_move_lea_3vars_read = program({
            mov(mem(num(5)), mem(lea("a"))),
            mov(mem(num(6)), mem(lea("b"))),
            mov(mem(num(7)), mem(lea("c"))),
            data("a", num(7)),
            data("b", num(8)),
            data("c", num(9))
            });
    auto ooasm_move_lea_3vars_write = program({
            mov(mem(num(5)), mem(lea("a"))),
            data("a", num(7)),
            mov(mem(num(6)), mem(lea("b"))),
            data("b", num(8)),
            mov(mem(num(7)), mem(lea("c"))),
            mov(mem(lea("c")), num(13)),
            mov(mem(lea("b")), num(14)),
            data("c", num(9)),
            mov(mem(lea("a")), num(14)),
            mov(mem(lea("a")), num(15))
            });

    Computer computer1(3);
    computer1.boot(ooasm_move_lea_1var);
    assert(memory_dump(computer1) == "5 0 42 ");

    Computer computer2(8);
    computer2.boot(ooasm_move_lea_3vars_read);
    assert(memory_dump(computer2) == "7 8 9 0 0 7 8 9 ");

    Computer computer3(8);
    computer3.boot(ooasm_move_lea_3vars_write);
    assert(memory_dump(computer3) == "15 14 13 0 0 7 8 9 ");
}
