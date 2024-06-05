# MetalCompute
A C++ wrapper for the Apple metal-cpp library to make it easier to run compute kernels on the GPU

# Usage
It's a little more usable than last time. Include [MTLCompute.hpp](src/MTLCompute.hpp) or individual
headers as needed. Everything is in the `MTLCompute::` namespace and have a look at [this example](src/hellometalcompute.cpp)
for more info. 
> [!CAUTION]
> ***YOU CAN*** build the documentation. I wrote it. It's ugly though.

Building is simple, just run `cmake -S . -B build/` and then `cmake --build build/` in the top directory.

| Thing for CMake to do | Command |
-----------------------------------
| Build tests | `-DMTLCOMPUTE_BUILD_TESTS=ON` |
| Build docs | `-DMTLCOMPUTE_BUILD_DOCS=ON` |
| Install tests | `-DMTLCOMPUTE_INSTALL_TESTS=ON` |
| Install docs | `-DMTLCOMPUTE_INSTALL_DOCS=ON` |
| Install examples | `-DMTLCOMPUTE_INSTALL_EXAMPLES=ON` |


# This update:

- Documentation main page is written!
- It's not very good
- Small CMake tweaks
  - Added build and install options for tests, docs, examples
  - Use single metal header insted of two
  - aaaaa

I'm satisfied with the docs for now so it's time to merge with main!

### Want to do:

- [x] One CommandManager
- [x] Implement strict limits on texture size (16384)
- [x] Limit number of loadable textures and buffers
- [ ] 1d and 3d textures
- [x] MTLCompute::GPU class to manage everything else
- [x] More access to texture data (like [])
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
