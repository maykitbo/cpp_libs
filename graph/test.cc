#include "graph.h"

#include "../utility/sstr.h"
#include "../utility/time.h"
#include "../utility/random.h"

#include <vector>

#include <random>

using namespace s21;

int main() {
    int layers = 3;

    Graph<std::string, float> G(layers);

    for (int k = 0; k < layers; ++k) {
        for (int g = 0; g < (k + 1) * 2; ++g) {
            G.AddNode(Sstr(k, " ", g), k);
        }
    }

    for (int k = 0; k < layers - 1; ++k) {
        for (auto &source : G.GetNodes(k)) {
            for (auto &destination : G.GetNodes(k + 1)) {
                G.AddEdge(source, destination, Random::Normal(0, 1));
            }
        }
    }

    G.Print();
    G.Update();
    G.Print();

    return 0;
}
