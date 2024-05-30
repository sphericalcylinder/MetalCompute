#include "MTLComputeGlobals.hpp"
#include "MTLComputeBuffer.hpp"
#include "MTLComputeKernel.hpp"
#include "MTLComputeCommandManager.hpp"
#include "MTLComputeTexture.hpp"

#pragma once

#ifdef USING_SINGLE_CLASS
#error "Cannot include both MTLCompute.hpp and MTLComputeGPU.hpp"
#endif

#define USING_MULTIPLE_CLASSES

/**
 * @file MTLCompute.hpp
 * @brief The main header file for the MTLCompute library
 *
*/
