// Przerobiony test 401 do zadania TMPAsm

#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>
#include <iostream>


namespace {
    std::string memory_dump(Computer const &computer) {
        std::stringstream ss;
        computer.memory_dump(ss);
        return ss.str();
    }
} // namespace

int main() {
    auto ooasm_add_inc = program({
            data("a", num(3)),
            data("b", num(3)),
            data("c", num(3)),
            data("d", num(3)),
            data("e", num(3)),
            add(mem(num(0)), num(2)),
            add(mem(num(1)), mem(num(4))),
            add(mem(num(2)), lea("e")),
            inc(mem(lea("e")))
            });
    auto ooasm_sub_dec = program({
            data("a", num(3)),
            data("b", num(3)),
            data("c", num(3)),
            data("d", num(3)),
            data("e", num(3)),
            sub(mem(num(0)), num(2)),
            sub(mem(num(1)), mem(num(4))),
            sub(mem(num(2)), lea("e")),
            dec(mem(lea("e")))
            });
    auto ooasm_mixed = program({
            data("a", num(3)),
            data("b", num(3)),
            data("c", num(3)),
            data("d", num(3)),
            data("e", num(3)),
            add(mem(num(0)), num(2)),
            sub(mem(num(1)), mem(num(4))),
            add(mem(num(2)), mem(num(4))),
            sub(mem(num(2)), lea("e")),
            inc(mem(lea("e"))),
            dec(mem(lea("e")))
            });
    auto ooasm_min_max = program({
            data("a", num(127)),
            data("b", num(-128)),
            data("c", num(255)),
            data("d", num(0)),
            data("e", num(100)),
            data("f", num(100)),
            add(mem(num(0)), num(1)),
            sub(mem(num(1)), num(1)),
            add(mem(num(2)), num(1)),
            sub(mem(num(3)), num(1)),
            add(mem(num(4)), num(29)),
            add(mem(num(5)), num(156))
            });

    Computer computer1(8);
    computer1.boot(ooasm_add_inc);
    assert(memory_dump(computer1) == "5 6 7 3 4 0 0 0 ");


    Computer computer2(8);
    computer2.boot(ooasm_sub_dec);
    assert(memory_dump(computer2) == "1 0 -1 3 2 0 0 0 ");

    Computer computer3(8);
    computer3.boot(ooasm_mixed);
    assert(memory_dump(computer3) == "5 0 2 3 3 0 0 0 ");

    Computer computer4(6);
    computer4.boot(ooasm_min_max);
    assert(memory_dump(computer4) == "128 -129 256 -1 129 256 ");
}
