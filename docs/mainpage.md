MetalCompute Documentation  {#mainpage}
================

# MetalCompute
MetalCompute is an API to make GPU compute calls easier. Usually, on a mac, you would have to use
Objective-C or Swift to use the Metal API. I don't know either and don't really want to put the effort 
into learning them. Thankfully, Apple released [metal-cpp](https://github.com/bkaradzic/metal-cpp), a C++ API that
calls Objective-C Metal functions. It's extremely useful, but if you want to do some GPU calculations for a simple project,
it's too much boilerplate. That's why I'm making this, a API on top of another API to use Metal for GPU compute. If you have
suggestions or find a bug, file an issue on the [repo](https://github.com/sphericalcylinder/MetalCompute) for this
project. Thank you so much!

================

# Usage {#usg}
[TOC]

This is a header only API, you just need to include the directory where `MTLCompute.hpp` is located after cmake install.
You also have to add the
`-framework Foundation -framework Metal -framework MetalKit` compilation flags or there will be problems.

To use the API regularly, like in the [buffer](examples/metalcomputebuffer.cpp) and [texture](examples/metalcompute2dtexture.cpp)
examples, include the `MTLCompute.hpp` header and add the [other flags](#usg) during compilation.

Currently, you still have to access the raw MTL::Device object because I haven't made a wrapper for that and probably never will. There is
a MTLCompute::GPU class, but it's used differently, dissused in the [Special GPU Class](#special-gpu-class) section. The start of your file should look like this:
```cpp
#include "MTLCompute.hpp"

int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

}
```

That's the only work you have to do with the metal-cpp API directly. Now, you can use MTLCompute in place of metal-cpp for simple GPU
compute tasks.


================

# Data Structures

<br />

## Buffers

A MTLCompute::Buffer is a 1D list that holds a predetermined number of elements. It's really simple. To create
a MTLCompute::Buffer, you specify a gpu, length, and a MTLCompute::ResourceStorage option (optional). When choosing your
MTLCompute::ResourceStorage value, refer to the 
[Metal Best Practices](https://developer.apple.com/library/archive/documentation/3DDrawing/Conceptual/MTLBestPracticesGuide/ResourceOptions.html#//apple_ref/doc/uid/TP40016642-CH17-SW1)
site under "Choose an Appropriate Resource Storage Mode (macOS)". Most commonly, you'll use MTLCompute::ResourceStorage::Shared.
If you don't want to type out that long thing, just leave that argument out and the Buffer will use shared storage by default.

To create a MTLCompute::Buffer that holds 10 floats:
```cpp
#include "MTLCompute.hpp"

int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    MTLCompute::Buffer<float> mybuffer(gpu, 10, MTLCompute::ResourceStorage::Shared);
    // without resourcestorage parameter:
    MTLCompute::Buffer<float> myidenticalbuffer(gpu, 10);
}
```

To set a value you can use the slice operator [] or assign a vector like such:
```cpp
mybuffer[0] = 3.1;
mybuffer = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
```

If you try to assign a vector that doesn't match the length of the buffer, it will throw a BufferSizeError. Similarly, 
when you try to assign an index that's out of range, it will throw a BufferIndexError. For more on errors, visit the [error documentation](errors.md)


To get data from the buffer, you can use MTLCompute::Buffer::getData() method or the slice operator:
```cpp
float val = mybuffer[4]; // 0.5
std::vector<float> bufferdata = mybuffer.getData();
```


================
## Textures

A MTLCompute::Texture can be a 1, 2, or 3d list that holds a predetermined number of elements.
Unfortunately, you can't use the slice operator [] set a texture value.


<br />

### 1D Textures

> [!TIP]
> There is absolutely no reason that I can think of to use
> a 1D Texture instead of a Buffer.
>
> I just added this for consistency. Use a buffer.

To create a MTLCompute::Texture1D, you specify a gpu and length.


To create a MTLCompute::Texture1D that holds 10 floats:
```cpp
#include "MTLCompute.hpp"

int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    MTLCompute::Texture1D<float> mytexture(gpu, 10);

}
```

To put data into the texture, assign a vector to it:
```cpp
mytexture = std::vector<float>(10, 1.0);
```

You can use the MTLCompute::Texture::getData() method or the slice operator to get data:
```cpp
float val = mytexture[3]; // 1.0
std::vector<float> texturedata = mytexture.getData();
```



<br />

### 2D Textures

To create a MTLCompute::Texture2D, you specify a gpu, width, and height.


To create a MTLCompute::Texture that holds 100 floats total (10x10):
```cpp
#include "MTLCompute.hpp"

int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    MTLCompute::Texture<float> mytexture(gpu, 10, 10);

}
```

To put data into the texture, assign a 2D vector to it:
```cpp
mytexture = std::vector<std::vector<float>>(10, std::vector<float>(10, 1.0))
```

You can use the MTLCompute::Texture::getData() method or the slice operator to get data:
```cpp
float val = mytexture[3][7]; // 1.0
std::vector<float> row = mytexture[5];
std::vector<std::vector<float>> texturedata = mytexture.getData();
```



<br />

### 3D Textures

To create a MTLCompute::Texture3D, you specify a gpu, width, height, and depth.


To create a MTLCompute::Texture that holds 1,000 floats total (10x10x10):
```cpp
#include "MTLCompute.hpp"

int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    MTLCompute::Texture3D<float> mytexture(gpu, 10, 10, 10);

}
```

To put data into the texture, assign a 3D vector to it:
```cpp
mytexture = std::vector<std::vector<std::vector<float>>>(10, std::vector<std::vector<float>>(10, std::vector<float>(10, 1.0)))
```


You can use the MTLCompute::Texture::getData() method or the slice operator to get data:
```cpp
float val = mytexture[3][7][2]; // 1.0
std::vector<float> row = mytexture[5][3];
std::vector<std::vector<float>> slice = mytexture[5];
std::vector<std::vector<std::vector<float>>> texturedata = mytexture.getData();
```


<br />

### TextureBuffers

> [!TIP]
> TextureBuffers are identical to Texture1Ds but have a larger
> max size (250,000,000). Again, they've only been added for consistency.
>
> Just use a buffer.

To create a MTLCompute::TextureBuffer, you specify a gpu and length.


To create a MTLCompute::TextureBuffer that holds 500,000 floats:
```cpp
#include "MTLCompute.hpp"

int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    MTLCompute::TextureBuffer<float> mytexture(gpu, 500'000);

}
```

To put data into the texture, assign a vector to it:
```cpp
mytexture = std::vector<float>(500'000, 1.0);
```

You can use the MTLCompute::TextureBuffer::getData() method or the slice operator to get data:
```cpp
float val = mytexture[300'000]; // 1.0
std::vector<float> texturedata = mytexture.getData();
```




<br />

================

# Fun extras for an easier life

## Components

Combined, textures and buffers can hold most types of data that you would need.
These types are:
- floats
- integers
  - 8 bit (char)
  - 16 bit (short)
  - 32 bit (int)
  - signed and unsigned

Buffers support user-defined types as well, as long as their size is known at compile time.

For unsigned values, there's easy types like MTLCompute::uint and MTLCompute::uchar that you can use instead
of writing out `unsigned int` or `unsigned char`. These types are for textures and buffers that hold individual values,
or single components.

Textures and buffers can also have multiple components. Metal only supports 2 components and 4 components (as far as I know).

A 2D texture with 2 float components would look like:
```cpp
MTLCompute::Texture2D<MTLCompute::float2> my2texture(gpu, 10, 10);
```

A buffer with 4 unsigned int components would look like:
```cpp
MTLCompute::Buffer<MTLCompute::uint4> my4buffer(gpu, 10, MTLCompute::ResourceStorage::Shared);
```

In MTLComputeGlobal.hpp, there's a bunch of typedef statements that define the multiple component types for your convenience. 
2 component types are just a `std::pair` and 4 component types are `std::tuple`. You could just write out 
`MTLCompute::Texture1D<std::pair<float, float>> my2texture(gpu, 10);`, but it looks nicer using the predefined types. 
It's pretty straightforward from there. It's the same as the regular
supported types, but with a 2 or 4 after the name. A std::pair of unsigned chars is a `MTLCompute::uchar2` and a std::tuple
of four floats is a `MTLCompute::float4`. When you slice a componented(?) texture, you get back a pair or tuple instead of a single value.

============

<br />

## Conversions

Luckily for you, dearest reader, I spent a ridiculous amount of time to allow smooth and easy conversions
between types. If you think of all three main list-like types (Buffers, 1D Textures, horrid TextureBuffers) as a
big triangle, you can now go from any point on the triangle to any other point, provided the sizes aren't too
large.

There are two ways to do the conversions: with the equals assignment operator and a regular function.
The function is the most readable and understandable way, in my opinion. Use the functions
`.toBuffer()`, `.toTexture1D()`, and `.toTextureBuffer()`. Probably the easiest thing to remember. Just 
'to' and then what you want to convert to.
Examples:


The next way is even easier, just assign what you want to convert from to an instance of what
you want to convert to. You couldn't possibly mess this up (not a challenge). `thing_to_convert_to = thing_to_convert_from`
So easy!

I was smart with this and made sure to add in size checks because each of the classes
has a different limit on their maximum size. If you try to cram a 2,000,000 long buffer
into a Texture1D (which can only handle 16,384), it will throw an error. The maximum sizes
for all of the classes can be found in the `MTLComputeGlobals.hpp` file, except for the buffer max size. Buffers have no max size. At least, I can't find
anything that says they do.


Examples
```cpp
int main() {
    
    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    MTLCompute::Buffer<int> buffer(gpu, 10);

    // i want to turn this into a 1d texture for some stupid irrelevant reason!
    // create the texture
    MTLCompute::Texture1D<int> texture(gpu, 10);
    // use '.toTexture1D()'
    texture = buffer.toTexture1D(); // wow


    // golly gee whiz i want that in a texturebuffer now!!
    // create the texturebuffer
    MTLCompute::TextureBuffer<int> texturebuffer(gpu, 10):
    // assign
    texturebuffer = texture // literally so easy
}
```
================

# Sending to the GPU

## Kernel

A MTLCompute::Kernel is how you tell the GPU what to do with the data you give it. You load a compiled Metal Shading Language file (.metallib)
and select a function to use. It's probably the simplest class in this whole project.


To create a MTLCompute::Kernel with a "default.metallib" compiled metal library:
```cpp
#include "MTLCompute.hpp"

int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    MTLCompute::Kernel kernel(gpu, "default.metallib");

}
```


To get all the functions in a library as a vector of strings, use MTLCompute::Kernel::getFunctionNames() like so:
```cpp
std::vector<std::string> names = kernel.getFunctionNames();
for (auto name : names) {
    std::cout << name << std::endl;
}
```


If that library had a function called "add_arrays" that you wanted to use, you would then add:
```cpp
kernel.useFunction("add_arrays");
```

================

## CommandManager
A MTLCompute::CommandManager is the way you really 'talk' to the gpu. You specify the kernel and then load buffers
and textures at certain indecies that correspond to your MSL (Metal Shading Language) function.


To create a new MTLCompute::CommandManager with the previous kernel:
```cpp
#include "MTLCompute.hpp"

int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    MTLCompute::Kernel kernel(gpu, "default.metallib");

    MTLCompute::CommandManager<float> manager(gpu, kernel);

}
```



The index lists for buffers and textures are seperate. This means that you can load a buffer at index 0 and a
texture at index 0 as well (under the previous condition). These indecies should correspond to the indecies in your MSL function.


Then to load a buffer at index 0 and a texture at index 0:
```cpp
manager.loadBuffer(mybuffer, 0);
manager.loadTexture(mytexture, 0);
```

BREAKING NEWS!! I summoned all of my brainpower and used variadic templates to allow the CommandManager
to have multiple types! You can now put a buffer of floats and a 2d texture of int4s into the same CommandManager! :o
This is revolutionary because I'm stupid and variadic templates are hard.

```cpp
#include "MTLCompute.hpp"

int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    MTLCompute::Kernel kernel(gpu, "default.metallib");

    MTLCompute::CommandManager<float, int4, uchar2> manager(gpu, kernel);

    MTLCompute::Buffer<float> mybuffer(gpu, 10);
    MTLCompute::Texture2D<int4> mytexture(gpu, 10, 10);
    MTLCompute::TextureBuffer<uchar4> uglytexturebuffer(gpu, 10); // i hate texturebuffers >:(


    manager.loadBuffer(mybuffer, 0);
    manager.loadTexture(mytexture, 0);
    manager.loadTexture(uglytexturebuffer, 1);

    // this is crazyyyyyy!!!!!
}
```

# Special GPU Class {#special-gpu-class}

There's a second way to use the API for people who want to do even less. The MTLCompute::GPU class is the simplest
possible way to use the API. It's not flexible at all, but it's really easy to use.

Create a MTLCompute::GPU object:
```cpp
#include "MTLCompute.hpp"

int main() {

    MTLCompute::GPU gpu;

}
```

You load a compiled metal library and a function with the GPU object like this:
```cpp
gpu.loadKernel("default.metallib", "add_arrays");
```

You don't even have to use buffers or textures, just use vectors:
```cpp
std::vector<float> a = {1.0, 2.0, 3.0, 4.0, 5.0};
std::vector<std::vector<float>> b(5, std::vector<float>(5, 1.0));

gpu.loadArray(a, 0);
gpu.loadMatrix(b, 0);
```

Then you run the function with the data you loaded:
```cpp
gpu.runKernel();
```

And finally, get the data back:
```cpp
std::vector<float> result = gpu.getArray(0);
std::vector<std::vector<float>> resultmatrix = gpu.getMatrix(0);
```

If you really don't want to do any work and are fine with maybe crashing your program because
I didn't really pay attention to pointers and lifetimes when making this one, this is for you!
Enjoy <3

The end!