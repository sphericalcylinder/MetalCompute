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
    MTLCompute::CommandManager<float> manager(gpu, kernel);
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

    // Create the textures
    std::vector<std::vector<float>> texdata(10, std::vector<float>(10, 1));

    MTLCompute::Texture<float> textureA(gpu, 10, 10, MTLCompute::TextureType::float32);
    textureA = texdata;

    MTLCompute::Texture<float> textureB(gpu, 10, 10, MTLCompute::TextureType::float32);
    textureB = texdata;

    MTLCompute::Texture<float> textureC(gpu, 10, 10, MTLCompute::TextureType::float32);

    // Select the "matrix_add" function to use
    kernel.useFunction("matrix_add");   

    // Create a TextureCommandManager and load the textures
    MTLCompute::TextureCommandManager<float> texturemanager(gpu, kernel);
    texturemanager.loadTexture(textureA, 0);
    texturemanager.loadTexture(textureB, 1);
    texturemanager.loadTexture(textureC, 2);

    // Dispatch the kernel
    texturemanager.dispatch();

    // Get and print the result
    std::vector<std::vector<float>> texresult = textureC.getData();
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            std::cout << texresult[i][j] << " ";
        }
        std::cout << std::endl;
    }

}