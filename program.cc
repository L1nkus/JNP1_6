#include "program.h"

inline jnp1_6::Program program(const std::initializer_list<jnp1_6::Instruction *> &init) {
    std::vector<std::shared_ptr<jnp1_6::Instruction>> vec;
    vec.reserve(init.size());
    for (auto &i: init) {
        vec.push_back(std::shared_ptr<jnp1_6::Instruction>(i));
    }
    return jnp1_6::Program(std::move(vec));
}