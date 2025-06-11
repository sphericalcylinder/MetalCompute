#include "MTLCompute.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

/**
 * @brief A function that returns true (for testing purposes)
 *
 * @return bool
 *
 */
bool returntrue() { return true; }

/**
 * @brief A function that returns x - 1
 *
 * @param x
 * @return int
 *
 */
int minusone(int x) { return x - 1; }

TEST_CASE("Test returntrue") {
    CHECK(returntrue());
    CHECK(minusone(10) == 9);
    CHECK(minusone(0) == -1);
    CHECK_FALSE(minusone(0) == 0);
}

TEST_CASE("Test numComponents") {

    using namespace MTLCompute;

    CHECK(numComponents<uchar2>() == 2);
    CHECK(numComponents<ushort2>() == 2);
    CHECK(numComponents<uint2>() == 2);
    CHECK(numComponents<char2>() == 2);
    CHECK(numComponents<short2>() == 2);
    CHECK(numComponents<int2>() == 2);
    CHECK(numComponents<float2>() == 2);

    CHECK(numComponents<uchar4>() == 4);
    CHECK(numComponents<ushort4>() == 4);
    CHECK(numComponents<uint4>() == 4);
    CHECK(numComponents<char4>() == 4);
    CHECK(numComponents<short4>() == 4);
    CHECK(numComponents<int4>() == 4);
    CHECK(numComponents<float4>() == 4);

    CHECK(numComponents<uchar>() == 1);
}

TEST_CASE("Test demangleTypeName") {

    using namespace MTLCompute;

    struct imaginary_type {};

    CHECK(strcmp(demangleTypeName(typeid(uchar).name()), "uchar") == 0);
    CHECK(strcmp(demangleTypeName(typeid(ushort).name()), "ushort") == 0);
    CHECK(strcmp(demangleTypeName(typeid(uint).name()), "uint") == 0);
    CHECK(strcmp(demangleTypeName(typeid(char).name()), "char") == 0);
    CHECK(strcmp(demangleTypeName(typeid(short).name()), "short") == 0);
    CHECK(strcmp(demangleTypeName(typeid(int).name()), "int") == 0);
    CHECK(strcmp(demangleTypeName(typeid(float).name()), "float") == 0);

    CHECK(strcmp(demangleTypeName(typeid(uchar2).name()), "uchar2") == 0);
    CHECK(strcmp(demangleTypeName(typeid(ushort2).name()), "ushort2") == 0);
    CHECK(strcmp(demangleTypeName(typeid(uint2).name()), "uint2") == 0);
    CHECK(strcmp(demangleTypeName(typeid(char2).name()), "char2") == 0);
    CHECK(strcmp(demangleTypeName(typeid(short2).name()), "short2") == 0);
    CHECK(strcmp(demangleTypeName(typeid(int2).name()), "int2") == 0);
    CHECK(strcmp(demangleTypeName(typeid(float2).name()), "float2") == 0);

    CHECK(strcmp(demangleTypeName(typeid(uchar4).name()), "uchar4") == 0);
    CHECK(strcmp(demangleTypeName(typeid(ushort4).name()), "ushort4") == 0);
    CHECK(strcmp(demangleTypeName(typeid(uint4).name()), "uint4") == 0);
    CHECK(strcmp(demangleTypeName(typeid(char4).name()), "char4") == 0);
    CHECK(strcmp(demangleTypeName(typeid(short4).name()), "short4") == 0);
    CHECK(strcmp(demangleTypeName(typeid(int4).name()), "int4") == 0);
    CHECK(strcmp(demangleTypeName(typeid(float4).name()), "float4") == 0);
    CHECK(strcmp(demangleTypeName(typeid(imaginary_type).name()), "unknown") == 0);

}