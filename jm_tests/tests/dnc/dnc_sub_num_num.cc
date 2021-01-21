// Przerobiony test 404 do zadania TMPAsm

#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>

int main() {
    [[maybe_unused]] auto ooasm_sub_num_num = program({sub(num(1), num(2))});
}
