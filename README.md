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

- No more MTLCompute::TextureCommandManager
  - Use loadTexture to load a texture to the CommandManager
  - Takes the kernel address instead of the kernel
  - Automatically updates the pipeline
  - Call `resetBuffers()`, `resetTextures()`, and `reset()` to reset the cached values
- No docs for the new stuff

### Want to do:

- [x] One CommandManager
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


### The end :)
