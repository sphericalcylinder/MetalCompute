#include "MTLCompute.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


MTL::Device *gpu = MTL::CreateSystemDefaultDevice();
MTLCompute::Texture2D<float> texture(gpu, 10, 10);
vec2<float> data(10, vec<float>(10, 1.0));
vec2<float> toomuch(11, vec<float>(11, 1.0));
vec2<float> toolittle(9, vec<float>(9, 1.0));

TEST_CASE("Test Constructor") {
    REQUIRE(texture.getWidth() == 10);
    REQUIRE(texture.getHeight() == 10);
    REQUIRE(texture.getGPU() == gpu);
    REQUIRE(texture.getTexture() != nullptr);
    REQUIRE(texture.getDescriptor() != nullptr);
    REQUIRE(texture.getFreed() == false);
}

TEST_CASE("Test copy constructor") {
    MTLCompute::Texture2D<float> other(texture);
    REQUIRE(other.getWidth() == texture.getWidth());
    REQUIRE(other.getHeight() == texture.getHeight());
    REQUIRE(other.getGPU() == texture.getGPU());
    REQUIRE(other.getTexture() == texture.getTexture());
    REQUIRE(other.getDescriptor() == texture.getDescriptor());
    REQUIRE(other.getFreed() == texture.getFreed());
}

TEST_CASE("Test default constructor") {
    MTLCompute::Texture2D<float> other;
    REQUIRE(other.getWidth() == -1);
    REQUIRE(other.getHeight() == -1);
    REQUIRE(other.getGPU() == nullptr);
    REQUIRE(other.getTexture() == nullptr);
    REQUIRE(other.getDescriptor() == nullptr);
    REQUIRE(other.getFreed() == false);
}

TEST_CASE("Test set with vector") {
    REQUIRE_NOTHROW(texture = data);
    REQUIRE(texture.getWidth() == 10);
    REQUIRE(texture.getHeight() == 10);
    REQUIRE(texture.getData() == data);
}

TEST_CASE("Test set with texture") {
    MTLCompute::Texture2D<float> other = texture;
    REQUIRE(other.getWidth() == texture.getWidth());
    REQUIRE(other.getHeight() == texture.getHeight());
    REQUIRE(other.getGPU() == texture.getGPU());
    REQUIRE(other.getTexture() == texture.getTexture());
    REQUIRE(other.getDescriptor() == texture.getDescriptor());
}

TEST_CASE("Test get with vector") {
    texture = data;
    vec2<float> result = texture.getData();
    for (int i = 0; i < texture.getWidth(); i++) {
        for (int j = 0; j < texture.getHeight(); j++) {
            CHECK(result[i][j] == 1.0);
        }
    }
}

TEST_CASE("Test get row with [] operator") {
    texture = data;
    for (int i = 0; i < texture.getHeight(); i++) {
        CHECK(texture[i] == data[i]);
    }
}

TEST_CASE("Test get item with [] operator") {
    texture = data;
    for (int i = 0; i < texture.getHeight(); i++) {
        for (int j = 0; j < texture.getWidth(); j++) {
            CHECK(texture[i][j] == data[i][j]);
        }
    }
}

TEST_CASE("Test create texture larger than max size") {
    REQUIRE_THROWS_AS_MESSAGE(MTLCompute::Texture2D<float>(gpu, 16385, 16385),
        MTLCompute::TextureSizeError, "Texture size too large, max size is 16384");
}

TEST_CASE("Test set with too much data") {
    REQUIRE_THROWS_AS_MESSAGE(texture = toomuch, MTLCompute::TextureSizeError,
        "Data size does not match texture size");
}

TEST_CASE("Test set with too little data") {
    REQUIRE_THROWS_AS_MESSAGE(texture = toolittle, MTLCompute::TextureSizeError, 
        "Data size does not match texture size");
}

TEST_CASE("Test OOB get with [] operator") {
    texture = data;
    REQUIRE_THROWS_AS_MESSAGE(texture[10], MTLCompute::TextureIndexError,
        "Texture index out of bounds");
    REQUIRE_THROWS_AS_MESSAGE(texture[-1], MTLCompute::TextureIndexError,
        "Texture index out of bounds");
}

TEST_CASE("Test uninitialized get") {
    MTLCompute::Texture2D<float> other;
    REQUIRE_THROWS_AS_MESSAGE(other.getData(), MTLCompute::TextureInitError,
        "Texture not initialized");
    REQUIRE_THROWS_AS_MESSAGE(other[0], MTLCompute::TextureInitError,
        "Texture not initialized");
}