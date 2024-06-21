#include "MTLCompute.hpp"

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

    // Create the 2d textures and fill them with data
    vec2<float> texdata(height, vec<float>(width, 1));
    
    MTLCompute::Texture2D<float> textureA(gpu, width, height);
    textureA = texdata;

    MTLCompute::Texture2D<float> textureB(gpu, width, height);
    textureB = texdata;

    MTLCompute::Texture2D<float> textureC(gpu, width, height);

    // Load the textures
    manager.loadTexture(textureA, 0);
    manager.loadTexture(textureB, 1);
    manager.loadTexture(textureC, 2);

    // Dispatch the kernel
    manager.dispatch();

    // Get and print the result
    vec2<float> texresult = textureC.getData();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << texresult[i][j] << " ";
        }
        std::cout << std::endl;
    }

}