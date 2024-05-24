#include "MTLCompute.hpp"
#include <algorithm>
#include <vector>

int main() {

    // Create a GPU device
    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    // Create a kernel object from default.metallib
    MTLCompute::Kernel kernel(gpu, "default.metallib");

    // Print the functions in the default.metallib
    std::cout << "Functions in the default.metallib: " << std::endl;
    std::vector<std::string> names = kernel.getFunctionNames();
    for (int i = 0; i < names.size(); i++) {
        std::cout << names[i] << std::endl;
    }

    // Select the "add_arrays" function to use
    kernel.useFunction("add_arrays");

    // Create buffers
    MTLCompute::Buffer<float> buffera(10, gpu, MTLCompute::ResourceStorage::Shared);
    MTLCompute::Buffer<float> bufferb(10, gpu, MTLCompute::ResourceStorage::Shared);
    MTLCompute::Buffer<float> bufferc(10, gpu, MTLCompute::ResourceStorage::Shared);

    std::vector<float> bufferdata = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // fill the buffers with data
    buffera = bufferdata;
    bufferb = bufferdata;

    // Create a CommandManager and load the buffers
    MTLCompute::CommandManager manager(gpu, kernel);
    manager.loadBuffer(buffera, 0);
    manager.loadBuffer(bufferb, 1);
    manager.loadBuffer(bufferc, 2);

    // Dispatch the kernel
    manager.dispatch();

    // Get and print the result
    std::vector<float> result = bufferc.getData();
    for (int i = 0; i < bufferc.length; i++) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;

}