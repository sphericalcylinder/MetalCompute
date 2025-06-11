# MetalCompute
A C++ wrapper for the Apple metal-cpp library to make it easier to run compute kernels on the GPU

[![Dev Tests](https://github.com/sphericalcylinder/MetalCompute/actions/workflows/cmake-dev-tests.yml/badge.svg?branch=dev)](https://github.com/sphericalcylinder/MetalCompute/actions/workflows/cmake-dev-tests.yml)
[![Dev Code Coverage](https://codecov.io/github/sphericalcylinder/MetalCompute/branch/github-actions-testing/graph/badge.svg?token=GWLE8BFJY8)](https://codecov.io/github/sphericalcylinder/MetalCompute)
[![CodeScene Average Code Health](https://codescene.io/projects/66945/status-badges/average-code-health)](https://codescene.io/projects/66945)


# Usage
It's a little more usable than last time. Include [MTLCompute.hpp](src/MTLCompute.hpp) for regular usage or [MTLComputeGPU.hpp](src/MTLComputeGPU.hpp) for easy usage. Most things are in the `MTLCompute::` namespace and have a look at the [examples](examples/) and the [docs](https://sphericalcylinder.github.io/MetalCompute/)
for more info. 

Building is simple, just run `cmake -S . -B build/` and then `cmake --build build/` in the top directory.
If you don't enable any of the flags down below, nothing will happen since this is a header only library.


| Thing for CMake to do | Command |
| ----------------- | ------------------ |
| Build tests | `-DMTLCOMPUTE_BUILD_TESTS=ON` |
| Install tests | `-DMTLCOMPUTE_INSTALL_TESTS=ON` |
| Build docs | `-DMTLCOMPUTE_BUILD_DOCS=ON` |
| Install docs | `-DMTLCOMPUTE_INSTALL_DOCS=ON` |
| Build examples | `-DMTLCOMPUTE_BUILD_EXAMPLES=ON` |
| Install examples | `-DMTLCOMPUTE_INSTALL_EXAMPLES=ON` |
| Report Code Coverage | `-DCOVERAGE=ON` |

If you enable an install flag, the build flag will be automatically enabled as well.


# This release
Holy guacamole! Now we have **99%** testing coverage!! This isn't needed and will most likely slip
in future releases, but I aim for above 80 or 85% (maybe even 90?) so that I can boast about it. This
release also includes better clang-format support with different standards for source and testing code using
the very cool "BasedOnParentConfig" style. The diff says that I also did something with the CMakeLists.txt
with the docs but I forgot what it was. Also there are new badges for CodeCov testing coverage and
CodeScene code health to brag extra to anybody who looks at the repo.

# Overview
Read the docs [here](https://sphericalcylinder.github.io/MetalCompute/). I spent a lot of time
on them so I hope they're good.


### Want to do:

- [x] CI Testing and Code Coverage
- [ ] More testing
- [ ] More documentation
- [ ] Perhaps seperate class definitions and implementation??

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
