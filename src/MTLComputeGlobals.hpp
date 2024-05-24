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

}