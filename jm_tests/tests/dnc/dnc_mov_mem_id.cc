// Przerobiony test 306 do zadania TMPAsm

#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>

int main() {
    auto ooasm_move_mem_id = program({mov(mem(num(7)), "a")});
}
