// Przerobiony test 308 do zadania TMPAsm

#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>

int main() {
    [[maybe_unused]] auto ooasm_move_mem_7_num = program({mov(mem(7), num(2))});
}
