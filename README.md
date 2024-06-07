# MetalCompute
A C++ wrapper for the Apple metal-cpp library to make it easier to run compute kernels on the GPU

[![Dev Tests](https://github.com/sphericalcylinder/MetalCompute/actions/workflows/cmake-dev-tests.yml/badge.svg?branch=dev)](https://github.com/sphericalcylinder/MetalCompute/actions/workflows/cmake-dev-tests.yml)


# Usage
It's pretty usable. Include [MTLCompute.hpp](src/MTLCompute.hpp) for regular usage or [MTLComputeGPU.hpp](src/MTLComputeGPU.hpp) for easy usage. Everything is in the `MTLCompute::` namespace and you should have a look at the [examples](examples/) and the [docs](https://sphericalcylinder.github.io/MetalCompute/)
for more info.

Building is simple, just run `cmake -S . -B build/` and then `cmake --build build/` in the top directory.

If you want to be extra, here's some extra flags you can add to the first command.

| Thing for CMake to do | Command |
| ----------------- | ------------------ |
| Build tests | `-DMTLCOMPUTE_BUILD_TESTS=ON` |
| Install tests | `-DMTLCOMPUTE_INSTALL_TESTS=ON` |
| Build docs | `-DMTLCOMPUTE_BUILD_DOCS=ON` |
| Install docs | `-DMTLCOMPUTE_INSTALL_DOCS=ON` |
| Build examples | `-DMTLCOMPUTE_BUILD_EXAMPLES=ON` |
| Install examples | `-DMTLCOMPUTE_INSTALL_EXAMPLES=ON` |


If you enable an install flag, the build flag will be automatically enabled as well.


# Overview
Read the docs [here](https://sphericalcylinder.github.io/MetalCompute/). I spent a lot of time
on them so I hope they're good.


## Goals
The goals for this project (which will probably change) are as follows:

- Concise and easy to read code
- A working API (duh)
- Minimal overhead
- Extensive documentation (Doxygen)
- Good, if not complete testing code coverage (doctest)
- and more!! (i cant think)

## Want to do:

- [ ] 1d and 3d textures
- [ ] Convert buffers to textures
- [ ] More kernel info commands
- [ ] More texture values (RGBA)


# Why use this?
This project is based on Apple's metal-cpp wrapper. It's a fantastic project that lets you add gpu compute and build apps in C++
instead of Objective-C or Objective-C++. The only problem is that there's a lot of setup and teardown boilerplate. MetalCompute just
simplifies the calls to metal-cpp and makes it much more user friendly. If you want to add 2 arrays on the gpu with pure metal-cpp,
it will look like this:
```cpp
#include <cstdlib>
#include <iostream>
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

    std::vector<float> data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    memcpy(bufferA->contents(), data.data(), bufferlength*itemsize);
    memcpy(bufferB->contents(), data.data(), bufferlength*itemsize);

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
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;

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
```

MetalCompute takes all of that and bundles it up into a much tighter package.
This is the same code using the MetalCompute library:
```cpp
#include "MTLCompute.hpp"
#include <algorithm>
#include <vector>
#include <iostream>

int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    MTLCompute::Kernel kernel(gpu, "default.metallib");

    kernel.useFunction("add_arrays");

    MTLCompute::Buffer<float> buffera(gpu, 10, MTLCompute::ResourceStorage::Shared);
    MTLCompute::Buffer<float> bufferb(gpu, 10, MTLCompute::ResourceStorage::Shared);
    MTLCompute::Buffer<float> bufferc(gpu, 10, MTLCompute::ResourceStorage::Shared);

    std::vector<float> bufferdata = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    buffera = bufferdata;
    bufferb = bufferdata;

    MTLCompute::CommandManager<float> manager(gpu, &kernel);
    manager.loadBuffer(buffera, 0);
    manager.loadBuffer(bufferb, 1);
    manager.loadBuffer(bufferc, 2);

    manager.dispatch();

    std::vector<float> result = bufferc.getData();
    for (int i = 0; i < bufferc.length; i++) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;

}
```
We just shaved off 39 lines of code while making everything more readable.
This example and more are in the [examples folder](examples/).


# Development Resources

- [Metal Docs](https://developer.apple.com/documentation/metal/) (they're in Objective-C)
- [Metal Best Practices](https://developer.apple.com/library/archive/documentation/3DDrawing/Conceptual/MTLBestPracticesGuide/index.html)
- [Metal Feature Set Tables](https://developer.apple.com/metal/Metal-Feature-Set-Tables.pdf)
- [Metal Shading Language Specs](https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf)

### The end :)
