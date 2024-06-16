MetalCompute Documentation  {#mainpage}
================

# MetalCompute
MetalCompute is an API to make GPU compute calls easier. Usually, on a mac, you would have to use
Objective-C or Swift to use the Metal API. I don't know either and don't really want to put the effort 
into learning them. Thankfully, Apple released [metal-cpp](https://github.com/bkaradzic/metal-cpp), a C++ API that
calls Objective-C Metal functions. It's really useful, but if you want to do some GPU calculations for a simple project,
it's just too much. That's why I'm making this, a API on top of another API to use Metal for compute. If you have
suggestions or find a bug, file an issue on the [repo](https://github.com/sphericalcylinder/MetalCompute) for this
project.

================

# Compilation {#compilation}
This is a header only API, you just need to include the directory where MTLCompute.hpp is located after cmake install.
You also have to add the
`-framework Foundation -framework Metal -framework MetalKit` flags or there will be problems.

================

# Usage
[TOC]
## Regular Usage {#reguse}

### Starting {#starting}
To use the API regularly, like in the buffer and texture
examples, include the `MTLCompute.hpp` header and add the [other flags](#compilation).

Currently, you still have to access the raw MTL::Device object because I haven't really made a good wrapper for that yet. There is
a MTLCompute::GPU class, but it's not for wrapping the MTL::Device. The start of your file should look like this:
```cpp
#include "MTLCompute.hpp"

int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

}
```

That's really the only work you have to do with the metal-cpp API directly. Now, you can use MTLCompute in place of metal-cpp. Mostly.


================
### Buffers {#buffers}

A MTLCompute::Buffer is a 1D list that holds a predetermined number of elements. It's really simple. To create
a MTLCompute::Buffer, you specify a gpu, length, and a MTLCompute::ResourceStorage option. When choosing your
MTLCompute::ResourceStorage value, refer to the 
[Metal Best Practices](https://developer.apple.com/library/archive/documentation/3DDrawing/Conceptual/MTLBestPracticesGuide/ResourceOptions.html#//apple_ref/doc/uid/TP40016642-CH17-SW1)
site under "Choose an Appropriate Resource Storage Mode (macOS)". Most commonly, you'll use MTLCompute::ResourceStorage::Shared.


To create a MTLCompute::Buffer that holds 10 floats:
```cpp
#include "MTLCompute.hpp"

int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    MTLCompute::Buffer<float> mybuffer(gpu, 10, MTLCompute::ResourceStorage::Shared);

}
```

To set a value you can use the slice operator [] or assign a vector like such:
```cpp
mybuffer[0] = 3.1;
mybuffer = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
```

If you try to assign a vector that doesn't match the length of the buffer, it will throw an invalid argument error. The same
happens when you try to assign an index that's out of range.


To get data from the buffer, you can use MTLCompute::Buffer::getData() method or the slice operator:
```cpp
float val = mybuffer[4]; // 0.5
std::vector<float> bufferdata = mybuffer.getData();
```


================
### Textures {#textures}

A MTLCompute::Texture can be a 1, 2, or 3d list that holds a predetermined number of elements. When creating a texture, 
you can optionally specify a MTLCompute::TextureType. The TextureType is the type of data going into the texture. 
The options are int and uint 8, 16, and 32, and float32. Usually, you can just ignore that argument and it will be inferred.
You can't use the slice operator [] set a texture value.

#### 1D Textures

To create a MTLCompute::Texture1D, you specify a gpu, length, and MTLCompute::TextureType option.


To create a MTLCompute::Texture1D that holds 10 floats:
```cpp
#include "MTLCompute.hpp"

int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    MTLCompute::Texture1D<float> mytexture(gpu, 10, MTLCompute::TextureType::float32);
    // or MTLCompute::Texture1D<float> mytexture(gpu, 10);

}
```

To put data into the texture, assign a vector to it:
```cpp
mytexture = std::vector<float>(10, 1.0);
```

You can't use the slice operator [] set a texture value.

You can use the MTLCompute::Texture::getData() method or the slice operator to get data:
```cpp
float val = mytexture[3]; // 1.0
std::vector<float> texturedata = mytexture.getData();
```

<br />

#### 2D Textures

To create a MTLCompute::Texture2D, you specify a gpu, width, height, and MTLCompute::TextureType option.


To create a MTLCompute::Texture that holds 100 floats (10x10):
```cpp
#include "MTLCompute.hpp"

int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    MTLCompute::Texture<float> mytexture(gpu, 10, 10, MTLCompute::TextureType::float32);
    // or MTLCompute::Texture<float> mytexture(gpu, 10, 10);

}
```

To put data into the texture, assign a 2D vector to it:
```cpp
mytexture = std::vector<std::vector<float>>(10, std::vector<float>(10, 1.0))
```
You can't use the slice operator [] set a texture value.

You can use the MTLCompute::Texture::getData() method or the slice operator to get data:
```cpp
float val = mytexture[3][7]; // 1.0
std::vector<float> row = mytexture[5];
std::vector<std::vector<float>> texturedata = mytexture.getData();
```

<br />

#### 3D Textures

To create a MTLCompute::Texture3D, you specify a gpu, width, height, depth, and MTLCompute::TextureType option.


To create a MTLCompute::Texture that holds 1000 floats (10x10x10):
```cpp
#include "MTLCompute.hpp"

int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    MTLCompute::Texture3D<float> mytexture(gpu, 10, 10, 10, MTLCompute::TextureType::float32);

   // or MTLCompute::Texture3D<float> mytexture(gpu, 10, 10, 10);

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

================


### Kernel {#kernel}

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

### CommandManager {#commandmanager}
A MTLCompute::CommandManager is the way you really 'talk' to the gpu. You specify the kernel and then load buffers
and textures at certain indecies that correspond to your MSL (Metal Shading Language) function.


To create a new MTLCompute::CommandManager with the previous kernel:
```cpp
#include "MTLCompute.hpp"

int main() {

    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    MTLCompute::Kernel kernel(gpu, "default.metallib");

    MTLCompute::CommandManager manager(gpu, kernel);

}
```

The index lists for buffers and textures are seperate. This means that you can load a buffer at index 0 and a
texture at index 0 as well. These indecies should correspond to the indecies in your MSL function.


Then to load a buffer at index 0 and a texture at index 0:
```cpp
manager.loadBuffer(mybuffer, 0);
manager.loadTexture(mytexture, 0);
```




## Special Usage {#specuse}

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

The end!