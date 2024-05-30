# MetalCompute
A C++ wrapper for the Apple metal-cpp library to make it easier to run compute kernels on the GPU

# Usage
It's a little more usable than last time. Include [MTLCompute.hpp](src/MTLCompute.hpp) or individual
headers as needed. Everything is in the `MTLCompute::` namespace and have a look at [this example](src/hellometalcompute.cpp)
for more info. 
> [!CAUTION]
> ***YOU CAN*** build the documentation. I wrote it. It's ugly though.

Building is simple, just run `cmake -S . -B build/` and then `cmake --build build/` in the top directory.
That will build the two example files. To get tests, add `-DMTLCOMPUTE_BUILD_TESTS=ON` to the first command
before the `-S`. To get the docs, add `-DMTLCOMPUTE_BUILD_DOCS=ON`. Enjoy!!!!!


# This update:

- Tragic oversight fixed!!
  - MTLCompute::CommandManager::dispatch() used `commandEncoder->dispatchThreadgroups()`
    when the arguments were meant for `commandEncoder->dispatchThreads()`
  - The texture size was limited to 32x32
  - It's fixed now
  - Now the texture limit is 16384x16384 as defined
    by the [Metal Feature Set Tables](https://developer.apple.com/metal/Metal-Feature-Set-Tables.pdf)

### Want to do:

- [x] One CommandManager
- [ ] Implement strict limits on texture size (16384)
- [ ] Limit number of loadable textures and buffers
- [ ] 1d and 3d textures
- [ ] MTLCompute::GPU class to manage everything else
- [ ] More access to texture data (like [])
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
