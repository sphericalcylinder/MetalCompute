#include "MTLCompute.hpp"
#include <vector>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


MTL::Device *gpu = MTL::CreateSystemDefaultDevice();
MTLCompute::Buffer<int> buffer(10, gpu, MTLCompute::ResourceStorage::Shared);
std::vector<int> data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
std::vector<int> toomuch(11);


TEST_CASE("Test Constructor") {
    REQUIRE(buffer.length == 10);
    REQUIRE(buffer.itemsize == sizeof(int));
}

TEST_CASE("Test set with vector") {
    REQUIRE_NOTHROW(buffer = data);
}

TEST_CASE("Test get with [] operator") {
    buffer = data;
    for (int i = 0; i < buffer.length; i++) {
        CHECK(buffer[i] == i);
    }
}

TEST_CASE("Test set with [] operator") {
    buffer = std::vector<int>(10);
    for (int i = 0; i < buffer.length; i++) {
        CHECK_NOTHROW(buffer[i] = i);
    }
    for (int i = 0; i < buffer.length; i++) {
        CHECK(buffer[i] == i);
    }
}

TEST_CASE("Test get with returned vector") {
    buffer = data;
    std::vector<int> result = buffer.getData();
    for (int i = 0; i < buffer.length; i++) {
        CHECK(result[i] == i);
    }
}

TEST_CASE("Test out of bounds [] operator") {
    CHECK_THROWS(buffer[11]);
    CHECK_THROWS(buffer[-1]);
}

TEST_CASE("Test set with too much data") {
    CHECK_THROWS(buffer = toomuch);
}

TEST_CASE("Test Buffer Destructor") {
    REQUIRE_NOTHROW(buffer.~Buffer());
}

TEST_CASE("Double free") {
    REQUIRE_NOTHROW(buffer.~Buffer());
}

TEST_CASE("Test freed buffer access") {
    buffer.~Buffer();
    CHECK_THROWS(buffer[0]);
    CHECK_THROWS(buffer = data);
}
