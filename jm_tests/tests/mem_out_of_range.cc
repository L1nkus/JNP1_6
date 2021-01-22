// Przerobiony test 309 do zadania TMPAsm

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
    auto ooasm_mem_out_of_range = program({mov(mem(num(100)), num(2))});

    Computer computer1(11);
    try {
        computer1.boot(ooasm_mem_out_of_range); // Should throw
    } catch(std::exception& e) {
        assert(memory_dump(computer1) == "0 0 0 0 0 0 0 0 0 0 0 ");

        return 0;
    }

    assert(false);
}
