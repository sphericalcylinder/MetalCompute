#include "MTLComputeBuffer.hpp"
#include "MTLComputeGlobals.hpp"
#include "MTLComputeKernel.hpp"
#include "MTLComputeTexture.hpp"

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

            using Texture = std::variant<Texture1D<T>, Texture2D<T>, Texture3D<T>>;

            vec<Buffer<T>> buffers = vec<Buffer<T>>(MAX_BUFFERS); ///< The buffers
            vec<Texture> textures = vec<Texture>(MAX_TEXTURES); ///< The 2D textures
            int bufferlength = -1; ///< The length of the buffers
            int texwidth = -1; ///< The width of the textures
            int texheight = -1; ///< The height of the textures
            int texdepth = -1; ///< The depth of the textures

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
             * @brief Load a 1d texture into the CommandManager
             *
             * Takes in a 1d texture and an index and adds the texture to an internal array
             *
             * @param texture The 1d texture to load
             * @param index The index to load the texture into
             *
            */
            void loadTexture(Texture1D<T> texture, int index) {
                if (this->texwidth == -1)
                    this->texwidth = texture.getWidth();
                
                if (this->texwidth != texture.getWidth())
                    throw std::invalid_argument("Texture sizes do not match");


                this->textures[index] = texture;
            }

            /**
             * @brief Load a 2d texture into the CommandManager
             *
             * Takes in a 2d texture and an index and adds the texture to an internal array
             *
             * @param texture The 2d texture to load
             * @param index The index to load the texture into
             *
            */
            void loadTexture(Texture2D<T> texture, int index) {
                if (this->texwidth == -1)
                    this->texwidth = texture.getWidth();
                
                if (this->texheight == -1) 
                    this->texheight = texture.getHeight();

                if (this->texwidth != texture.getWidth() || this->texheight != texture.getHeight())
                    throw std::invalid_argument("Texture sizes do not match");
                

                this->textures[index] = texture;
            }

            /**
             * @brief Load a 3d texture into the CommandManager
             *
             * Takes in a 3d texture and an index and adds the texture to an internal array
             *
             * @param texture The 3d texture to load
             * @param index The index to load the texture into
             *
            */
            void loadTexture(Texture3D<T> texture, int index) {
                if (this->texwidth == -1)
                    this->texwidth = texture.getWidth();
                
                if (this->texheight == -1) 
                    this->texheight = texture.getHeight();

                if (this->texdepth == -1) 
                    this->texdepth = texture.getDepth();

                if (this->texwidth != texture.getWidth() || this->texheight != texture.getHeight() || this->texdepth != texture.getDepth())
                    throw std::invalid_argument("Texture sizes do not match");

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

                    Texture tex = this->textures[i];
                    if (std::holds_alternative<Texture1D<T>>(tex)) {
                        Texture1D<T> texture = std::get<Texture1D<T>>(tex);

                        if (texture.getWidth() == this->texwidth && texture.getTexture() != nullptr) {
                            this->commandEncoder->setTexture(texture.getTexture(), i);

                            if (!usingtextures)
                                usingtextures = true;
                        }
                    }
                    else if (std::holds_alternative<Texture2D<T>>(tex)) {
                        Texture2D<T> texture = std::get<Texture2D<T>>(tex);

                        if (texture.getWidth() == this->texwidth && texture.getHeight() == this->texheight
                                && texture.getTexture() != nullptr) {
                            this->commandEncoder->setTexture(texture.getTexture(), i);
                            
                            if (!usingtextures)
                                usingtextures = true;
                        }
                    } else if (std::holds_alternative<Texture3D<T>>(tex)) {
                        Texture3D<T> texture = std::get<Texture3D<T>>(tex);

                        if (texture.getWidth() == this->texwidth && texture.getHeight() == this->texheight
                                && texture.getDepth() == this->texdepth && texture.getTexture() != nullptr) {
                            this->commandEncoder->setTexture(texture.getTexture(), i);
                            
                            if (!usingtextures)
                                usingtextures = true;
                        }
                    }
                }


                for (int i = MAX_BUFFERS; i < MAX_TEXTURES; i++) {
                    Texture tex = this->textures[i];
                    if (std::holds_alternative<Texture1D<T>>(tex)) {
                        Texture1D<T> texture = std::get<Texture1D<T>>(tex);

                        if (texture.getWidth() == this->texwidth && texture.getTexture() != nullptr) {
                            this->commandEncoder->setTexture(texture.getTexture(), i);

                            if (!usingtextures)
                                usingtextures = true;
                        }
                    }
                    else if (std::holds_alternative<Texture2D<T>>(tex)) {
                        Texture2D<T> texture = std::get<Texture2D<T>>(tex);

                        if (texture.getWidth() == this->texwidth && texture.getHeight() == this->texheight
                                && texture.getTexture() != nullptr) {
                            this->commandEncoder->setTexture(texture.getTexture(), i);
                            
                            if (!usingtextures)
                                usingtextures = true;
                        }
                    } else if (std::holds_alternative<Texture3D<T>>(tex)) {
                        Texture3D<T> texture = std::get<Texture3D<T>>(tex);

                        if (texture.getWidth() == this->texwidth && texture.getHeight() == this->texheight
                                && texture.getDepth() == this->texdepth && texture.getTexture() != nullptr) {
                            this->commandEncoder->setTexture(texture.getTexture(), i);
                            
                            if (!usingtextures)
                                usingtextures = true;
                        }
                    }
                }

                // Calculate the grid size and thread group size
                MTL::Size threadsPerThreadgroup;
                threadsPerThreadgroup.width = this->pipeline->threadExecutionWidth();
                threadsPerThreadgroup.height = this->pipeline->maxTotalThreadsPerThreadgroup() / threadsPerThreadgroup.width;
                threadsPerThreadgroup.depth = 1;

                MTL::Size threadsPerGrid;

                int currentwidth = this->texwidth;
                int currentheight = (this->texheight == -1 ? 1 : this->texheight);
                int currentdepth = (this->texdepth == -1 ? 1 : this->texdepth);

                if (usingbuffers && usingtextures) {
                    if (this->bufferlength > currentwidth)
                        threadsPerGrid = MTL::Size::Make(this->bufferlength, currentheight, currentdepth);
                    else
                        threadsPerGrid = MTL::Size::Make(currentwidth, currentheight, currentdepth);

                } else if (usingbuffers && !usingtextures) {
                    threadsPerGrid = MTL::Size::Make(this->bufferlength, 1, 1);

                } else if (!usingbuffers && usingtextures) {
                    threadsPerGrid = MTL::Size::Make(currentwidth, currentheight, currentdepth);
                    
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
                this->buffers = vec<Buffer<T>>(MAX_BUFFERS);
                this->bufferlength = -1;
            }

            /**
             * @brief reset the textures and cached width and height
             *
            */
            void resetTextures() {
                this->textures.clear();
                this->textures = vec<Texture>(MAX_TEXTURES);
                this->texwidth = -1;
                this->texheight = -1;
                this->texdepth = -1;
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
            vec<Buffer<T>>& getBuffers() {
                return this->buffers;
            }

            /**
             * @brief Get a loaded 1D texture
             *
             * @return Texture1D<T> The texture
             *
            */
            Texture1D<T>& getTexture1D(int index) {
                if (!std::holds_alternative<Texture1D<T>>(this->textures[index]))
                    throw std::invalid_argument("Texture is not a 1D texture");
                return std::get<Texture1D<T>>(this->textures[index]);
            }

            /**
             * @brief Get a loaded 2D texture
             *
             * @return Texture2D<T> The texture
             *
            */
            Texture2D<T>& getTexture2D(int index) {
                if (!std::holds_alternative<Texture2D<T>>(this->textures[index]))
                    throw std::invalid_argument("Texture is not a 2D texture");
                return std::get<Texture2D<T>>(this->textures[index]);
            }

            /**
             * @brief Get a loaded 3D texture
             *
             * @return Texture3D<T> The texture
             *
            */
            Texture3D<T>& getTexture3D(int index) {
                if (!std::holds_alternative<Texture3D<T>>(this->textures[index]))
                    throw std::invalid_argument("Texture is not a 3D texture");
                return std::get<Texture3D<T>>(this->textures[index]);
            }

    };

}