// Przerobiony test 304 do zadania TMPAsm

#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>

int main() {
    [[maybe_unused]] auto ooasm_move_lea_num = program({mov(lea("a"), num(2))});
}
