#include <iostream>
#include <ostream>
#include "Graph/Input/Structure.h"
#include "Dijkstra.h"
int main() {

    std::string in_text = "5\n"
                          "(1,2,1, \"1_to_2\")(2,3,5, \"2_to_3\")(3,4, 100, \"3_to_1\")(4,5, 1, \"3_to_1\")\n"
                          "(2,4,1000, \"2_to_4_2\")(4,5,1, \"4_to_5\")";
    auto x = Structure();
    x.readData(std::string(in_text));

    std::cout<<x<<std::endl;
    auto dij = Dijkstra(x);
    dij.compute(0,2);
    for (auto &x:dij.get_neigh()) {
        std::cout << x << std::endl;
    }
    return 0;
}
