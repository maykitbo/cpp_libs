#pragma once

#include <chrono>

namespace s21 {

namespace Time {

typedef decltype((std::chrono::high_resolution_clock::now)()) T;

T Now() {
    return std::chrono::high_resolution_clock::now();
}

typedef std::chrono::hours h;
typedef std::chrono::minutes min;
typedef std::chrono::seconds sec;
typedef std::chrono::milliseconds ms;
typedef std::chrono::microseconds mcs;
typedef std::chrono::nanoseconds ns;

template<class Unit = ms>
int64_t Duration(T first, T second = Now()) {
    return std::chrono::duration_cast<Unit>(second - first).count();
}

} // namespace Time

} // namespace s21