#include "MTLCompute.hpp"

int main() {

    // Create a GPU device
    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    // Create a kernel object from default.metallib
    MTLCompute::Kernel kernel(gpu, "default.metallib");

    // Select the "matrix_add" function to use
    kernel.useFunction("matrix_add_3d"); 

    // Create a CommandManager
    MTLCompute::CommandManager<float> manager(gpu, &kernel);

    int width = 5; // width of the texture
    int height = 5; // height of the texture
    int depth = 5; // depth of the texture

    // Create the 3d textures and fill them with data
    vec3<float> texdata(depth, vec2<float>(height, vec<float>(width, 1.0f)));
    
    MTLCompute::Texture3D<float> textureA(gpu, width, height, depth, MTLCompute::TextureItemType::float32);

    MTLCompute::Texture3D<float> textureB(gpu, width, height, depth, MTLCompute::TextureItemType::float32);

    MTLCompute::Texture3D<float> textureC(gpu, width, height, depth, MTLCompute::TextureItemType::float32);

    textureA = texdata;
    textureB = texdata;

    // Load the textures
    manager.loadTexture(textureA, 0);
    manager.loadTexture(textureB, 1);
    manager.loadTexture(textureC, 2);

    // Dispatch the kernel
    manager.dispatch();

    // Get and print the result
    vec3<float> texresult = textureC.getData();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < depth; k++) {
                std::cout << texresult[i][j][k] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

}