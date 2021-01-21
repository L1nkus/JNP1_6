// Przerobiony test 226 do zadania TMPAsm

#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>
#include <exception>

namespace {
    std::string memory_dump(Computer const &computer) {
        std::stringstream ss;
        computer.memory_dump(ss);
        return ss.str();
    }
} // namespace

int main() {
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

    Computer computer1(9);

    try {
        computer1.boot(ooasm_10diffvars); // Should throw
    } catch(std::exception& e) {
        assert(memory_dump(computer1) == "1 2 3 4 5 6 7 8 9 ");

        return 0;
    }

    assert(false);
}
