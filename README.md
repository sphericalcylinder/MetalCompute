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

- More initialization for MTLCompute::Buffer
  - Initialize buffers with other buffers!
  - Set buffers to other buffers even after initialization!
  - Copy and swap!!
  - Default constructor (initalize blank buffer!)
- I love the new MTLCompute::CommandManager! :heart:
  - Less likely to segfault
  - Set buffers to any index (less than 50)
  - Do it however many times you want
  - Call `dispatch()` an infinite number of times, it won't break (probably)
  - And no more pesky `refresh()` after calling it
  - I forgot to add something to clear the buffers
  - gosh darn it
  - I'll do it in another commit
  - **You have to supply the type that you're using for the buffers**
  - If your buffers are `MTLCompute::Buffer<float>` then the CommandManager
  has to be `MTLCompute::CommandManager<float>`
  - Please don't supply the wrong type
- MTLCompute::Kernel!
  - I didn't do anything with it
  - Just wrote documentation
- Docs
  - Technically complete
  - Need to write a mainpage


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
