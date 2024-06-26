#include <iostream>
#include <map>
#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include "Metal.hpp"

#pragma once

namespace MTLCompute {

    constexpr int MAX_BUFFERS = 31;
    constexpr int MAX_TEXTURES = 128;
    constexpr long MAX_TEXTURE_SIZE = 16384;
    // i cant find the max buffer size

    enum class ResourceStorage {
        Shared = MTL::ResourceStorageModeShared,
        Managed = MTL::ResourceStorageModeManaged,
        Private = MTL::ResourceStorageModePrivate
    };

    enum class TextureType {
        uint8 = MTL::PixelFormatR8Uint,
        uint16 = MTL::PixelFormatR16Uint,
        uint32 = MTL::PixelFormatR32Uint,

        int8 = MTL::PixelFormatR8Sint,
        int16 = MTL::PixelFormatR16Sint,
        int32 = MTL::PixelFormatR32Sint,

        float32 = MTL::PixelFormatR32Float,
    };

    inline std::map<TextureType, size_t> TextureTypeSizes = {
        {TextureType::uint8, sizeof(uint8_t)},
        {TextureType::uint16, sizeof(uint16_t)},
        {TextureType::uint32, sizeof(uint32_t)},
        {TextureType::int8, sizeof(int8_t)},
        {TextureType::int16, sizeof(int16_t)},
        {TextureType::int32, sizeof(int32_t)},
        {TextureType::float32, sizeof(float_t)}
    };

}