#pragma once

#include "MTLComputeBuffer.hpp" // IWYU pragma: export
#include "MTLComputeCommandManager.hpp" // IWYU pragma: export
#include "MTLComputeErrors.hpp" // IWYU pragma: export
#include "MTLComputeGlobals.hpp" // IWYU pragma: export
#include "MTLComputeKernel.hpp" // IWYU pragma: export
#include "MTLComputeTexture.hpp" // IWYU pragma: export

#ifdef USING_SINGLE_CLASS
#error "Cannot include both MTLCompute.hpp and MTLComputeGPU.hpp"
#endif

#define USING_MULTIPLE_CLASSES
