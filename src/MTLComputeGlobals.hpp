#include <iostream>
#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include "Foundation/Foundation.hpp"
#include "Metal/Metal.hpp"

#pragma once

namespace MTLCompute {

    enum class ResourceStorage {
        Shared = MTL::ResourceStorageModeShared,
        Managed = MTL::ResourceStorageModeManaged,
        Private = MTL::ResourceStorageModePrivate
    };

    enum class TextureType {
        uint8 = MTL::PixelFormatR8Uint,
        uint16 = MTL::PixelFormatR16Uint,
        uint32 = MTL::PixelFormatR32Uint,

        sint8 = MTL::PixelFormatR8Sint,
        sint16 = MTL::PixelFormatR16Sint,
        sint32 = MTL::PixelFormatR32Sint,

        float16 = MTL::PixelFormatR16Float,
        float32 = MTL::PixelFormatR32Float,
        
        norm8 = MTL::PixelFormatR8Unorm,
        norm16 = MTL::PixelFormatR16Unorm,
    };

}