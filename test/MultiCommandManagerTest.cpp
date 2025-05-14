#include "MTLCompute.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// Testing MTLCompute::CommandManager with multiple types

std::string name = "default.metallib";
MTL::Device *gpu = MTL::CreateSystemDefaultDevice();
MTLCompute::Kernel kernel(gpu, name, "add_arrays");
MTLCompute::CommandManager<int, float, char> manager(gpu, &kernel);

TEST_CASE("Test constructor") {
    REQUIRE(manager.getGPU() == gpu);
    REQUIRE(manager.getKernel() == &kernel);
}

TEST_CASE("Test resetBuffers") {
    MTLCompute::Buffer<float> bufferone(gpu, 10, MTLCompute::ResourceStorage::Shared);
    MTLCompute::Buffer<int> buffertwo(gpu, 10, MTLCompute::ResourceStorage::Shared);
    REQUIRE_NOTHROW(manager.loadBuffer(bufferone, 0));
    REQUIRE_NOTHROW(manager.loadBuffer(buffertwo, 1));
    manager.resetBuffers();
    REQUIRE_THROWS_AS_MESSAGE(manager.getBuffers<float>()[0].getData(), std::runtime_error, "Buffer not initialized");
    REQUIRE_THROWS_AS_MESSAGE(manager.getBuffers<float>()[1].getData(), std::runtime_error, "Buffer not initialized");
}

TEST_CASE("Test two type loadBuffer") {
    MTLCompute::Buffer<float> bufferone(gpu, 10, MTLCompute::ResourceStorage::Shared);
    MTLCompute::Buffer<int> buffertwo(gpu, 10, MTLCompute::ResourceStorage::Shared);
    CHECK_NOTHROW(manager.loadBuffer(bufferone, 0));
    CHECK_NOTHROW(manager.loadBuffer(buffertwo, 1));
    CHECK(manager.getBuffers<float>()[0].getData() == bufferone.getData());
    CHECK(manager.getBuffers<int>()[1].getData() == buffertwo.getData());
    manager.resetBuffers();
}

TEST_CASE("Test resetTextures") {
    MTLCompute::Texture2D<float> textureone(gpu, 10, 10);
    MTLCompute::Texture2D<int> texturetwo(gpu, 10, 10);
    REQUIRE_NOTHROW(manager.loadTexture(textureone, 0));
    REQUIRE_NOTHROW(manager.loadTexture(texturetwo, 1));
    manager.resetTextures();
    REQUIRE_THROWS(manager.getTexture2D<float>(0).getData());
    REQUIRE_THROWS(manager.getTexture2D<int>(1).getData());
}

TEST_CASE("Test two type 1D loadTexture") {
    MTLCompute::Texture1D<float> textureone(gpu, 10);
    MTLCompute::Texture1D<int> texturetwo(gpu, 10);
    CHECK_NOTHROW(manager.loadTexture(textureone, 0));
    CHECK_NOTHROW(manager.loadTexture(texturetwo, 1));
    CHECK(manager.getTexture1D<float>(0).getData() == textureone.getData());
    CHECK(manager.getTexture1D<int>(1).getData() == texturetwo.getData());
    manager.resetTextures();
}

TEST_CASE("Test two type 2D loadTexture") {
    MTLCompute::Texture2D<float> textureone(gpu, 10, 10);
    MTLCompute::Texture2D<int> texturetwo(gpu, 10, 10);
    CHECK_NOTHROW(manager.loadTexture(textureone, 0));
    CHECK_NOTHROW(manager.loadTexture(texturetwo, 1));
    CHECK(manager.getTexture2D<float>(0).getData() == textureone.getData());
    CHECK(manager.getTexture2D<int>(1).getData() == texturetwo.getData());
    manager.resetTextures();
}

TEST_CASE("Test two type 3D loadTexture") {
    MTLCompute::Texture3D<float> textureone(gpu, 10, 10, 10);
    MTLCompute::Texture3D<int> texturetwo(gpu, 10, 10, 10);
    CHECK_NOTHROW(manager.loadTexture(textureone, 0));
    CHECK_NOTHROW(manager.loadTexture(texturetwo, 1));
    CHECK(manager.getTexture3D<float>(0).getData() == textureone.getData());
    CHECK(manager.getTexture3D<int>(1).getData() == texturetwo.getData());
    manager.resetTextures();
}

TEST_CASE("Test empty dispatch") {
    CHECK_THROWS(manager.dispatch());
}

TEST_CASE("Test two type dispatch") {
    MTLCompute::Buffer<float> bufferone(gpu, 10, MTLCompute::ResourceStorage::Shared);
    MTLCompute::Buffer<int> buffertwo(gpu, 10, MTLCompute::ResourceStorage::Shared);
    manager.loadBuffer(bufferone, 0);
    manager.loadBuffer(buffertwo, 1);
    manager.dispatch();
    manager.resetBuffers();
}

TEST_CASE("Test two type double dispatch") {
    MTLCompute::Buffer<float> bufferone(gpu, 10, MTLCompute::ResourceStorage::Shared);
    MTLCompute::Buffer<int> buffertwo(gpu, 10, MTLCompute::ResourceStorage::Shared);
    manager.loadBuffer(bufferone, 0);
    manager.loadBuffer(buffertwo, 1);
    manager.dispatch();
    CHECK_NOTHROW(manager.dispatch());
    manager.resetBuffers();
}

