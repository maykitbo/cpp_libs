#pragma once

#include <random>

namespace s21 {

namespace Random {

template<class Type, class Distribution, class Engine, class ...Args>
Type Full(Args &&...args) {
    static Engine generator{std::random_device{}()};
    static Distribution distribution{args...};
    return distribution(generator);
}

template<class Type = float, class Distribution = std::normal_distribution<Type>,class Engine = std::mt19937>
Type Normal(float mean, float sd) {
    return Full<Type, Distribution, Engine>(mean, sd);
}

template<class Distribution = std::bernoulli_distribution, class Engine = std::mt19937>
bool Bool(float chance) {
    return Full<bool, Distribution, Engine>(chance);
}

template<class Type = float, class Engine = std::mt19937>
Type Uniform(float min, float max) {
    return Full<Type, std::uniform_real_distribution<Type>, Engine>(min, max);
}

template<class Engine = std::mt19937>
int Int(int min, int max) {
    return Full<int, std::uniform_int_distribution<int>, Engine>(min, max);
}

} // namespace Random

} // namespace s21
