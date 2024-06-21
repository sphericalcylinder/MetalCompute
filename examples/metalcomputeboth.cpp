#include "MTLCompute.hpp"

int main() {

    // Create a GPU device
    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    // Create a kernel object from default.metallib
    MTLCompute::Kernel kernel(gpu, "default.metallib");

    // Select the "both" function to use
    kernel.useFunction("both");

    // Create a CommandManager
    MTLCompute::CommandManager<float> manager(gpu, &kernel);

    // Create a buffer
    MTLCompute::Buffer<float> buffera(gpu, 10, MTLCompute::ResourceStorage::Shared);

    // Create a vector of data
    vec<float> bufferdata = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Fill the buffer with data
    buffera = bufferdata;

    int width = 5; // width of the texture
    int height = 5; // height of the texture

    // Create a 2D texture
    MTLCompute::Texture2D<float> textureD(gpu, width, height);

    // Load the buffer and texture
    manager.loadBuffer(buffera, 0);
    manager.loadTexture(textureD, 0);

    // Dispatch the kernel
    manager.dispatch();

    // Get and print the result (addition tables)
    std::cout << std::endl;
    vec2<float> bothresult = textureD.getData();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << bothresult[i][j] << " ";
        }
        std::cout << std::endl;
    }

    manager.resetBuffers();
}