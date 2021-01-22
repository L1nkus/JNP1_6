// Przerobiony test 402 do zadania TMPAsm

#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>

int main() {
    [[maybe_unused]] auto ooasm_add_num_num = program({add(num(1), num(2))});
}
