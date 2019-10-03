#include <iostream>
#include <gtest/gtest.h>

#include "../src/headers/EssentMtrx.hpp"

TEST_P(EssentMtrx, CAMERA_TEST){

}

INSTANTIATE_TEST_CASE_P(InstantCamera, EssentMtrx, testing::Values());

int main(int argc, char const *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS;
}
