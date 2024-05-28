#include "MTLCompute.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


MTL::Device *gpu = MTL::CreateSystemDefaultDevice();
MTLCompute::Texture<float> texture(gpu, 10, 10, MTLCompute::TextureType::float32);
std::vector<std::vector<float>> data(10, std::vector<float>(10, 1.0));
std::vector<std::vector<float>> toomuch(11, std::vector<float>(10, 1.0));
std::vector<std::vector<float>> toosmall(9, std::vector<float>(10, 1.0));

TEST_CASE("Test Constructor") {
    REQUIRE(texture.getWidth() == 10);
    REQUIRE(texture.getHeight() == 10);
    REQUIRE(texture.getGPU() == gpu);
    REQUIRE(texture.getTexture() != nullptr);
    REQUIRE(texture.getDescriptor() != nullptr);
    REQUIRE(texture.getFreed() == false);
}

TEST_CASE("Test copy constructor") {
    MTLCompute::Texture<float> other(texture);
    REQUIRE(other.getWidth() == texture.getWidth());
    REQUIRE(other.getHeight() == texture.getHeight());
    REQUIRE(other.getGPU() == texture.getGPU());
    REQUIRE(other.getTexture() == texture.getTexture());
    REQUIRE(other.getDescriptor() == texture.getDescriptor());
    REQUIRE(other.getFreed() == texture.getFreed());
}

TEST_CASE("Test default constructor") {
    MTLCompute::Texture<float> other;
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
}

TEST_CASE("Test set with texture") {
    MTLCompute::Texture<float> other = texture;
    REQUIRE(other.getWidth() == texture.getWidth());
    REQUIRE(other.getHeight() == texture.getHeight());
    REQUIRE(other.getGPU() == texture.getGPU());
    REQUIRE(other.getTexture() == texture.getTexture());
    REQUIRE(other.getDescriptor() == texture.getDescriptor());
}

TEST_CASE("Test get with vector") {
    texture = data;
    std::vector<std::vector<float>> result = texture.getData();
    for (int i = 0; i < texture.getWidth(); i++) {
        for (int j = 0; j < texture.getHeight(); j++) {
            CHECK(result[i][j] == 1.0);
        }
    }
}

TEST_CASE("Test set with too much data") {
    REQUIRE_THROWS(texture = toomuch);
}

TEST_CASE("Test set with too little data") {
    REQUIRE_THROWS(texture = toosmall);
}
