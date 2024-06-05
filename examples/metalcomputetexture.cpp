#include "MTLCompute.hpp"
#include <algorithm>
#include <vector>
#include <iostream>

int main() {

    // Create a GPU device
    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    // Create a kernel object from default.metallib
    MTLCompute::Kernel kernel(gpu, "default.metallib");

    // Select the "matrix_add" function to use
    kernel.useFunction("matrix_add"); 

    // Create a CommandManager
    MTLCompute::CommandManager<float> manager(gpu, &kernel);

    int width = 5; // width of the texture
    int height = 5; // height of the texture

    // Create the textures and fill them with data
    std::vector<std::vector<float>> texdata(height, std::vector<float>(width, 1));

    MTLCompute::Texture<float> textureA(gpu, width, height, MTLCompute::TextureType::float32);
    textureA = texdata;

    MTLCompute::Texture<float> textureB(gpu, width, height, MTLCompute::TextureType::float32);
    textureB = texdata;

    MTLCompute::Texture<float> textureC(gpu, width, height, MTLCompute::TextureType::float32);

    // Select the "matrix_add" function to use
    kernel.useFunction("matrix_add");   

    // Load the textures
    manager.loadTexture(textureA, 0);
    manager.loadTexture(textureB, 1);
    manager.loadTexture(textureC, 2);

    // Dispatch the kernel
    manager.dispatch();

    // Get and print the result
    std::vector<std::vector<float>> texresult = textureC.getData();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << texresult[i][j] << " ";
        }
        std::cout << std::endl;
    }

}