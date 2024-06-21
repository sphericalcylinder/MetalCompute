#include "MTLCompute.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


std::string name = "default.metallib";
MTL::Device *gpu = MTL::CreateSystemDefaultDevice();
MTLCompute::Kernel kernel(gpu, name);


TEST_CASE("Test getFunctionNames") {
    vec<std::string> names = kernel.getFunctionNames();
    CHECK(names.size() > 0);
}

TEST_CASE("Test useFunction") {
    CHECK_NOTHROW(kernel.useFunction("add_arrays"));
}

TEST_CASE("Test faulty useFunction") {
    CHECK_THROWS_AS_MESSAGE(kernel.useFunction("doesn't exist"), MTLCompute::KernelLoadError,
        "Could not find function doesn't exist");
}