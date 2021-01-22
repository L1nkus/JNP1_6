// Przerobiony test 223 do zadania TMPAsm

#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>

int main() {
    [[maybe_unused]] auto ooasm_data_num_num = program({
        data("a", num(num(1)))
    });
}
