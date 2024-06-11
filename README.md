# MetalCompute
A C++ wrapper for the Apple metal-cpp library to make it easier to run compute kernels on the GPU

[![Dev Tests](https://github.com/sphericalcylinder/MetalCompute/actions/workflows/cmake-dev-tests.yml/badge.svg?branch=dev)](https://github.com/sphericalcylinder/MetalCompute/actions/workflows/cmake-dev-tests.yml)


# Usage
It's a little more usable than last time. Include [MTLCompute.hpp](src/MTLCompute.hpp) for regular usage or [MTLComputeGPU.hpp](src/MTLComputeGPU.hpp) for easy usage. Everything is in the `MTLCompute::` namespace and have a look at the [examples](examples/) and the [docs](https://sphericalcylinder.github.io/MetalCompute/)
for more info. 

Building is simple, just run `cmake -S . -B build/` and then `cmake --build build/` in the top directory.


| Thing for CMake to do | Command |
| ----------------- | ------------------ |
| Build tests | `-DMTLCOMPUTE_BUILD_TESTS=ON` |
| Install tests | `-DMTLCOMPUTE_INSTALL_TESTS=ON` |
| Build docs | `-DMTLCOMPUTE_BUILD_DOCS=ON` |
| Install docs | `-DMTLCOMPUTE_INSTALL_DOCS=ON` |
| Build examples | `-DMTLCOMPUTE_BUILD_EXAMPLES=ON` |
| Install examples | `-DMTLCOMPUTE_INSTALL_EXAMPLES=ON` |


If you enable an install flag, the build flag will be automatically enabled as well.

# This release
- Textures
  - Multiple types (1d, 2d, 3d)
  - I think i did it right
  - Maybe not
  - Tests are written
  - Docs are not
- CommandManager
  - Only supports 2d textures for the time being
  - I can't think of an elegant way to do multiple
- GPU
  - Same problem as CommandManager

Sorry this one took so long. I had finals the last two weeks. The docs are coming in the next commit.

# Overview
Read the docs [here](https://sphericalcylinder.github.io/MetalCompute/). I spent a lot of time
on them so I hope they're good.


### Want to do:

- [x] 1d and 3d textures
- [ ] Command Manager 1d and 3d texture support
- [ ] Convert buffers to textures
- [ ] More kernel info commands
- [ ] More texture values (RGBA)

# About
I not so recently created a C++ application and wanted to use Apple's metal-cpp library to add gpu
proccessing capability, but I found out that it was hard to do. It eventually worked, but I don't want
to deal with that again, which is why I'm creating this.


## Goals
The goals for this project (which will probably change) are as follows:

- Concise and easy to read code
- A working API (duh)
- Minimal overhead
- Extensive documentation (Doxygen)
- Good, if not complete testing code coverage (doctest)
- and more!! (i cant think)


# Development Resources

- [Metal Docs](https://developer.apple.com/documentation/metal/) (they're in Objective-C)
- [Metal Best Practices](https://developer.apple.com/library/archive/documentation/3DDrawing/Conceptual/MTLBestPracticesGuide/index.html)
- [Metal Feature Set Tables](https://developer.apple.com/metal/Metal-Feature-Set-Tables.pdf)
- [Metal Shading Language Specs](https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf)

### The end :)
