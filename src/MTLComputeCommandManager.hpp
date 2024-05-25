#include "MTLComputeBuffer.hpp"
#include "MTLComputeGlobals.hpp"
#include "MTLComputeKernel.hpp"
#include "Metal/MTLComputePipeline.hpp"
#include <iostream>
#include <vector>

#pragma once

namespace MTLCompute {


    class CommandManager {

        private:
            MTL::Device *gpu; ///< The Metal device object
            MTL::ComputePipelineState *pipeline; ///< The Metal compute pipeline state object
            MTL::CommandQueue *commandQueue; ///< The Metal command queue object
            MTL::CommandBuffer *commandBuffer; ///< The Metal command buffer object
            MTL::ComputeCommandEncoder *commandEncoder; ///< The Metal compute command encoder object
            int bufferlength = -1; ///< The length of the buffer
            bool bufferindecies[50]; ///< The buffer indecies
            bool stale = false; ///< The stale flag
            bool endedencoding = false; ///< The ended encoding flag

        public:

            CommandManager(MTL::Device *gpu, MTLCompute::Kernel &kernel) {
                this->gpu = gpu;
                this->pipeline = kernel.getPLS();

                this->commandQueue = this->gpu->newCommandQueue();
                this->commandBuffer = this->commandQueue->commandBuffer();
                this->commandEncoder = this->commandBuffer->computeCommandEncoder();

                this->commandEncoder->setComputePipelineState(this->pipeline);

                for (int i = 0; i < 50; i++) {
                    this->bufferindecies[i] = false;
                }
            }

            ~CommandManager() {
                this->commandEncoder->endEncoding();
                this->commandEncoder->release();
                this->commandBuffer->release();
                this->commandQueue->release();
            }

            template< typename T>
            void loadBuffer(Buffer<T> buffer, int index) {
                if ((buffer.length != this->bufferlength) && (this->bufferlength != -1)) {
                    throw std::invalid_argument("Buffer length does not match");
                }
                if (this->stale) {
                    throw std::logic_error("Command manager is stale");
                }

                if (this->bufferindecies[index]) {
                    throw std::invalid_argument("Buffer index already in use");
                }

                this->commandEncoder->setBuffer(buffer.getBuffer(), 0, index);
                this->bufferindecies[index] = true;

                if (this->bufferlength == -1)
                    this->bufferlength = buffer.length;
            }

            void dispatch() {
                if (this->stale) {
                    throw std::logic_error("Command manager is stale");
                }

                // Calculate the grid size and thread group size
                MTL::Size gridsize = MTL::Size(10, 1, 1);
                NS::UInteger maxthreads = this->pipeline->maxTotalThreadsPerThreadgroup();
                if (maxthreads > bufferlength) {
                    maxthreads = bufferlength;
                }

                MTL::Size threadgroupsize = MTL::Size(1, 1, 1);
                this->commandEncoder->dispatchThreadgroups(gridsize, threadgroupsize);
                this->commandEncoder->endEncoding();
                this->commandBuffer->commit();
                this->commandBuffer->waitUntilCompleted();

                this->stale = true;
                this->endedencoding = true;
            }

            void refresh() {
                if (!this->endedencoding) {
                    this->commandEncoder->endEncoding();
                    this->endedencoding = true;
                }
                this->commandEncoder->release();
                this->commandBuffer->release();
                this->commandQueue->release();

                this->commandQueue = this->gpu->newCommandQueue();
                this->commandBuffer = this->commandQueue->commandBuffer();
                this->commandEncoder = this->commandBuffer->computeCommandEncoder();

                this->commandEncoder->setComputePipelineState(this->pipeline);
                this->bufferlength = -1;
                for (int i = 0; i < 50; i++) {
                    this->bufferindecies[i] = false;
                }

                this->stale = false;
                this->endedencoding = false;
            }

    };

}