#include "MTLCompute.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


std::string name = "default.metallib";
MTL::Device *gpu = MTL::CreateSystemDefaultDevice();
MTLCompute::Kernel kernel(gpu, name, "add_arrays");
MTLCompute::CommandManager<float> manager(gpu, &kernel);

TEST_CASE("Test constructor") {
    REQUIRE(manager.getGPU() == gpu);
    REQUIRE(manager.getKernel() == &kernel);
}

TEST_CASE("Test loadBuffer") {
    MTLCompute::Buffer<float> buffer(gpu, 10, MTLCompute::ResourceStorage::Shared);
    CHECK_NOTHROW(manager.loadBuffer(buffer, 0));
    CHECK(manager.getBuffers()[0].getData() == buffer.getData());
    manager.resetBuffers();
}

TEST_CASE("Test resetBuffers") {
    MTLCompute::Buffer<float> buffer(gpu, 10, MTLCompute::ResourceStorage::Shared);
    CHECK_NOTHROW(manager.loadBuffer(buffer, 0));
    manager.resetBuffers();
    CHECK_THROWS_AS_MESSAGE(manager.getBuffers()[0].getData(), std::runtime_error, "Buffer not initialized");
}

TEST_CASE("Test loadTexture") {
    MTLCompute::Texture2D<float> texture(gpu, 10, 10, MTLCompute::TextureType::float32);
    CHECK_NOTHROW(manager.loadTexture(texture, 0));
    CHECK(manager.getTextures()[0].getData() == texture.getData());
    manager.resetTextures();
}

TEST_CASE("Test resetTextures") {
    MTLCompute::Texture2D<float> texture(gpu, 10, 10, MTLCompute::TextureType::float32);
    CHECK_NOTHROW(manager.loadTexture(texture, 0));
    manager.resetTextures();
    CHECK_THROWS_AS_MESSAGE(manager.getTextures()[0].getData(), std::runtime_error, "Texture not initialized");
}

TEST_CASE("Test empty dispatch") {
    CHECK_THROWS(manager.dispatch());
}

TEST_CASE("Test dispatch") {
    MTLCompute::Buffer<float> bufferone(gpu, 10, MTLCompute::ResourceStorage::Shared);
    manager.loadBuffer(bufferone, 0);
    manager.dispatch();
    manager.resetBuffers();
}

TEST_CASE("Test double dispatch") {
    MTLCompute::Buffer<float> bufferone(gpu, 10, MTLCompute::ResourceStorage::Shared);
    manager.loadBuffer(bufferone, 0);
    manager.dispatch();
    CHECK_NOTHROW(manager.dispatch());
    manager.resetBuffers();
}

TEST_CASE("Test double load buffer on same index") {
    MTLCompute::Buffer<float> bufferone(gpu, 10, MTLCompute::ResourceStorage::Shared);
    MTLCompute::Buffer<float> buffertwo(gpu, 10, MTLCompute::ResourceStorage::Shared);
    CHECK_NOTHROW(manager.loadBuffer(bufferone, 0));
    CHECK_NOTHROW(manager.loadBuffer(buffertwo, 0));
    manager.resetBuffers();
}

TEST_CASE("Test inconsistent size loadBuffer") {
    MTLCompute::Buffer<float> bufferone(gpu, 10, MTLCompute::ResourceStorage::Shared);
    MTLCompute::Buffer<float> buffertwo(gpu, 11, MTLCompute::ResourceStorage::Shared);
    CHECK_NOTHROW(manager.loadBuffer(bufferone, 0));
    CHECK_THROWS(manager.loadBuffer(buffertwo, 1));
    manager.resetBuffers();
}

TEST_CASE("Test double load texture on same index") {
    MTLCompute::Texture2D<float> textureone(gpu, 10, 10, MTLCompute::TextureType::float32);
    MTLCompute::Texture2D<float> texturetwo(gpu, 10, 10, MTLCompute::TextureType::float32);
    CHECK_NOTHROW(manager.loadTexture(textureone, 0));
    CHECK_NOTHROW(manager.loadTexture(texturetwo, 0));
    manager.resetTextures();
}

TEST_CASE("Test inconsistent size loadTexture") {
    MTLCompute::Texture2D<float> textureone(gpu, 10, 10, MTLCompute::TextureType::float32);
    MTLCompute::Texture2D<float> texturetwo(gpu, 12, 12, MTLCompute::TextureType::float32);
    CHECK_NOTHROW(manager.loadTexture(textureone, 0));
    CHECK_THROWS(manager.loadTexture(texturetwo, 1));
    manager.resetTextures();
}