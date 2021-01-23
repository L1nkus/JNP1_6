#ifndef JNP1_6_OOASM_H
#define JNP1_6_OOASM_H

#include "ooasm_instructions.h"
#include "ooasm_elements.h"

jnp1_6::Mov *mov(jnp1_6::Lvalue *dst, jnp1_6::Rvalue *src);

jnp1_6::Lvalue *mem(jnp1_6::Rvalue *addr);

jnp1_6::Lea *lea(const char *id);

jnp1_6::Inc *inc(jnp1_6::Lvalue *lvalue);

jnp1_6::Dec *dec(jnp1_6::Lvalue *lvalue);

jnp1_6::Add *add(jnp1_6::Lvalue *arg1, jnp1_6::Rvalue *arg2);

jnp1_6::Sub *sub(jnp1_6::Lvalue *arg1, jnp1_6::Rvalue *arg2);

jnp1_6::Data *data(const char *id, jnp1_6::Num *num);

jnp1_6::One *one(jnp1_6::Lvalue *arg);

jnp1_6::Onez *onez(jnp1_6::Lvalue *arg);

jnp1_6::Ones *ones(jnp1_6::Lvalue *arg);

jnp1_6::Num *num(jnp1_6::word_t value);

#endif // JNP1_6_OOASM_H
