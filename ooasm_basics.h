#ifndef JNP1_6_OOASM_BASICS_H_
#define JNP1_6_OOASM_BASICS_H_

#include <cstdint>
#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <utility>
#include <vector>
#include <list>
#include <tuple>
#include <functional>
#include <type_traits>
#include <stdexcept>
#include <limits>
#include <iostream> // do uproszczenia

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

#endif //JNP1_6_OOASM_BASICS_H_
