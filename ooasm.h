#ifndef JNP1_6_OOASM_H
#define JNP1_6_OOASM_H

#include "ooasm_instructions.h"
#include "ooasm_elements.h"

jnp1_6::NumPtr num(jnp1_6::word_t value);

jnp1_6::RvaluePtr lea(const char *id);

jnp1_6::LvaluePtr mem(jnp1_6::RvaluePtr &&addr);

jnp1_6::InstructionPtr inc(jnp1_6::LvaluePtr &&arg);

jnp1_6::InstructionPtr dec(jnp1_6::LvaluePtr &&arg);

jnp1_6::InstructionPtr add(jnp1_6::LvaluePtr &&arg1, jnp1_6::RvaluePtr &&arg2);

jnp1_6::InstructionPtr sub(jnp1_6::LvaluePtr &&arg1, jnp1_6::RvaluePtr &&arg2);

jnp1_6::InstructionPtr mov(jnp1_6::LvaluePtr &&dst, jnp1_6::RvaluePtr &&src);

jnp1_6::InstructionPtr data(const char *id, jnp1_6::NumPtr &&num);

jnp1_6::InstructionPtr one(jnp1_6::LvaluePtr &&arg);

jnp1_6::InstructionPtr onez(jnp1_6::LvaluePtr &&arg);

jnp1_6::InstructionPtr ones(jnp1_6::LvaluePtr &&arg);

#endif // JNP1_6_OOASM_H
