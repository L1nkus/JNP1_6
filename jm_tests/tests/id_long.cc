// Przerobiony test 205 do zadania TMPAsm

#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>
#include <exception>

int main() {
    try {
        auto ooasm_id_long = program({
                data("abcdefghijk", num(42))
        });
    } catch(std::exception& e) {
        return 0;
    }

    assert(false);
}
