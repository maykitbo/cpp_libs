#pragma once

#include <sstream>

namespace s21 {

template < typename... Args >
std::string Sstr(Args &&... args) {
    std::ostringstream sstr;
    (sstr << std::dec << ... << args);
    return sstr.str();
}

} // namespace s21
