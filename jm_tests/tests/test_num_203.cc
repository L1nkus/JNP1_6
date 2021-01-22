// Przerobiony test 203 do zadania TMPAsm

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
    auto ooasm_1var = program({
        data("a", num(42))
    });
    
    auto ooasm_10diffvars = program({
            data("a1", num(1)),
            data("a2", num(2)),
            data("a3", num(3)),
            data("a4", num(4)),
            data("a5", num(5)),
            data("a6", num(6)),
            data("a7", num(7)),
            data("a8", num(8)),
            data("a9", num(9)),
            data("a10", num(10))
            });

    auto ooasm_10dsamevars = program({
            data("a", num(42)),
            data("a", num(42)),
            data("a", num(42)),
            data("a", num(42)),
            data("a", num(42)),
            data("a", num(42)),
            data("a", num(42)),
            data("a", num(42)),
            data("a", num(42)),
            data("a", num(42)),
            });

    Computer computer1(11);
    computer1.boot(ooasm_1var);
    assert(memory_dump(computer1) == "42 0 0 0 0 0 0 0 0 0 0 ");


    Computer computer2(11);
    computer2.boot(ooasm_10diffvars);
    assert(memory_dump(computer2) == "1 2 3 4 5 6 7 8 9 10 0 ");

    Computer computer3(11);
    computer3.boot(ooasm_10dsamevars);
    assert(memory_dump(computer3) == "42 42 42 42 42 42 42 42 42 42 0 ");

    Computer computer4(1);
    computer4.boot(ooasm_1var);
    assert(memory_dump(computer4) == "42 ");

    Computer computer5(10);
    computer5.boot(ooasm_10dsamevars);
    assert(memory_dump(computer5) == "42 42 42 42 42 42 42 42 42 42 ");
}
