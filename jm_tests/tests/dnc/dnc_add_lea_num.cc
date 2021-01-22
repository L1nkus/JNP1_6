// Przerobiony test 403 do zadania TMPAsm

#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>

int main() {
    [[maybe_unused]] auto ooasm_add_lea_num = program({add(lea("a"), num(2))});
}
