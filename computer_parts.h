#ifndef JNP1_6_COMPUTER_PARTS_H_
#define JNP1_6_COMPUTER_PARTS_H_

#include "ooasm_types.h"
#include <ostream>
#include <algorithm>

namespace jnp1_6 {
class Processor {
  private:
    bool zf;
    bool sf;
  public:
    Processor() : zf(false), sf(false) {}

    bool get_zero_flag() const {
        return zf;
    }

    bool get_sign_flag() const {
        return sf;
    }

    void set_flags(word_t res) {
        zf = res == 0;
        sf = res < 0;
    }
};

class Memory {
  private:
    // Holds pair {variable name, memory address}.
    using variable_t = std::pair<std::string, size_t>;

    std::vector<variable_t> variables_register;
    std::vector<word_t> variables;

  public:
    Memory(unsigned_word_t mem_size) {
        variables.resize(mem_size, 0);
    }

    word_t value_at(unsigned_word_t pos) const {
        // Note: std::vector.at() throws std::out_of_range,
        // while operator[] does not.
        return variables.at(pos);
    }

    unsigned_word_t find_variable_address(const Id &id) const {
        // Because ids are short, it is faster than using an unordered map.
        auto it = std::lower_bound(variables_register.begin(),
                                   variables_register.end(),
                                   std::pair<std::string, size_t>{id.get_string(), 0});
        if (it == variables_register.end() || it->first != id.get_string()) {
            throw std::invalid_argument("Variable " + id.get_string() +
                " is not defined.");
        } else {
            return it->second;
        }
    }

    void add_new_variable(const Id &id, word_t value) {
        variables.at(variables_register.size()) = value;
        variables_register.emplace_back(id.get_string(), variables_register.size());
    }

    void set_value(unsigned_word_t pos, word_t value) {
        variables.at(pos) = value;
    }

    void finish_loading_variables() {
        std::sort(variables_register.begin(), variables_register.end());
    }

    void dump(std::ostream &os) const {
        for (auto const &num: variables)
            os << num << ' ';
    }
};
}
#endif //JNP1_6_COMPUTER_PARTS_H_