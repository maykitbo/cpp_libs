#pragma once

#include <sstream>

namespace s21 {

namespace Str {

template <const char Delimetr, class... Args >
std::string Fill(Args &&... args) {
  std::ostringstream sstr;
  int i = 0;
  ((sstr << std::dec << args << (++i == sizeof...(args) ? '\0' : Delimetr)), ...);
  return sstr.str();
}

template <class... Args >
std::string Fill(Args &&... args) {
  std::ostringstream sstr;
  (sstr << std::dec << ... << args);
  return sstr.str();
}

} // namespace Str

} // namespace s21
