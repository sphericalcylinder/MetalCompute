#include "MTLCompute.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


MTL::Device *gpu = MTL::CreateSystemDefaultDevice();
MTLCompute::Texture1D<float> texture(gpu, 10, MTLCompute::TextureType::float32);
std::vector<float> data(10, 1.0);
std::vector<float> toomuch(11, 1.0);
std::vector<float> toolittle(9, 1.0);

TEST_CASE("Test Constructor") {
    REQUIRE(texture.getWidth() == 10);
    REQUIRE(texture.getGPU() == gpu);
    REQUIRE(texture.getTexture() != nullptr);
    REQUIRE(texture.getDescriptor() != nullptr);
    REQUIRE(texture.getFreed() == false);
}

TEST_CASE("Test copy constructor") {
    MTLCompute::Texture1D<float> other(texture);
    REQUIRE(other.getWidth() == texture.getWidth());
    REQUIRE(other.getGPU() == texture.getGPU());
    REQUIRE(other.getTexture() == texture.getTexture());
    REQUIRE(other.getDescriptor() == texture.getDescriptor());
    REQUIRE(other.getFreed() == texture.getFreed());
}

TEST_CASE("Test default constructor") {
    MTLCompute::Texture1D<float> other;
    REQUIRE(other.getWidth() == -1);
    REQUIRE(other.getGPU() == nullptr);
    REQUIRE(other.getTexture() == nullptr);
    REQUIRE(other.getDescriptor() == nullptr);
    REQUIRE(other.getFreed() == false);
}

TEST_CASE("Test set with vector") {
    REQUIRE_NOTHROW(texture = data);
    REQUIRE(texture.getWidth() == 10);
}

TEST_CASE("Test set with texture") {
    MTLCompute::Texture1D<float> other = texture;
    REQUIRE(other.getWidth() == texture.getWidth());
    REQUIRE(other.getGPU() == texture.getGPU());
    REQUIRE(other.getTexture() == texture.getTexture());
    REQUIRE(other.getDescriptor() == texture.getDescriptor());
}

TEST_CASE("Test get with vector") {
    texture = data;
    std::vector<float> result = texture.getData();
    for (int i = 0; i < texture.getWidth(); i++) {
        CHECK(result[i] == 1.0);
    }
}

TEST_CASE("Test get item with [] operator") {
    texture = data;
    for (int i = 0; i < texture.getWidth(); i++) {
        CHECK(texture[i] == data[i]);
    }
}

TEST_CASE("Test set with too much data") {
    REQUIRE_THROWS(texture = toomuch);
}

TEST_CASE("Test set with too little data") {
    REQUIRE_THROWS(texture = toolittle);
}

TEST_CASE("Test create texture larger than max size") {
    REQUIRE_THROWS_AS_MESSAGE(MTLCompute::Texture1D<float>(gpu, 16385, MTLCompute::TextureType::float32),
        std::invalid_argument, "Texture size too large, max size is 16384");
}