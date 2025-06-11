#include "MTLCompute.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

TEST_CASE("Test full functionality") {

    MTLCompute::Kernel kernel(gpu, "default.metallib");

    kernel.useFunction("add_arrays");

    MTLCompute::CommandManager<int, float> manager(gpu, &kernel);

    MTLCompute::Buffer<float> buffera(gpu, 20);
    // buffera = vec<float>(20, 5.0f);

    MTLCompute::Buffer<float> bufferb(gpu, 20);
    vec<float> randvals(20);
    for (int i = 0; i < 20; i++) {
        randvals[i] = static_cast<float>(rand()/(static_cast<float>(RAND_MAX)*2.0f));
        bufferb[i] = randvals[i];
    }

    MTLCompute::Buffer<float> bufferc(gpu, 20);

    manager.loadBuffer(buffera, 0);
    manager.loadBuffer(bufferb, 1);
    manager.loadBuffer(bufferc, 2);

    manager.dispatch();

    vec<float> results = manager.getBuffers<float>()[2].getData();

    std::cout << "add_arrays results: ";
    for (int i = 0; i < 5; i++) {
        std::cout << results[i] << " ";
    }
    std::cout << std::endl;

    manager.reset();

    kernel.useFunction("dim_add_textures");

    const int texa_size = 4;
    const int texb_size = 4;
    const int texc_size = 4;

    MTLCompute::Texture1D<float> texturea(gpu, texa_size);
    MTLCompute::Texture2D<int> textureb(gpu, texb_size, texb_size);
    MTLCompute::Texture3D<float> texturec(gpu, texc_size, texc_size, texc_size);

    // Fill input textures with data
    vec<float> texa_data(texa_size, 2.0f);
    vec2<int> texb_data(texb_size, vec<int>(texb_size, 3));
    texturea = texa_data;
    textureb = texb_data;

    // Load textures into manager
    manager.loadTexture(texturea, 0);
    manager.loadTexture(textureb, 1);
    manager.loadTexture(texturec, 2);

    manager.dispatch();

    // Get and print a few output values from the 3D texture
    auto texc_data = texturec.getData();
    std::cout << "Texture dim_add_textures results: ";
    for (int i = 0; i < std::min(5, texc_size); i++) {
        std::cout << texc_data[i][0][0] << " ";
    }
    std::cout << std::endl;

    manager.reset();

}