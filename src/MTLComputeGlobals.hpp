#include <iostream>
#include <vector>
#include <map>
#include <typeinfo>
#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include "Metal.hpp"

#pragma once

// alias for std::vector<T>
template< typename T >
using vec = std::vector<T>;

// alias for std::vector<std::vector<T>>
template< typename T >
using vec2 = std::vector<std::vector<T>>;

// alias for std::vector<std::vector<std::vector<T>>>
template< typename T >
using vec3 = std::vector<std::vector<std::vector<T>>>;


namespace MTLCompute {

    constexpr int MAX_BUFFERS = 31;
    constexpr int MAX_TEXTURES = 128;
    constexpr long MAX_TEXTURE1D_SIZE = 16384;
    constexpr long MAX_TEXTURE2D_SIZE = 16384;
    constexpr long MAX_TEXTURE3D_SIZE = 2048;
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

    // Metal does not support 3 component types
    
    typedef std::tuple<uchar, uchar, uchar, uchar> uchar4;
    typedef std::tuple<ushort, ushort, ushort, ushort> ushort4;
    typedef std::tuple<uint, uint, uint, uint> uint4;
    typedef std::tuple<char, char, char, char> char4;
    typedef std::tuple<short, short, short, short> short4;
    typedef std::tuple<int, int, int, int> int4;
    typedef std::tuple<float, float, float, float> float4;

    template<typename T>
    inline int numComponents() {
        
        if (typeid(T) == typeid(uchar2)) return 2;
        if (typeid(T) == typeid(ushort2)) return 2;
        if (typeid(T) == typeid(uint2)) return 2;
        if (typeid(T) == typeid(char2)) return 2;
        if (typeid(T) == typeid(short2)) return 2;
        if (typeid(T) == typeid(int2)) return 2;
        if (typeid(T) == typeid(float2)) return 2;

        if (typeid(T) == typeid(uchar4)) return 4;
        if (typeid(T) == typeid(ushort4)) return 4;
        if (typeid(T) == typeid(uint4)) return 4;
        if (typeid(T) == typeid(char4)) return 4;
        if (typeid(T) == typeid(short4)) return 4;
        if (typeid(T) == typeid(int4)) return 4;
        if (typeid(T) == typeid(float4)) return 4;

        return 1;
    }

}