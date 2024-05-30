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
            Kernel *kernel; ///< The kernel object
            MTL::ComputePipelineState *pipeline; ///< The Metal compute pipeline state object
            MTL::CommandQueue *commandQueue; ///< The Metal command queue object
            MTL::CommandBuffer *commandBuffer; ///< The Metal command buffer object
            MTL::ComputeCommandEncoder *commandEncoder; ///< The Metal compute command encoder object

            std::vector<Buffer<T>> buffers = std::vector<Buffer<T>>(MAX_BUFFERS); ///< The buffers
            std::vector<Texture<T>> textures = std::vector<Texture<T>>(MAX_TEXTURES); ///< The textures
            int bufferlength = -1; ///< The length of the buffers
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
            CommandManager(MTL::Device *gpu, MTLCompute::Kernel *kernel) {
                this->gpu = gpu;
                this->kernel = kernel;
                this->pipeline = this->kernel->getPLS();

                this->commandQueue = this->gpu->newCommandQueue();

            }

            /**
             * @brief Default constructor for the CommandManager class
             *
            */
            CommandManager() = default;

            /**
             * @brief Destructor for the CommandManager class
             *
             * Releases the command queue
             *
            */
            ~CommandManager() {
                this->commandQueue->autorelease();
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
             * @brief Load a texture into the CommandManager
             *
             * Takes in a texture and an index and adds the texture to an internal array
             *
             * @param texture The texture to load
             * @param index The index to load the texture into
             *
            */
            void loadTexture(Texture<T> texture, int index) {
                if (this->texwidth == -1 && this->texheight == -1) {
                    this->texwidth = texture.getWidth();
                    this->texheight = texture.getHeight();
                    if (this->texwidth > MAX_TEXTURE_SIZE || this->texheight > MAX_TEXTURE_SIZE) {
                        throw std::invalid_argument("Texture size too large, max size is 16384");
                    }
                } else if (this->texwidth != texture.getWidth() || this->texheight != texture.getHeight()) {
                    std::cout << this->texwidth << " " << this->texheight << std::endl;
                    std::cout << texture.getTexture()->width() << " " << texture.getTexture()->height() << std::endl;
                    throw std::invalid_argument("Texture sizes do not match");
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
                if (this->kernel->getPLS() != this->pipeline) {
                    // Refresh the pipeline if it has changed
                    this->pipeline = this->kernel->getPLS();
                }

                // Create a new command buffer and command encoder
                this->commandBuffer = this->commandQueue->commandBuffer();
                this->commandEncoder = this->commandBuffer->computeCommandEncoder();
                this->commandEncoder->setComputePipelineState(this->pipeline);
                bool usingbuffers = false;
                bool usingtextures = false;

                // Load the buffers and textures into the commandEncoder
                for (int i = 0; i < MAX_BUFFERS; i++) {
                    if (buffers[i].length == this->bufferlength && buffers[i].getBuffer() != nullptr) {
                        this->commandEncoder->setBuffer(buffers[i].getBuffer(), 0, i);
                        usingbuffers = true;
                    }
                    if (textures[i].getWidth() == this->texwidth && textures[i].getHeight() == this->texheight
                            && textures[i].getTexture() != nullptr) {
                        this->commandEncoder->setTexture(textures[i].getTexture(), i);
                        usingtextures = true;
                    }
                }
                for (int i = MAX_BUFFERS; i < MAX_TEXTURES; i++) {
                    if (textures[i].getWidth() == this->texwidth && textures[i].getHeight() == this->texheight
                            && textures[i].getTexture() != nullptr) {
                        this->commandEncoder->setTexture(textures[i].getTexture(), i);
                        usingtextures = true;
                    }
                }

                // Calculate the grid size and thread group size
                MTL::Size threadsPerThreadgroup;
                threadsPerThreadgroup.width = this->pipeline->threadExecutionWidth();
                threadsPerThreadgroup.height = this->pipeline->maxTotalThreadsPerThreadgroup() / threadsPerThreadgroup.width;
                threadsPerThreadgroup.depth = 1;

                MTL::Size threadsPerGrid;
                if (usingbuffers && usingtextures) {
                    if (this->bufferlength > this->texwidth)
                        threadsPerGrid = MTL::Size::Make(this->bufferlength, this->texheight, 1);
                    else
                        threadsPerGrid = MTL::Size::Make(this->texwidth, this->texheight, 1);
                } else if (usingbuffers && !usingtextures) {
                    threadsPerGrid = MTL::Size::Make(this->bufferlength, 1, 1);

                } else if (!usingbuffers && usingtextures) {
                    threadsPerGrid = MTL::Size::Make(this->texwidth, this->texheight, 1);
                    
                } else {
                    throw std::invalid_argument("No buffers or textures loaded");
                }

                // Use dispatchThreads NOT dispatchThreadgroups
                this->commandEncoder->dispatchThreads(threadsPerGrid, threadsPerThreadgroup);
                this->commandEncoder->endEncoding();
                this->commandBuffer->commit();
                this->commandBuffer->waitUntilCompleted();

                // Release the command encoder and command buffer
                this->commandEncoder->release();
                this->commandBuffer->release();
            }

            /**
             * @brief reset the buffers and cached length
             *
            */
            void resetBuffers() {
                this->buffers.clear();
                this->buffers = std::vector<Buffer<T>>(MAX_BUFFERS);
                this->bufferlength = -1;
            }

            /**
             * @brief reset the textures and cached width and height
             *
            */
            void resetTextures() {
                this->textures.clear();
                this->textures = std::vector<Texture<T>>(MAX_TEXTURES);
                this->texwidth = -1;
                this->texheight = -1;
            }

            /**
             * @brief reset the buffers and textures
             *
             * Calls CommandManager::resetBuffers and CommandManager::resetTextures
             *
            */
            void reset() {
                this->resetBuffers();
                this->resetTextures();
            }

            /**
             * @brief Get the GPU device
             *
             * @return MTL::Device* The GPU device
             *
            */
            MTL::Device *getGPU() {
                return this->gpu;
            }

            /**
             * @brief Get the kernel object
             *
             * @return Kernel* The kernel object
             *
            */
            Kernel *getKernel() {
                return this->kernel;
            }

            /**
             * @brief Get the loaded buffers
             *
             * @return std::vector<Buffer<T>> The buffers
             *
            */
            std::vector<Buffer<T>>& getBuffers() {
                return this->buffers;
            }

            /**
             * @brief Get the loaded textures
             *
             * @return std::vector<Texture<T>> The textures
             *
            */
            std::vector<Texture<T>>& getTextures() {
                return this->textures;
            }

    };

}