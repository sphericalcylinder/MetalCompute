#include <cstdlib>
#include <iostream>
#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include "Foundation/Foundation.hpp"
#include "Metal/Metal.hpp"


template<typename T>
std::vector<T> flatten(std::vector<std::vector<T>> &v) {
    std::vector<T> result;
    for (auto &inner : v) {
        result.insert(result.end(), inner.begin(), inner.end());
    }
    return result;
}

template <typename T>
std::vector<std::vector<T>> unflatten(std::vector<T> &v, int width, int height) {
    std::vector<std::vector<T>> result(height, std::vector<T>(width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            result[i][j] = v[i*width + j];
        }
    }
    return result;
}

/**
 * @brief A simple Metal compute shader that adds two arrays
 *
 * @return int
 *
*/
int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice(); // Create a GPU device

    MTL::Library *library = gpu->newDefaultLibrary(); // Get the default metal library (default.metallib)

    // Create a new function called add_arrays
    MTL::Function *addfunction = library->newFunction(NS::String::string("matrix_add", NS::ASCIIStringEncoding));
    
    // Create a new compute pipeline state with the addfunction
    NS::Error *error = nullptr;
    MTL::ComputePipelineState *computepipeline = gpu->newComputePipelineState(addfunction, &error);

    // Define texture width and height
    int width = 5;
    int height = 5;

    // Create the textures with 32 bit float pixel format
    MTL::TextureDescriptor *txdesc = MTL::TextureDescriptor::alloc()->init();
    txdesc->setTextureType(MTL::TextureType2D);
    txdesc->setPixelFormat(MTL::PixelFormatR32Float);
    txdesc->setWidth(width);
    txdesc->setHeight(height);

    gpu->newBuffer(10, MTL::ResourceStorageModeShared);

    MTL::Texture *textureA = gpu->newTexture(txdesc);
    MTL::Texture *textureB = gpu->newTexture(txdesc);
    MTL::Texture *textureC = gpu->newTexture(txdesc);

    // Create the command queue, command buffer, and compute command encoder
    MTL::CommandQueue *commqueue = gpu->newCommandQueue();
    MTL::CommandBuffer *commbuffer = commqueue->commandBuffer();
    MTL::ComputeCommandEncoder *commencoder = commbuffer->computeCommandEncoder();

    // Set the pipeline state and buffers
    commencoder->setComputePipelineState(computepipeline);
    commencoder->setTexture(textureA, 0);
    commencoder->setTexture(textureB, 1);
    commencoder->setTexture(textureC, 2);

    // Create the data for the textures
    std::vector<std::vector<float>> data(height, std::vector<float>(width, 1));
    std::vector<float> flatdata = flatten(data); // F L A T T E N

    // Set the data for texture A
    textureA->replaceRegion(MTL::Region::Make2D(0, 0, width, height), 
                            0, flatdata.data(), width*sizeof(float));


    // Set the data for texture B
    textureB->replaceRegion(MTL::Region::Make2D(0, 0, width, height), 
                            0, flatdata.data(), width*sizeof(float));

    // Calculate the grid size and thread group size
    MTL::Size threadgroupsize = MTL::Size(10, 10, 1);

    MTL::Size threadgroupcount;
    threadgroupcount.width = (width + threadgroupsize.width - 1) / threadgroupsize.width;
    threadgroupcount.height = (height + threadgroupsize.height - 1) / threadgroupsize.height;
    threadgroupcount.depth = 1;

    // Dispatch the compute command encoder
    commencoder->dispatchThreadgroups(threadgroupcount, threadgroupsize);
    commencoder->endEncoding();
    commbuffer->commit();
    commbuffer->waitUntilCompleted();


    // Get and verify the results
    std::vector<float> flatresult(width*height);
    textureC->getBytes(flatresult.data(), width*sizeof(float), 
                       MTL::Region::Make2D(0, 0, width, height), 0);
    
    std::vector<std::vector<float>> result = unflatten(flatresult, width, height);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << result[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // Release the resources
    // https://arc.net/l/quote/yqbmylyw
    commencoder->release();
    commbuffer->release();
    commqueue->release();
    computepipeline->release();
    addfunction->release();
    library->release();
    gpu->release();


    return 0;
}