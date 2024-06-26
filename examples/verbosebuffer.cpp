#include <cstdlib>
#include <iostream>
#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include "Foundation/Foundation.hpp"
#include "Metal/Metal.hpp"

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
    MTL::Function *addfunction = library->newFunction(NS::String::string("add_arrays", NS::ASCIIStringEncoding));
    
    // Create a new compute pipeline state with the addfunction
    NS::Error *error = nullptr;
    MTL::ComputePipelineState *computepipeline = gpu->newComputePipelineState(addfunction, &error);

    // Define buffer length and item size
    int bufferlength = 10;
    size_t itemsize = sizeof(float);

    // Create the buffers with shared memory
    MTL::Buffer *bufferA = gpu->newBuffer(bufferlength*itemsize, MTL::ResourceStorageModeShared);
    MTL::Buffer *bufferB = gpu->newBuffer(bufferlength*itemsize, MTL::ResourceStorageModeShared);
    MTL::Buffer *bufferC = gpu->newBuffer(bufferlength*itemsize, MTL::ResourceStorageModeShared);

    // Create the command queue, command buffer, and compute command encoder
    MTL::CommandQueue *commqueue = gpu->newCommandQueue();
    MTL::CommandBuffer *commbuffer = commqueue->commandBuffer();
    MTL::ComputeCommandEncoder *commencoder = commbuffer->computeCommandEncoder();

    // Set the pipeline state and buffers
    commencoder->setComputePipelineState(computepipeline);
    commencoder->setBuffer(bufferA, 0, 0);
    commencoder->setBuffer(bufferB, 0, 1);
    commencoder->setBuffer(bufferC, 0, 2);

    std::vector<float> data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Copy the data to the buffers
    memcpy(bufferA->contents(), data.data(), bufferlength*itemsize);
    memcpy(bufferB->contents(), data.data(), bufferlength*itemsize);

    // Calculate the grid size and thread group size
    MTL::Size gridsize = MTL::Size(bufferlength, 1, 1);
    NS::UInteger maxthreads = computepipeline->maxTotalThreadsPerThreadgroup();
    if (maxthreads > bufferlength) {
        maxthreads = bufferlength;
    }

    MTL::Size threadgroupsize = MTL::Size(maxthreads, 1, 1);

    // Dispatch the compute command encoder
    commencoder->dispatchThreadgroups(gridsize, threadgroupsize);
    commencoder->endEncoding();
    commbuffer->commit();
    commbuffer->waitUntilCompleted();


    // Get and verify the results
    float *result = (float *)bufferC->contents();
    for (int i = 0; i < bufferlength; i++) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;

    // Release the resources
    // https://arc.net/l/quote/yqbmylyw
    commencoder->release();
    commbuffer->release();
    commqueue->release();
    bufferA->release();
    bufferB->release();
    bufferC->release();
    computepipeline->release();
    addfunction->release();
    library->release();
    gpu->release();


    return 0;
}