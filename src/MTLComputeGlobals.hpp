#pragma once

#include <tuple>
#include <typeinfo>
#include <map>
#include <vector>
#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include "Metal.hpp" // IWYU pragma: keep

// alias for std::vector<T>
template <typename T> using vec = std::vector<T>;

// alias for std::vector<std::vector<T>>
template <typename T> using vec2 = std::vector<std::vector<T>>;

// alias for std::vector<std::vector<std::vector<T>>>
template <typename T> using vec3 = std::vector<std::vector<std::vector<T>>>;

namespace MTLCompute {

constexpr int MAX_BUFFERS = 31;
constexpr int MAX_TEXTURES = 128;
constexpr long MAX_TEXTURE1D_SIZE = 16384;
constexpr long MAX_TEXTURE2D_SIZE = 16384;
constexpr long MAX_TEXTURE3D_SIZE = 2048;
constexpr long MAX_TEXTUREBUFFER_SIZE = 256'000'000;
// i cant find the max buffer size

enum class ResourceStorage {
    Shared = 0,   // MTL::ResourceStorageModeShared
    Managed = 16, // MTL::ResourceStorageModeManaged
    Private = 32, // MTL::ResourceStorageModePrivate
};

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef std::pair<uchar, uchar> uchar2;
typedef std::pair<ushort, ushort> ushort2;
typedef std::pair<uint, uint> uint2;
typedef std::pair<char, char> char2;
typedef std::pair<short, short> short2;
typedef std::pair<int, int> int2;
typedef std::pair<float, float> float2;

// Metal does not support 3 component types (from what i can tell)

typedef std::tuple<uchar, uchar, uchar, uchar> uchar4;
typedef std::tuple<ushort, ushort, ushort, ushort> ushort4;
typedef std::tuple<uint, uint, uint, uint> uint4;
typedef std::tuple<char, char, char, char> char4;
typedef std::tuple<short, short, short, short> short4;
typedef std::tuple<int, int, int, int> int4;
typedef std::tuple<float, float, float, float> float4;

template <typename T>
constexpr inline int numComponents() {

    if (typeid(T) == typeid(uchar2))
        return 2;
    if (typeid(T) == typeid(ushort2))
        return 2;
    if (typeid(T) == typeid(uint2))
        return 2;
    if (typeid(T) == typeid(char2))
        return 2;
    if (typeid(T) == typeid(short2))
        return 2;
    if (typeid(T) == typeid(int2))
        return 2;
    if (typeid(T) == typeid(float2))
        return 2;

    if (typeid(T) == typeid(uchar4))
        return 4;
    if (typeid(T) == typeid(ushort4))
        return 4;
    if (typeid(T) == typeid(uint4))
        return 4;
    if (typeid(T) == typeid(char4))
        return 4;
    if (typeid(T) == typeid(short4))
        return 4;
    if (typeid(T) == typeid(int4))
        return 4;
    if (typeid(T) == typeid(float4))
        return 4;

    return 1;
}

/*
Apple Clang Version 17.0.0

typeid name is implementation defined and may differ between
compilers.

typeid(your type here).name()
uchar = h
ushort = t
uint = j
char = c
short = s
int = i
float = f
uchar2 = NSt3__14pairIhhEE
ushort2 = NSt3__14pairIttEE
uint2 = NSt3__14pairIjjEE
char2 = NSt3__14pairIccEE
short2 = NSt3__14pairIssEE
int2 = NSt3__14pairIiiEE
float2 = NSt3__14pairIffEE
uchar4 = NSt3__15tupleIJhhhhEEE
ushort4 = NSt3__15tupleIJttttEEE
uint4 = NSt3__15tupleIJjjjjEEE
char4 = NSt3__15tupleIJccccEEE
short4 = NSt3__15tupleIJssssEEE
int4 = NSt3__15tupleIJiiiiEEE
float4 = NSt3__15tupleIJffffEEE
*/

inline const char* demangleTypeName(const char *type) {
    if (strcmp(type, typeid(uchar).name()) == 0)
        return "uchar";
    if (strcmp(type, typeid(ushort).name()) == 0)
        return "ushort";
    if (strcmp(type, typeid(uint).name()) == 0)
        return "uint";
    if (strcmp(type, typeid(char).name()) == 0)
        return "char";
    if (strcmp(type, typeid(short).name()) == 0)
        return "short";
    if (strcmp(type, typeid(int).name()) == 0)
        return "int";
    if (strcmp(type, typeid(float).name()) == 0)
        return "float";

    if (strcmp(type, typeid(uchar2).name()) == 0)
        return "uchar2";
    if (strcmp(type, typeid(ushort2).name()) == 0)
        return "ushort2";
    if (strcmp(type, typeid(uint2).name()) == 0)
        return "uint2";
    if (strcmp(type, typeid(char2).name()) == 0)
        return "char2";
    if (strcmp(type, typeid(short2).name()) == 0)
        return "short2";
    if (strcmp(type, typeid(int2).name()) == 0)
        return "int2";
    if (strcmp(type, typeid(float2).name()) == 0)
        return "float2";

    if (strcmp(type, typeid(uchar4).name()) == 0)
        return "uchar4";
    if (strcmp(type, typeid(ushort4).name()) == 0)
        return "ushort4";
    if (strcmp(type, typeid(uint4).name()) == 0)
        return "uint4";
    if (strcmp(type, typeid(char4).name()) == 0)
        return "char4";
    if (strcmp(type, typeid(short4).name()) == 0)
        return "short4";
    if (strcmp(type, typeid(int4).name()) == 0)
        return "int4";
    if (strcmp(type, typeid(float4).name()) == 0)
        return "float4";

    return "unknown";
}

inline std::map<const char*, MTL::PixelFormat> pixelFormatMap = {
    {"uchar", MTL::PixelFormatR8Uint},
    {"ushort", MTL::PixelFormatR16Uint},
    {"uint", MTL::PixelFormatR32Uint},
    {"char", MTL::PixelFormatR8Sint},
    {"short", MTL::PixelFormatR16Sint},
    {"int", MTL::PixelFormatR32Sint},
    {"float", MTL::PixelFormatR32Float},
    {"uchar2", MTL::PixelFormatRG8Uint},
    {"ushort2", MTL::PixelFormatRG16Uint},
    {"uint2", MTL::PixelFormatRG32Uint},
    {"char2", MTL::PixelFormatRG8Sint},
    {"short2", MTL::PixelFormatRG16Sint},
    {"int2", MTL::PixelFormatRG32Sint},
    {"float2", MTL::PixelFormatRG32Float},
    {"uchar4", MTL::PixelFormatRGBA8Uint},
    {"ushort4", MTL::PixelFormatRGBA16Uint},
    {"uint4", MTL::PixelFormatRGBA32Uint},
    {"char4", MTL::PixelFormatRGBA8Sint},
    {"short4", MTL::PixelFormatRGBA16Sint},
    {"int4", MTL::PixelFormatRGBA32Sint},
    {"float4", MTL::PixelFormatRGBA32Float}
};

} // namespace MTLCompute