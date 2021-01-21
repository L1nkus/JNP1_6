// Przerobiony test 224 do zadania TMPAsm
#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>

int main() {
    [[maybe_unused]] auto ooasm_data_lea = program({
        data("a", lea("a"))
    });
}
