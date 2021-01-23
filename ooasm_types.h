#ifndef JNP1_6_OOASM_BASICS_H
#define JNP1_6_OOASM_BASICS_H

#include <memory>
#include <stdexcept>


namespace jnp1_6 {
using word_t = int64_t;
using unsigned_word_t = std::make_unsigned_t<word_t>;

class Id {
  private:
    const std::string id;
  public:
    Id(const char *id) : id(id) {
        if (this->id.empty() || this->id.size() > 10) {
            throw std::invalid_argument("Invalid id length");
        }
    }

    const std::string &get_string() const {
        return id;
    }
};
}

#endif // JNP1_6_OOASM_BASICS_H
