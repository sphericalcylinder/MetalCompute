#include "MTLComputeGPU.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

MTLCompute::GPU<float> gpu;
std::vector<std::vector<float>> matrix(10, std::vector<float>(10, 1.0));
std::vector<float> array(10, 1.0);

TEST_CASE("Test constructor") {
    REQUIRE(gpu.getGPU() != nullptr);
}

TEST_CASE("Test loadKernel") {
    REQUIRE_NOTHROW(gpu.loadKernel("default", "both"));
}

TEST_CASE("Test loadArray and getArray") {
    gpu.loadKernel("default", "both");
    CHECK_NOTHROW(gpu.loadArray(array, 0));
    CHECK(gpu.getArray(0) == array);
}

TEST_CASE("Test loadMatrix and getMatrix") {
    gpu.loadKernel("default", "both");
    CHECK_NOTHROW(gpu.loadMatrix(matrix, 0));
    CHECK(gpu.getMatrix(0) == matrix);
}

TEST_CASE("Test runKernel") {
    gpu.loadKernel("default", "both");
    gpu.loadArray(array, 0);
    gpu.loadMatrix(matrix, 0);
    CHECK_NOTHROW(gpu.runKernel());
}