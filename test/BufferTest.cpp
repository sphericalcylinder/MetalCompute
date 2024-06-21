#include "MTLCompute.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


MTL::Device *gpu = MTL::CreateSystemDefaultDevice();
MTLCompute::Buffer<int> buffer(gpu, 10, MTLCompute::ResourceStorage::Shared);
vec<int> data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
vec<int> toomuch(11);
vec<int> toolittle(9);


TEST_CASE("Test Constructor") {
    REQUIRE(buffer.getLength() == 10);
    REQUIRE(buffer.getItemSize() == sizeof(int));
    REQUIRE(buffer.getGPU() == gpu);
    REQUIRE(buffer.getBuffer() != nullptr);
    REQUIRE(buffer.getStorageMode() == MTLCompute::ResourceStorage::Shared);
}

TEST_CASE("Test copy constructor") {
    MTLCompute::Buffer<int> other(buffer);
    REQUIRE(other.getLength() == buffer.getLength());
    REQUIRE(other.getItemSize() == buffer.getItemSize());
    REQUIRE(other.getGPU() == buffer.getGPU());
    REQUIRE(other.getBuffer() == buffer.getBuffer());
    REQUIRE(other.getStorageMode() == buffer.getStorageMode());
}

TEST_CASE("Test default constructor") {
    MTLCompute::Buffer<int> other;
    REQUIRE(other.getLength() == -1);
    REQUIRE(other.getItemSize() == -1);
    REQUIRE(other.getGPU() == nullptr);
    REQUIRE(other.getBuffer() == nullptr);
    REQUIRE(other.getStorageMode() == MTLCompute::ResourceStorage::Shared);
}

TEST_CASE("Test set with vector") {
    REQUIRE_NOTHROW(buffer = data);
}

TEST_CASE("Test set with buffer") {
    MTLCompute::Buffer<int> other = buffer;
    REQUIRE(other.getLength() == buffer.getLength());
    REQUIRE(other.getItemSize() == buffer.getItemSize());
    REQUIRE(other.getGPU() == buffer.getGPU());
    REQUIRE(other.getBuffer() == buffer.getBuffer());
    REQUIRE(other.getStorageMode() == buffer.getStorageMode());
}

TEST_CASE("Test get with [] operator") {
    buffer = data;
    for (int i = 0; i < buffer.getLength(); i++) {
        CHECK(buffer[i] == i);
    }
}

TEST_CASE("Test set with [] operator") {
    buffer = vec<int>(10);
    for (int i = 0; i < buffer.getLength(); i++) {
        CHECK_NOTHROW(buffer[i] = i);
    }
    for (int i = 0; i < buffer.getLength(); i++) {
        CHECK(buffer[i] == i);
    }
}

TEST_CASE("Test get with returned vector") {
    buffer = data;
    vec<int> result = buffer.getData();
    for (int i = 0; i < buffer.getLength(); i++) {
        CHECK(result[i] == i);
    }
}

TEST_CASE("Test out of bounds [] operator") {
    CHECK_THROWS(buffer[10]);
    CHECK_THROWS(buffer[-1]);
}

TEST_CASE("Test set with too much data") {
    CHECK_THROWS(buffer = toomuch);
}

