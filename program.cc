#include "program.h"
#include <initializer_list>

jnp1_6::Program program(const std::initializer_list<jnp1_6::InstructionPtr> &&init) {
    return jnp1_6::Program(std::move(init));
}
