// Przerobiony test 225 do zadania TMPAsm

#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>

int main() {
    [[maybe_unused]] auto ooasm_data_mem = program({
        data("a", mem(num(0)))
    });
}
