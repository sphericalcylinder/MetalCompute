#include "MTLComputeBuffer.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


TEST_CASE("Test Buffer") {
    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();
    MTLCompute::Buffer<int> buffer(10, gpu, MTLCompute::ResourceStorage::Shared);
    std::vector<int> data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    REQUIRE(buffer.length == 10);
    REQUIRE(buffer.itemsize == sizeof(int));


    SUBCASE("Test contents set with vector") {
        REQUIRE_NOTHROW(buffer = data);
    }

    buffer = data;

    SUBCASE("Test contents get with [] operator") {
        for (int i = 0; i < buffer.length; i++) {
            REQUIRE(buffer[i] == i);
        }
    }

    SUBCASE("Test out of bounds [] operator") {
        REQUIRE_THROWS(buffer[11]);
        REQUIRE_THROWS(buffer[-1]);
    }

}
