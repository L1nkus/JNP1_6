#include "computer.h"
#include "ooasm.h"
#include <cassert>
#include <sstream>
#include <string>
#include <exception>

int main() {
    try {
        auto ooasm_id_null = program({
                data(NULL, num(42))
        });
    } catch(std::exception& e) {
        return 0;
    }

    assert(false);
}
