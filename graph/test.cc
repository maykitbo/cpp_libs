#include <unordered_map>

#include <iostream>
#include <vector>
#include <list>

#include "../utility/time.h"
#include "../utility/str.h"

#include "graph.h"

using namespace s21;

int main() {

    DirectedGraph<std::string, int> DG(3);
    for (int k = 0; k < 3; ++k) {
        for (int g = k; g < 4; ++g) {
            DG.AddNode(Str::Fill<' '>("Di Node", k, g), k);
            try {
                DG.AddEdge(Str::Fill<' '>("Di Node", k, g), Str::Fill<' '>("Di Node", k - 1, g - 1), k);
            } catch(...) {}
        }
    }

    UndirectedGraph<std::string ,int> UDG(3);
    for (int k = 0; k < 3; ++k) {
        for (int g = k; g < 4; ++g) {
            UDG.AddNode(Str::Fill<' '>("Di Node", k, g), k);
            try {
                UDG.AddEdge(Str::Fill<' '>("Di Node", k, g), Str::Fill<' '>("Di Node", k - 1, g - 1), k);
            } catch(...) {}
        }
    }

    // DG.Print();
    // UDG.Print();
    return 0;
}
