// Przerobiony test 204 do zadania TMPAsm

#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>
#include <exception>

int main() {
    try {
        auto ooasm_id_empty = program({
                data("", num(42))
        });
    } catch(std::exception& e) {
        return 0;
    }

    assert(false);
}
