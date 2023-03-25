#include <unordered_map>

#include <iostream>
#include <vector>
#include <list>

#include "../utility/time.h"

using namespace s21;

int main() {
    std::unordered_map<int, int> um;
    for (int k = 0; k < 5000000; ++k) {
        um.insert({k, 0});
    }
    std::vector<int> V(5000000);
    std::list<int> L(5000000);

    auto T = Time::Now();
    for (auto &i : um) {}
    std::cout << Time::Duration(T) << "\n";

    T = Time::Now();
    for (auto &i : V) {}
    std::cout << Time::Duration(T) << "\n";

    T = Time::Now();
    for (auto &i : L) {}
    std::cout << Time::Duration(T) << "\n";

    return 0;
}
