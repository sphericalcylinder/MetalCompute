#include <iostream>
#include <vector>
#include <map>
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
        Shared = 0, // MTL::ResourceStorageModeShared
        Managed = 16, // MTL::ResourceStorageModeManaged
        Private = 32, // MTL::ResourceStorageModePrivate
    };

    enum class TextureItemType {
        uint8 = MTL::PixelFormatR8Uint,     // 13
        uint16 = MTL::PixelFormatR16Uint,   // 23
        uint32 = MTL::PixelFormatR32Uint,   // 53

        int8 = MTL::PixelFormatR8Sint,      // 14
        int16 = MTL::PixelFormatR16Sint,    // 24
        int32 = MTL::PixelFormatR32Sint,    // 54

        float32 = MTL::PixelFormatR32Float, // 55
    };

    inline std::map<TextureItemType, size_t> TextureTypeSizes = {
        {TextureItemType::uint8, sizeof(uint8_t)},
        {TextureItemType::uint16, sizeof(uint16_t)},
        {TextureItemType::uint32, sizeof(uint32_t)},
        {TextureItemType::int8, sizeof(int8_t)},
        {TextureItemType::int16, sizeof(int16_t)},
        {TextureItemType::int32, sizeof(int32_t)},
        {TextureItemType::float32, sizeof(float_t)}
    };

}