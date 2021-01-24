#include "program.h"
#include <initializer_list>

jnp1_6::Program program(std::initializer_list<jnp1_6::InstructionPtr> init) {
    return jnp1_6::Program(init);
}
