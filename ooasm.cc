#include "ooasm.h"

jnp1_6::Mov *mov(jnp1_6::Lvalue *dst, jnp1_6::Rvalue *src) {
    return new jnp1_6::Mov(dst, src);
}

jnp1_6::Lvalue *mem(jnp1_6::Rvalue *addr) {
    return new jnp1_6::Mem(addr);
}

jnp1_6::Lea *lea(const char *id) {
    return new jnp1_6::Lea(id);
}

jnp1_6::Inc *inc(jnp1_6::Lvalue *lvalue) {
    return new jnp1_6::Inc(lvalue);
}

jnp1_6::Dec *dec(jnp1_6::Lvalue *lvalue) {
    return new jnp1_6::Dec(lvalue);
}

jnp1_6::Add *add(jnp1_6::Lvalue *arg1, jnp1_6::Rvalue *arg2) {
    return new jnp1_6::Add(arg1, arg2);
}

jnp1_6::Sub *sub(jnp1_6::Lvalue *arg1, jnp1_6::Rvalue *arg2) {
    return new jnp1_6::Sub(arg1, arg2);
}

jnp1_6::Data *data(const char *id, jnp1_6::Num *num) {
    return new jnp1_6::Data(id, num);
}

jnp1_6::One *one(jnp1_6::Lvalue *arg) {
    return new jnp1_6::One(arg);
}

jnp1_6::Onez *onez(jnp1_6::Lvalue *arg) {
    return new jnp1_6::Onez(arg);
}

jnp1_6::Ones *ones(jnp1_6::Lvalue *arg) {
    return new jnp1_6::Ones(arg);
}

jnp1_6::Num *num(jnp1_6::word_t value) {
    return new jnp1_6::Num(value);
}
