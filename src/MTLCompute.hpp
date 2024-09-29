#include "MTLComputeBuffer.hpp"
#include "MTLComputeCommandManager.hpp"
#include "MTLComputeErrors.hpp"
#include "MTLComputeGlobals.hpp"
#include "MTLComputeKernel.hpp"
#include "MTLComputeTexture.hpp"

#pragma once

#ifdef USING_SINGLE_CLASS
#error "Cannot include both MTLCompute.hpp and MTLComputeGPU.hpp"
#endif

#define USING_MULTIPLE_CLASSES
