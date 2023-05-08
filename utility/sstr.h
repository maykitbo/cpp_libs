#pragma once

#include <sstream>
#include <iostream>
#include <iterator>
#include <type_traits>

namespace s21 {

namespace SStr {

template <const char Delimiter, class... Args >
std::string Fill(const Args &... args) {
  std::ostringstream sstr;
  int i = 0;
  ((sstr << std::dec << args << (++i == sizeof...(args) ? '\0' : Delimiter)), ...);
  return sstr.str();
}

template <class... Args >
std::string Fill(const Args &... args) {
  std::ostringstream sstr;
  (sstr << std::dec << ... << args);
  return sstr.str();
}

template <const char Delimiter = '\n', class... Args >
void Print(const Args &... args) {
  int i = 0;
  ((std::cout << std::dec << args << (++i == sizeof...(args) ? '\0' : Delimiter)), ...);
}

struct PrintStyle {
  inline static std::string container_delimiter = ", ";
  inline static std::string pair_delimiter = ", ";

  template<class T>
  static typename std::enable_if<std::is_same<T, std::string>::value, std::string>::type
  Wrapper(const T &l) {
    return "\"" + l + "\"";
  }

  template<class T>
  static typename std::enable_if<! std::is_same<T, std::string>::value, const T&>::type
  Wrapper(const T &l) {
    return l;
  }
};

template <class Container>
typename std::enable_if<
  std::is_same<
    decltype(std::begin(std::declval<Container>())),
    decltype(std::end(std::declval<Container>()))
  >::value &&
  !std::is_same<Container, std::string>::value,
  std::ostream&
>::type operator<<(std::ostream &os, const Container &c) {
  auto i = c.begin();
  if (i == c.end()) {
    return os;
  }
  os << '{';
  while (i != --c.end()) {
    os << PrintStyle::Wrapper(*(i++)) << PrintStyle::container_delimiter;
  }
  os << PrintStyle::Wrapper(*i) << "}\n";
  return os;
}

template<class A, class B>
std::ostream &operator<<(std::ostream &os, const std::pair<A, B> &p) {
  std::cout << '[' << p.first << PrintStyle::pair_delimiter << p.second << "]\n";
  return os;
}

template<class A>
std::ostream &operator<<(std::ostream &os, const std::pair<A, A> &p) {
  std::cout << '[' << p.first << PrintStyle::pair_delimiter << p.second << "]\n";
  return os;
}

} // namespace Str

} // namespace s21
