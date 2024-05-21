#include <cstdlib>
#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include "Foundation/Foundation.hpp"
#include "Metal/Metal.hpp"


int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    MTL::Library *library = gpu->newDefaultLibrary();
    MTL::Function *addfunction = library->newFunction(NS::String::string("add_arrays", NS::ASCIIStringEncoding));
    NS::Error *error = nullptr;
    MTL::ComputePipelineState *computepipeline = gpu->newComputePipelineState(addfunction, &error);

    int bufferlength = 10;
    size_t itemsize = sizeof(float);

    MTL::Buffer *bufferA = gpu->newBuffer(bufferlength*itemsize, MTL::ResourceStorageModeShared);
    MTL::Buffer *bufferB = gpu->newBuffer(bufferlength*itemsize, MTL::ResourceStorageModeShared);
    MTL::Buffer *bufferC = gpu->newBuffer(bufferlength*itemsize, MTL::ResourceStorageModeShared);

    MTL::CommandQueue *commqueue = gpu->newCommandQueue();
    MTL::CommandBuffer *commbuffer = commqueue->commandBuffer();
    MTL::ComputeCommandEncoder *commencoder = commbuffer->computeCommandEncoder();

    commencoder->setComputePipelineState(computepipeline);
    commencoder->setBuffer(bufferA, 0, 0);
    commencoder->setBuffer(bufferB, 0, 1);
    commencoder->setBuffer(bufferC, 0, 2);


    srand(time(NULL));
    float *bufferAcontents = (float *)bufferA->contents();
    for (int i = 0; i < bufferlength; i++) {
        bufferAcontents[i] = rand() % 1000;
    }

    float *bufferBcontents = (float *)bufferB->contents();
    for (int i = 0; i < bufferlength; i++) {
        bufferBcontents[i] = rand() % 1000;
    }


    MTL::Size gridsize = MTL::Size(bufferlength, 1, 1);
    NS::UInteger maxthreads = computepipeline->maxTotalThreadsPerThreadgroup();
    if (maxthreads > bufferlength) {
        maxthreads = bufferlength;
    }

    MTL::Size threadgroupsize = MTL::Size(maxthreads, 1, 1);

    commencoder->dispatchThreadgroups(gridsize, threadgroupsize);
    commencoder->endEncoding();
    commbuffer->commit();
    commbuffer->waitUntilCompleted();


    float *result = (float *)bufferC->contents();
    for (int i = 0; i < bufferlength; i++) {
        if (bufferAcontents[i] + bufferBcontents[i] != result[i]) {
            printf("Error: %f + %f != %f\n", bufferAcontents[i], bufferBcontents[i], result[i]);
        }
        printf("%f + %f = %f\n", bufferAcontents[i], bufferBcontents[i], result[i]);
    }


    return 0;
}