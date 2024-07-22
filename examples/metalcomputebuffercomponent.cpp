#include "MTLCompute.hpp"

int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    MTLCompute::Kernel kernel(gpu, "default.metallib");

    kernel.useFunction("add_arrays_2");

    int length = 5;

    MTLCompute::Buffer<MTLCompute::int4> bufferA(gpu, length);
    MTLCompute::Buffer<MTLCompute::int4> bufferB(gpu, length);
    MTLCompute::Buffer<MTLCompute::int4> bufferC(gpu, length);

    std::vector<MTLCompute::int4> data;

    for (int i = 0; i < length; i++) {
        data.push_back(MTLCompute::int4(1, 2, 3, 4));
    }

    bufferA = data;
    bufferB = data;    

    MTLCompute::CommandManager<MTLCompute::int4> manager(gpu, &kernel);

    manager.loadBuffer(bufferA, 0);
    manager.loadBuffer(bufferB, 1);
    manager.loadBuffer(bufferC, 2);

    manager.dispatch();

    std::vector<MTLCompute::int4> result = bufferC.getData();

    using std::get;

    for (int i = 0; i < length; i++) {
        std::cout << get<0>(result[i]) << " " << get<1>(result[i]) << " " \
                  << get<2>(result[i]) << " " << get<3>(result[i]) << std::endl;
    }
}