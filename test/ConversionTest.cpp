#include "MTLCompute.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


MTL::Device *gpu = MTL::CreateSystemDefaultDevice();


// Buffer to Texture1D

TEST_CASE("Test Buffer to Texture1D") {
    MTLCompute::Buffer<int> buffer(gpu, 10);
    REQUIRE_NOTHROW(buffer = vec<int>(10, 1));

    MTLCompute::Texture1D<int> texture(gpu, 10);
    REQUIRE_NOTHROW(texture = buffer.toTexture1D()); // toTexture1D() function

    REQUIRE(texture.getWidth() == 10);
    REQUIRE(texture.getComponents() == 1);
    REQUIRE(texture.getData() == vec<int>(10, 1));

    MTLCompute::Texture1D<int> texture2(gpu, 10);
    REQUIRE_NOTHROW(texture2 = buffer); // = operator override

    REQUIRE(texture2.getWidth() == 10);
    REQUIRE(texture2.getComponents() == 1);
    REQUIRE(texture2.getData() == vec<int>(10, 1));
}

TEST_CASE("Test oversize Buffer to Texture1D") {
    MTLCompute::Buffer<int> buffer(gpu, MTLCompute::MAX_TEXTURE1D_SIZE + 1);
    buffer = vec<int>(MTLCompute::MAX_TEXTURE1D_SIZE + 1, 1);

    REQUIRE_THROWS_AS_MESSAGE(buffer.toTexture1D(), MTLCompute::ConversionSizeError, (
        "Invalid size for conversion to Texture1D (must be less than + " + std::to_string(MTLCompute::MAX_TEXTURE1D_SIZE) + ")"
    ));

    MTLCompute::Texture1D<int> texture(gpu, MTLCompute::MAX_TEXTURE1D_SIZE);

    REQUIRE_THROWS_AS_MESSAGE(texture = buffer, MTLCompute::ConversionSizeError, (
        "Invalid size for conversion to Texture1D (ust be less than + " + std::to_string(MTLCompute::MAX_TEXTURE1D_SIZE) + ")"
    ));
}

TEST_CASE("Test wrong type Buffer to Texture1D") {
    struct imaginary_type { int a = 1; };
    MTLCompute::Buffer<imaginary_type> buffer(gpu, 10);
    buffer = vec<imaginary_type>(10, imaginary_type());

    MTLCompute::Texture1D<int> texture;
    REQUIRE_THROWS_AS_MESSAGE(buffer.toTexture1D(), MTLCompute::ConversionTypeError, (
        "Invalid type for conversion to Texture1D"
    ));
}



// Texture1D to Buffer

TEST_CASE("Test Texture1D to Buffer") {
    MTLCompute::Texture1D<int> texture(gpu, 10);
    REQUIRE_NOTHROW(texture = vec<int>(10, 1));

    MTLCompute::Buffer<int> buffer(gpu, 10);
    REQUIRE_NOTHROW(buffer = texture.toBuffer()); // toBuffer() function

    REQUIRE(buffer.getLength() == 10);
    REQUIRE(buffer.getData() == vec<int>(10, 1));

    MTLCompute::Buffer<int> buffer2(gpu, 10);
    REQUIRE_NOTHROW(buffer2 = texture); // = operator override

    REQUIRE(buffer2.getLength() == 10);
    REQUIRE(buffer2.getData() == vec<int>(10, 1));
}



// TextureBuffer to Buffer

TEST_CASE("Test TextureBuffer to Buffer") {
    MTLCompute::TextureBuffer<int> texture(gpu, 10);
    REQUIRE_NOTHROW(texture = vec<int>(10, 1));

    MTLCompute::Buffer<int> buffer(gpu, 10);
    REQUIRE_NOTHROW(buffer = texture.toBuffer()); // toBuffer() function

    REQUIRE(buffer.getLength() == 10);
    REQUIRE(buffer.getData() == vec<int>(10, 1));

    MTLCompute::Buffer<int> buffer2(gpu, 10);
    REQUIRE_NOTHROW(buffer2 = texture); // = operator override

    REQUIRE(buffer2.getLength() == 10);
    REQUIRE(buffer2.getData() == vec<int>(10, 1));
}


// Buffer to TextureBuffer