TEST_CASE("Test double load two type buffers on same index") {
    MTLCompute::Buffer<float> bufferone(gpu, 10, MTLCompute::ResourceStorage::Shared);
    MTLCompute::Buffer<int> buffertwo(gpu, 10, MTLCompute::ResourceStorage::Shared);
    CHECK_NOTHROW(manager.loadBuffer(bufferone, 0));
    CHECK_NOTHROW(manager.loadBuffer(buffertwo, 0));
    manager.resetBuffers();
}

TEST_CASE("Test inconsistent size and two type loadBuffer") {
    MTLCompute::Buffer<float> bufferone(gpu, 10, MTLCompute::ResourceStorage::Shared);
    MTLCompute::Buffer<int> buffertwo(gpu, 11, MTLCompute::ResourceStorage::Shared);
    CHECK_NOTHROW(manager.loadBuffer(bufferone, 0));
    CHECK_THROWS(manager.loadBuffer(buffertwo, 1));
    manager.resetBuffers();
}

TEST_CASE("Test double load two type 1D textures on same index") {
    MTLCompute::Texture1D<float> textureone(gpu, 10);
    MTLCompute::Texture1D<int> texturetwo(gpu, 10);
    CHECK_NOTHROW(manager.loadTexture(textureone, 0));
    CHECK_NOTHROW(manager.loadTexture(texturetwo, 0));
    manager.resetTextures();
}

TEST_CASE("Test double load two type 2D textures on same index") {
    MTLCompute::Texture2D<float> textureone(gpu, 10, 10);
    MTLCompute::Texture2D<int> texturetwo(gpu, 10, 10);
    CHECK_NOTHROW(manager.loadTexture(textureone, 0));
    CHECK_NOTHROW(manager.loadTexture(texturetwo, 0));
    manager.resetTextures();
}

TEST_CASE("Test double load two type 3D textures on same index") {
    MTLCompute::Texture3D<float> textureone(gpu, 10, 10, 10);
    MTLCompute::Texture3D<int> texturetwo(gpu, 10, 10, 10);
    CHECK_NOTHROW(manager.loadTexture(textureone, 0));
    CHECK_NOTHROW(manager.loadTexture(texturetwo, 0));
    manager.resetTextures();
}

TEST_CASE("Test inconsistent size two type 1D loadTextures") {
    MTLCompute::Texture1D<float> textureone(gpu, 10);
    MTLCompute::Texture1D<int> texturetwo(gpu, 12);
    CHECK_NOTHROW(manager.loadTexture(textureone, 0));
    CHECK_THROWS(manager.loadTexture(texturetwo, 1));
    manager.resetTextures();
}

TEST_CASE("Test inconsistent size two type 2D loadTextures") {
    MTLCompute::Texture2D<float> textureone(gpu, 10, 10);
    MTLCompute::Texture2D<int> texturetwo(gpu, 12, 12);
    CHECK_NOTHROW(manager.loadTexture(textureone, 0));
    CHECK_THROWS(manager.loadTexture(texturetwo, 1));
    manager.resetTextures();
}

TEST_CASE("Test inconsistent size two type 3D loadTextures") {
    MTLCompute::Texture3D<float> textureone(gpu, 10, 10, 10);
    MTLCompute::Texture3D<int> texturetwo(gpu, 12, 12, 12);
    CHECK_NOTHROW(manager.loadTexture(textureone, 0));
    CHECK_THROWS(manager.loadTexture(texturetwo, 1));
    manager.resetTextures();
}

TEST_CASE("Test inconsistent size three type mixed loadTextures") {
    MTLCompute::Texture1D<float> textureone(gpu, 10);
    MTLCompute::Texture2D<int> texturetwo(gpu, 11, 11);
    MTLCompute::Texture3D<char> texturethree(gpu, 12, 12, 12);
    CHECK_NOTHROW(manager.loadTexture(textureone, 0));
    CHECK_THROWS(manager.loadTexture(texturetwo, 1));
    CHECK_THROWS(manager.loadTexture(texturethree, 2));
    manager.resetTextures();
}

TEST_CASE("Test correct three type getTextures") {
    MTLCompute::Texture1D<float> textureone(gpu, 10);
    MTLCompute::Texture2D<int> texturetwo(gpu, 10, 10);
    MTLCompute::Texture3D<char> texturethree(gpu, 10, 10, 10);
    manager.loadTexture(textureone, 0);
    manager.loadTexture(texturetwo, 1);
    manager.loadTexture(texturethree, 2);
    CHECK(manager.getTexture1D<float>(0).getData() == textureone.getData());
    CHECK(manager.getTexture2D<int>(1).getData() == texturetwo.getData());
    CHECK(manager.getTexture3D<char>(2).getData() == texturethree.getData());
    manager.resetTextures();

}

TEST_CASE("Test incorrect three type getTextures") {
    manager.resetTextures();
    CHECK_THROWS_AS_MESSAGE(manager.getTexture1D<float>(0).getData(), MTLCompute::Error::CommandManagerIndexError, "No 1D texture at index 0");
    CHECK_THROWS_AS_MESSAGE(manager.getTexture2D<int>(0).getData(), MTLCompute::Error::CommandManagerIndexError, "No 2D texture at index 0");
    CHECK_THROWS_AS_MESSAGE(manager.getTexture3D<char>(0).getData(), MTLCompute::Error::CommandManagerIndexError, "No 3D texture at index 0");
}
