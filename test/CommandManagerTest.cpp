#include "MTLCompute.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


std::string name = "default.metallib";
MTL::Device *gpu = MTL::CreateSystemDefaultDevice();
MTLCompute::Kernel kernel(gpu, name, "add_arrays");
MTLCompute::CommandManager manager(gpu, kernel);


TEST_CASE("Test loadBuffer") {
    MTLCompute::Buffer<float> buffer(10, gpu, MTLCompute::ResourceStorage::Shared);
    CHECK_NOTHROW(manager.loadBuffer(buffer, 0));
}

TEST_CASE("Test dispatch") {
    CHECK_NOTHROW(manager.dispatch());
}

TEST_CASE("Test refresh") {
    CHECK_NOTHROW(manager.refresh());
}

TEST_CASE("Test faulty useFunction") {
    CHECK_THROWS(kernel.useFunction("doesn't exist"));
}

TEST_CASE("Test stale dispatch") {
    manager.dispatch();
    CHECK_THROWS(manager.dispatch());
}

TEST_CASE("Test stale loadBuffer") {
    manager.refresh();
    manager.dispatch();
    MTLCompute::Buffer<float> buffer(10, gpu, MTLCompute::ResourceStorage::Shared);
    CHECK_THROWS(manager.loadBuffer(buffer, 0));
}

TEST_CASE("Test double load on same index") {
    manager.refresh();
    MTLCompute::Buffer<float> bufferone(10, gpu, MTLCompute::ResourceStorage::Shared);
    MTLCompute::Buffer<float> buffertwo(10, gpu, MTLCompute::ResourceStorage::Shared);
    CHECK_NOTHROW(manager.loadBuffer(bufferone, 0));
    CHECK_THROWS(manager.loadBuffer(buffertwo, 0));
}

TEST_CASE("Test inconsistent size loadBuffer") {
    manager.refresh();
    MTLCompute::Buffer<float> bufferone(10, gpu, MTLCompute::ResourceStorage::Shared);
    MTLCompute::Buffer<float> buffertwo(12, gpu, MTLCompute::ResourceStorage::Shared);
    CHECK_NOTHROW(manager.loadBuffer(bufferone, 0));
    CHECK_THROWS(manager.loadBuffer(buffertwo, 1));
}
