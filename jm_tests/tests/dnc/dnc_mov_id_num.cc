// Przerobiony test 305 do zadania TMPAsm

#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>

int main() {
    [[maybe_unused]] auto ooasm_move_id_num = program({mov("a", num(2))});
}