TEST_CASE("Test Buffer to TextureBuffer") {
    MTLCompute::Buffer<int> buffer(gpu, 10);
    REQUIRE_NOTHROW(buffer = vec<int>(10, 1));

    MTLCompute::TextureBuffer<int> texturebuffer(gpu, 10);
    REQUIRE_NOTHROW(texturebuffer = buffer.toTextureBuffer()); // toTextureBuffer() function

    REQUIRE(texturebuffer.getWidth() == 10);
    REQUIRE(texturebuffer.getComponents() == 1);
    REQUIRE(texturebuffer.getData() == vec<int>(10, 1));

    MTLCompute::TextureBuffer<int> texturebuffer2(gpu, 10);
    REQUIRE_NOTHROW(texturebuffer2 = buffer); // = operator override

    REQUIRE(texturebuffer2.getWidth() == 10);
    REQUIRE(texturebuffer2.getComponents() == 1);
    REQUIRE(texturebuffer2.getData() == vec<int>(10, 1));
}

TEST_CASE("Test oversize Buffer to TextureBuffer") {
    MTLCompute::Buffer<int> buffer(gpu, MTLCompute::MAX_TEXTUREBUFFER_SIZE + 1);
    buffer = vec<int>(MTLCompute::MAX_TEXTUREBUFFER_SIZE + 1, 1);

    REQUIRE_THROWS_AS_MESSAGE(buffer.toTextureBuffer(), MTLCompute::ConversionSizeError, (
        "Invalid size for conversion to TextureBuffer (must be less than + " + std::to_string(MTLCompute::MAX_TEXTUREBUFFER_SIZE) + ")"
    ));
}

TEST_CASE("Test wrong type Buffer to TextureBuffer") {
    struct imaginary_type { int a = 1; };
    MTLCompute::Buffer<imaginary_type> buffer(gpu, 10);
    buffer = vec<imaginary_type>(10, imaginary_type());

    MTLCompute::TextureBuffer<int> texture;
    REQUIRE_THROWS_AS_MESSAGE(buffer.toTextureBuffer(), MTLCompute::ConversionTypeError, (
        "Invalid type for conversion to TextureBuffer"
    ));
}


// Texture1D to TextureBuffer

TEST_CASE("Test Texture1D to TextureBuffer") {
    MTLCompute::Texture1D<int> texture(gpu, 10);
    REQUIRE_NOTHROW(texture = vec<int>(10, 1));

    MTLCompute::TextureBuffer<int> texturebuffer(gpu, 10);
    REQUIRE_NOTHROW(texturebuffer = texture.toTextureBuffer()); // toTextureBuffer() function

    REQUIRE(texturebuffer.getWidth() == 10);
    REQUIRE(texturebuffer.getComponents() == 1);
    REQUIRE(texturebuffer.getData() == vec<int>(10, 1));

    MTLCompute::TextureBuffer<int> texturebuffer2(gpu, 10);
    REQUIRE_NOTHROW(texturebuffer2 = texture); // = operator override

    REQUIRE(texturebuffer2.getWidth() == 10);
    REQUIRE(texturebuffer2.getComponents() == 1);
    REQUIRE(texturebuffer2.getData() == vec<int>(10, 1));
}



// TextureBuffer to Texture1D

TEST_CASE("Test TextureBuffer to Texture1D") {
    MTLCompute::TextureBuffer<int> texturebuffer(gpu, 10);
    REQUIRE_NOTHROW(texturebuffer = vec<int>(10, 1));

    MTLCompute::Texture1D<int> texture(gpu, 10);
    REQUIRE_NOTHROW(texture = texturebuffer.toTexture1D()); // toTexture1D() function

    REQUIRE(texture.getWidth() == 10);
    REQUIRE(texture.getComponents() == 1);
    REQUIRE(texture.getData() == vec<int>(10, 1));

    MTLCompute::Texture1D<int> texture2(gpu, 10);
    REQUIRE_NOTHROW(texture2 = texturebuffer); // = operator override

    REQUIRE(texture2.getWidth() == 10);
    REQUIRE(texture2.getComponents() == 1);
    REQUIRE(texture2.getData() == vec<int>(10, 1));
}

TEST_CASE("Test oversize TextureBuffer to Texture1D") {
    MTLCompute::TextureBuffer<int> texturebuffer(gpu, MTLCompute::MAX_TEXTURE1D_SIZE + 1);
    texturebuffer = vec<int>(MTLCompute::MAX_TEXTURE1D_SIZE + 1, 1);

    REQUIRE_THROWS_AS_MESSAGE(texturebuffer.toTexture1D(), MTLCompute::ConversionSizeError, (
        "Invalid size for conversion to Texture1D (must be less than + " + std::to_string(MTLCompute::MAX_TEXTURE1D_SIZE) + ")"
    ));
}