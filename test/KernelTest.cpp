#include "MTLCompute.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

std::string name = "default.metallib";
MTL::Device *gpu = MTL::CreateSystemDefaultDevice();
MTLCompute::Kernel kernel(gpu, name);

TEST_CASE("Test Kernel constructor") { CHECK_NOTHROW(MTLCompute::Kernel(gpu, name)); }

TEST_CASE("Test Kernel constructor with function name") {
    CHECK_NOTHROW(MTLCompute::Kernel(gpu, name, "add_arrays"));
}

TEST_CASE("Test Kernel constructor with faulty function name") {
    CHECK_THROWS_AS_MESSAGE(
        MTLCompute::Kernel(gpu, name, "doesn't exist"),
        MTLCompute::Error::KernelLoadError, "Could not load library default.metallib"
    );
}

TEST_CASE("Test Kernel constructor with faulty library") {
    CHECK_THROWS_AS_MESSAGE(
        MTLCompute::Kernel(gpu, "doesn't exist"), MTLCompute::Error::KernelLoadError,
        "Could not load library doesn't exist"
    );
}

TEST_CASE("Test getFunctionNames") {
    vec<std::string> names = kernel.getFunctionNames();
    REQUIRE(names.size() > 0);
}

TEST_CASE("Test useFunction") { CHECK_NOTHROW(kernel.useFunction("add_arrays")); }

TEST_CASE("Test faulty useFunction") {
    CHECK_THROWS_AS_MESSAGE(
        kernel.useFunction("doesn't exist"), MTLCompute::Error::KernelLoadError,
        "Could not find function doesn't exist"
    );
}