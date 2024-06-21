#include "MTLCompute.hpp"


int main() {

    // Create a GPU device
    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    // Create a kernel object from default.metallib
    MTLCompute::Kernel kernel(gpu, "default.metallib");

    // Select the "matrix_add" function to use
    kernel.useFunction("vector_add_1d"); 

    // Create a CommandManager
    MTLCompute::CommandManager<MTLCompute::float2> manager(gpu, &kernel);

    int width = 5; // width of the texture

    // Create the 2d textures with 2 components and fill them with data
    vec<MTLCompute::float2> texdata(width, std::make_pair(1.0f, 1.0f));
    
    MTLCompute::Texture1D<MTLCompute::float2> textureA(gpu, width);
    textureA = texdata;

    MTLCompute::Texture1D<MTLCompute::float2> textureB(gpu, width);
    textureB = texdata;

    MTLCompute::Texture1D<MTLCompute::float2> textureC(gpu, width);

    // Load the textures
    manager.loadTexture(textureA, 0);
    manager.loadTexture(textureB, 1);
    manager.loadTexture(textureC, 2);

    // Dispatch the kernel
    manager.dispatch();

    // Get and print the result
    vec<MTLCompute::float2> texresult = textureC.getData();
    std::cout << "1D with 2 components:\n";
    for (int i = 0; i < width; i++) {
        std::cout << texresult[i].first << " " << texresult[i].second << std::endl;
    }

    // Reset the CommandManager
    manager.reset();

    // Select the "vector_add_2d" function to use
    kernel.useFunction("vector_add_2d");

    int height = 5; // height of the texture

    // Create the 2d textures with 2 components and fill them with data
    vec2<MTLCompute::float2> texdata2(width, texdata);

    MTLCompute::Texture2D<MTLCompute::float2> textureD(gpu, width, height);
    textureD = texdata2;

    MTLCompute::Texture2D<MTLCompute::float2> textureE(gpu, width, height);
    textureE = texdata2;

    MTLCompute::Texture2D<MTLCompute::float2> textureF(gpu, width, height);

    // Load the textures
    manager.loadTexture(textureD, 0);
    manager.loadTexture(textureE, 1);
    manager.loadTexture(textureF, 2);

    // Dispatch the kernel
    manager.dispatch();

    // Get and print the result
    vec2<MTLCompute::float2> texresult2 = textureF.getData();
    std::cout << "\n\n2D with 2 components:\n";
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            std::cout << texresult2[i][j].first << " " << texresult2[i][j].second << "\t";
        }
        std::cout << std::endl;
    }

}