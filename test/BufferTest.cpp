#include "MTLCompute.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

template <MTLCompute::ResourceStorage StorageMode> void BufferTests() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();
    MTLCompute::Buffer<int> buffer(gpu, 10, StorageMode);
    vec<int> data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    vec<int> toomuch(11);
    vec<int> toolittle(9);

    SUBCASE("Test Constructor") {
        REQUIRE(buffer.getLength() == 10);
        REQUIRE(buffer.getItemSize() == sizeof(int));
        REQUIRE(buffer.getGPU() == gpu);
        REQUIRE(buffer.getBuffer() != nullptr);
        REQUIRE(buffer.getStorageMode() == StorageMode);
    }

    SUBCASE("Test copy constructor") {
        MTLCompute::Buffer<int> other(buffer);
        REQUIRE(other.getLength() == buffer.getLength());
        REQUIRE(other.getItemSize() == buffer.getItemSize());
        REQUIRE(other.getGPU() == buffer.getGPU());
        REQUIRE(other.getBuffer() == buffer.getBuffer());
        REQUIRE(other.getStorageMode() == buffer.getStorageMode());
    }

    SUBCASE("Test default constructor") {
        MTLCompute::Buffer<int> other;
        REQUIRE(other.getLength() == -1);
        REQUIRE(other.getItemSize() == -1);
        REQUIRE(other.getGPU() == nullptr);
        REQUIRE(other.getBuffer() == nullptr);
        REQUIRE(other.getStorageMode() == MTLCompute::ResourceStorage::Shared);
    }

    SUBCASE("Test OOB access") {
        MTLCompute::Buffer<int> freedbuffer(gpu, 10, StorageMode);
        freedbuffer.free();
        REQUIRE_THROWS_AS_MESSAGE(freedbuffer.getData(),
                                  MTLCompute::Error::BufferFreeError,
                                  "Buffer already freed");
        REQUIRE(freedbuffer.getFreed() == true);
    }

    SUBCASE("Test set with vector") { REQUIRE_NOTHROW(buffer = data); }

    SUBCASE("Test set with buffer") {
        MTLCompute::Buffer<int> other = buffer;
        REQUIRE(other.getLength() == buffer.getLength());
        REQUIRE(other.getItemSize() == buffer.getItemSize());
        REQUIRE(other.getGPU() == buffer.getGPU());
        REQUIRE(other.getBuffer() == buffer.getBuffer());
        REQUIRE(other.getStorageMode() == buffer.getStorageMode());
    }

    SUBCASE("Test get with [] operator") {
        buffer = data;
        for (int i = 0; i < buffer.getLength(); i++) {
            CHECK(buffer[i] == i);
        }
    }

    SUBCASE("Test set with [] operator") {
        buffer = vec<int>(10);
        for (int i = 0; i < buffer.getLength(); i++) {
            CHECK_NOTHROW(buffer[i] = i);
        }
        for (int i = 0; i < buffer.getLength(); i++) {
            CHECK(buffer[i] == i);
        }
    }

    SUBCASE("Test get with returned vector") {
        buffer = data;
        vec<int> result = buffer.getData();
        for (int i = 0; i < buffer.getLength(); i++) {
            CHECK(result[i] == i);
        }
    }

    SUBCASE("Test out of bounds [] operator") {
        CHECK_THROWS_AS_MESSAGE(buffer[10], MTLCompute::Error::BufferIndexError,
                                "Index out of bounds");
        CHECK_THROWS_AS_MESSAGE(buffer[-1], MTLCompute::Error::BufferIndexError,
                                "Index out of bounds");
    }

    SUBCASE("Test set with too much data") {
        CHECK_THROWS_AS_MESSAGE(buffer = toomuch,
                                MTLCompute::Error::BufferSizeError,
                                "Data size does not match buffer size");
    }

    SUBCASE("Test set with too little data") {
        CHECK_THROWS_AS_MESSAGE(buffer = toolittle,
                                MTLCompute::Error::BufferSizeError,
                                "Data size does not match buffer size");
    }
}

TEST_CASE("Test Buffer with Shared Storage") {
    BufferTests<MTLCompute::ResourceStorage::Shared>();
}
TEST_CASE("Test Buffer with Managed Storage") {
    BufferTests<MTLCompute::ResourceStorage::Managed>();
}