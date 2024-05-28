#include "MTLComputeBuffer.hpp"
#include "MTLComputeGlobals.hpp"
#include "MTLComputeKernel.hpp"
#include "MTLComputeTexture.hpp"
#include <iostream>
#include <vector>

#pragma once

namespace MTLCompute {

    template< typename T>
    class CommandManager {

        private:
            MTL::Device *gpu; ///< The Metal device object
            MTL::ComputePipelineState *pipeline; ///< The Metal compute pipeline state object
            MTL::CommandQueue *commandQueue; ///< The Metal command queue object
            MTL::CommandBuffer *commandBuffer; ///< The Metal command buffer object
            MTL::ComputeCommandEncoder *commandEncoder; ///< The Metal compute command encoder object
            std::vector<Buffer<T>> buffers = std::vector<Buffer<T>>(50); ///< The buffers
            int bufferlength = -1; ///< The length of the buffers

        public:

            /**
             * @brief Constructor for the CommandManager class
             *
             * Takes in the GPU device and the kernel object and creates a new CommandManager.
             * Also takes in the target buffer type with a template parameter.
             *
             * @param gpu The GPU device
             * @param kernel The kernel object
             *
            */
            CommandManager<T>(MTL::Device *gpu, MTLCompute::Kernel &kernel) {
                this->gpu = gpu;
                this->pipeline = kernel.getPLS();

                this->commandQueue = this->gpu->newCommandQueue();
                //this->commandBuffer = this->commandQueue->commandBuffer();
                //this->commandEncoder = this->commandBuffer->computeCommandEncoder();

                //this->commandEncoder->setComputePipelineState(this->pipeline);

            }

            /**
             * @brief Destructor for the CommandManager class
             *
             * Releases the command queue
             *
            */
            ~CommandManager() {
                //this->commandBuffer->release();
                this->commandQueue->release();
            }

            /**
             * @brief Load a buffer into the CommandManager
             *
             * Takes in a buffer and an index and adds the buffer to an internal array
             *
             * @param buffer The buffer to load
             * @param index The index to load the buffer into
             *
            */
            void loadBuffer(Buffer<T> buffer, int index) {
                if (this->bufferlength == -1) {
                    this->bufferlength = buffer.length;
                } else if (this->bufferlength != buffer.length) {
                    throw std::invalid_argument("Buffer lengths do not match");
                }

                this->buffers[index] = buffer;
            }

            /**
             * @brief Dispatch the kernel
             *
             * Creates new command buffer and command encoder objects,
             * adds the specified buffers at the correct positons, and dispatches the kernel
             *
            */
            void dispatch() {

                this->commandBuffer = this->commandQueue->commandBuffer();
                this->commandEncoder = this->commandBuffer->computeCommandEncoder();
                this->commandEncoder->setComputePipelineState(this->pipeline);

                for (int i = 0; i < buffers.size(); i++) {
                    if (buffers[i].length == this->bufferlength) {
                        this->commandEncoder->setBuffer(buffers[i].getBuffer(), 0, i);
                    }
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

                this->commandEncoder->release();
                this->commandBuffer->release();
            }

            /*void refresh() {
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
            }*/

    };

    template< typename T>
    class TextureCommandManager {

        private:
            MTL::Device *gpu; ///< The Metal device object
            MTL::ComputePipelineState *pipeline; ///< The Metal compute pipeline state object
            MTL::CommandQueue *commandQueue; ///< The Metal command queue object
            MTL::CommandBuffer *commandBuffer; ///< The Metal command buffer object
            MTL::ComputeCommandEncoder *commandEncoder; ///< The Metal compute command encoder object
            std::vector<Texture<T>> textures = std::vector<Texture<T>>(50); ///< The textures
            int texwidth = -1; ///< The width of the textures
            int texheight = -1; ///< The height of the textures

        public:

            /**
             * @brief Constructor for the CommandManager class
             *
             * Takes in the GPU device and the kernel object and creates a new CommandManager.
             * Also takes in the target buffer type with a template parameter.
             *
             * @param gpu The GPU device
             * @param kernel The kernel object
             *
            */
            TextureCommandManager<T>(MTL::Device *gpu, MTLCompute::Kernel &kernel) {
                this->gpu = gpu;
                this->pipeline = kernel.getPLS();

                this->commandQueue = this->gpu->newCommandQueue();

            }

            /**
             * @brief Destructor for the CommandManager class
             *
             * Releases the command queue
             *
            */
            ~TextureCommandManager() {
                //this->commandBuffer->release();
                this->commandQueue->release();
            }

            /**
             * @brief Load a buffer into the CommandManager
             *
             * Takes in a buffer and an index and adds the buffer to an internal array
             *
             * @param buffer The buffer to load
             * @param index The index to load the buffer into
             *
            */
            void loadTexture(Texture<T> texture, int index) {
                if (this->texwidth == -1 && this->texheight == -1) {
                    this->texwidth = texture.getWidth();
                    this->texheight = texture.getHeight();
                } else if (this->texwidth != texture.getTexture()->width() || this->texheight != texture.getTexture()->height()) {
                    std::cout << this->texwidth << " " << this->texheight << std::endl;
                    std::cout << texture.getTexture()->width() << " " << texture.getTexture()->height() << std::endl;
                    throw std::invalid_argument("Buffer lengths do not match");
                }

                this->textures[index] = texture;
            }

            /**
             * @brief Dispatch the kernel
             *
             * Creates new command buffer and command encoder objects,
             * adds the specified buffers at the correct positons, and dispatches the kernel
             *
            */
            void dispatch() {

                this->commandBuffer = this->commandQueue->commandBuffer();
                this->commandEncoder = this->commandBuffer->computeCommandEncoder();
                this->commandEncoder->setComputePipelineState(this->pipeline);

                for (int i = 0; i < textures.size(); i++) {
                    if (textures[i].getWidth() == this->texwidth && textures[i].getHeight() == this->texheight) {
                        this->commandEncoder->setTexture(textures[i].getTexture(), i);
                    }
                }

                // Calculate the grid size and thread group size
                MTL::Size threadgroupsize = MTL::Size(10, 10, 1);

                MTL::Size threadgroupcount;
                threadgroupcount.width = (this->texwidth + threadgroupsize.width - 1) / threadgroupsize.width;
                threadgroupcount.height = (this->texheight + threadgroupsize.height - 1) / threadgroupsize.height;
                threadgroupcount.depth = 1;


                this->commandEncoder->dispatchThreadgroups(threadgroupcount, threadgroupsize);
                this->commandEncoder->endEncoding();
                this->commandBuffer->commit();
                this->commandBuffer->waitUntilCompleted();

                this->commandEncoder->release();
                this->commandBuffer->release();
            }
    };

}