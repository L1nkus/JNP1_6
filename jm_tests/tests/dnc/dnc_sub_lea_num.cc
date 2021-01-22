// Przerobiony test 405 do zadania TMPAsm

#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>

int main() {
    [[maybe_unused]] auto ooasm_sub_lea_num = program({sub(lea("a"), num(2))});
}
