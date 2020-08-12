#include <iostream>

#include "Graph/Input/Structure.h"

int main() {

    // auto str = std::string("asd");
    // Structure().readData<std::string>(str);
    // std::cout << str << std::endl;

    std::string in_text = "1\n"
                          "a() b c (11,21, 3asd) (8,2, 3   )\n";

    auto x = Structure();//.readData("asdf");
    x.readData(std::string(in_text));

    return 0;
}
