# MetalCompute
A C++ wrapper for the Apple metal-cpp library to make it easier to run compute kernels on the GPU

# Usage
You can technically use it but not really. Include [MTLCompute.hpp](src/MTLCompute.hpp) or individual
headers as needed. Everything is in the `MTLCompute::` namespace and have a look at [this example](src/hellometalcompute.cpp)
for more info. 
> [!CAUTION]
> ***DO NOT*** build the documentation. I haven't written it.

Building is simple, just `cmake -S . -B build/` and then `cmake --build build/` in the top directory.
That will build the two example files. To get tests, add `-DMTLCOMPUTE_BUILD_TESTS=ON` to the first command
before the `-S`. To get the docs, you do nothing because I'm not letting you build them yet. Enjoy!!!!!


# About
I not recently created a C++ application and wanted to use Apple's metal-cpp library to add gpu
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
