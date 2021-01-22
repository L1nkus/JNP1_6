// Przerobiony test 307 do zadania TMPAsm

#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>

int main() {
    [[maybe_unused]] auto ooasm_move_mem_lea_num = program({mov(mem(num(7)), lea(num(2)))});
}
