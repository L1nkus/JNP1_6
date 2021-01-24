#include "ooasm.h"

jnp1_6::NumPtr num(jnp1_6::word_t value) {
    return std::make_shared<jnp1_6::Num>(value);
}

jnp1_6::RvaluePtr lea(const char *id) {
    return std::make_shared<jnp1_6::Lea>(id);
}

jnp1_6::LvaluePtr mem(jnp1_6::RvaluePtr &&addr) {
    return std::make_shared<jnp1_6::Mem>(std::move(addr));
}

jnp1_6::InstructionPtr inc(jnp1_6::LvaluePtr &&arg) {
    return std::make_shared<jnp1_6::Inc>(std::move(arg));
}

jnp1_6::InstructionPtr dec(jnp1_6::LvaluePtr &&arg) {
    return std::make_shared<jnp1_6::Dec>(std::move(arg));
}

jnp1_6::InstructionPtr add(jnp1_6::LvaluePtr &&arg1, jnp1_6::RvaluePtr &&arg2) {
    return std::make_shared<jnp1_6::Add>(std::move(arg1), std::move(arg2));
}

jnp1_6::InstructionPtr sub(jnp1_6::LvaluePtr &&arg1, jnp1_6::RvaluePtr &&arg2) {
    return std::make_shared<jnp1_6::Sub>(std::move(arg1), std::move(arg2));
}

jnp1_6::InstructionPtr mov(jnp1_6::LvaluePtr &&dst, jnp1_6::RvaluePtr &&src) {
    return std::make_shared<jnp1_6::Mov>(std::move(dst), std::move(src));
}

jnp1_6::InstructionPtr data(const char *id, jnp1_6::NumPtr &&num) {
    return std::make_shared<jnp1_6::Data>(id, std::move(num));
}

jnp1_6::InstructionPtr one(jnp1_6::LvaluePtr &&arg) {
    return std::make_shared<jnp1_6::One>(std::move(arg));
}

jnp1_6::InstructionPtr onez(jnp1_6::LvaluePtr &&arg) {
    return std::make_shared<jnp1_6::Onez>(std::move(arg));
}

jnp1_6::InstructionPtr ones(jnp1_6::LvaluePtr &&arg) {
    return std::make_shared<jnp1_6::Ones>(std::move(std::move(arg)));
}
