#include "MTLCompute.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


std::string name = "default.metallib";
MTL::Device *gpu = MTL::CreateSystemDefaultDevice();
MTLCompute::Kernel kernel(gpu, name, "add_arrays");
MTLCompute::CommandManager<float> manager(gpu, kernel);


TEST_CASE("Test loadBuffer") {
    MTLCompute::Buffer<float> buffer(10, gpu, MTLCompute::ResourceStorage::Shared);
    CHECK_NOTHROW(manager.loadBuffer(buffer, 0));
}

TEST_CASE("Test dispatch") {
    CHECK_NOTHROW(manager.dispatch());
}

TEST_CASE("Test double dispatch") {
    manager.dispatch();
    CHECK_NOTHROW(manager.dispatch());
}

TEST_CASE("Test double load on same index") {
    MTLCompute::Buffer<float> bufferone(10, gpu, MTLCompute::ResourceStorage::Shared);
    MTLCompute::Buffer<float> buffertwo(10, gpu, MTLCompute::ResourceStorage::Shared);
    CHECK_NOTHROW(manager.loadBuffer(bufferone, 0));
    CHECK_NOTHROW(manager.loadBuffer(buffertwo, 0));
}

TEST_CASE("Test inconsistent size loadBuffer") {
    MTLCompute::Buffer<float> bufferone(10, gpu, MTLCompute::ResourceStorage::Shared);
    MTLCompute::Buffer<float> buffertwo(12, gpu, MTLCompute::ResourceStorage::Shared);
    CHECK_NOTHROW(manager.loadBuffer(bufferone, 0));
    CHECK_THROWS(manager.loadBuffer(buffertwo, 1));
}
