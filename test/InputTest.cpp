//
// Created by a on 8/5/20.
//
#include "gtest/gtest.h"
#include "Graph/Input/Structure.h"

TEST(InputTest, GraphRepr1) {

    std::string in_text = "1\n"
                          "(11,123,21,\"3asd\")\n";

    auto x = Structure();
    auto state = x.readData(std::string(in_text));

   ASSERT_EQ(Structure::graphState::E_FAIL, state);

};



