#include "MTLComputeGPU.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

/**
 * @brief A function that returns true (for testing purposes)
 *
 * @return bool
 *
*/
bool returntrue() {
    return true;
}

/**
 * @brief A function that returns x - 1
 *
 * @param x
 * @return int
 *
*/
int minusone(int x) {
    return x - 1;
}

TEST_CASE("Test returntrue") {
    CHECK(returntrue());
    CHECK(minusone(10) == 9);
    CHECK(minusone(0) == -1);
    CHECK_FALSE(minusone(0) == 0);
}

TEST_CASE("Test numComponents") {
    CHECK(MTLCompute::numComponents<MTLCompute::uchar2>() == 2);
    CHECK(MTLCompute::numComponents<MTLCompute::ushort2>() == 2);
    CHECK(MTLCompute::numComponents<MTLCompute::uint2>() == 2);
    CHECK(MTLCompute::numComponents<MTLCompute::char2>() == 2);
    CHECK(MTLCompute::numComponents<MTLCompute::short2>() == 2);
    CHECK(MTLCompute::numComponents<MTLCompute::int2>() == 2);
    CHECK(MTLCompute::numComponents<MTLCompute::float2>() == 2);

    CHECK(MTLCompute::numComponents<MTLCompute::uchar4>() == 4);
    CHECK(MTLCompute::numComponents<MTLCompute::ushort4>() == 4);
    CHECK(MTLCompute::numComponents<MTLCompute::uint4>() == 4);
    CHECK(MTLCompute::numComponents<MTLCompute::char4>() == 4);
    CHECK(MTLCompute::numComponents<MTLCompute::short4>() == 4);
    CHECK(MTLCompute::numComponents<MTLCompute::int4>() == 4);
    CHECK(MTLCompute::numComponents<MTLCompute::float4>() == 4);

    CHECK(MTLCompute::numComponents<MTLCompute::uchar>() == 1);
}