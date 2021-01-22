// Przerobiony test 303 do zadania TMPAsm

#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>

int main() {
    [[maybe_unused]] auto ooasm_move_num_num = program({mov(num(1), num(2))});
}
