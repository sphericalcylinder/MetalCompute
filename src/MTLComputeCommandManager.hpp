#pragma once ///< Include guard

#include "MTLComputeBuffer.hpp"
#include "MTLComputeErrors.hpp"
#include "MTLComputeGlobals.hpp"
#include "MTLComputeKernel.hpp"
#include "MTLComputeTexture.hpp"

namespace MTLCompute {

/**
 * @brief CommandManager class to manage arguments and dispatch kernels
 *
 * This class is used to manage the arguments for a kernel and dispatch the
 * kernel to the GPU. It takes in a kernel object and a GPU device object and
 * creates a new command queue and command buffer. It also takes in the target
 * types with a variadic template.
 *
 * @tparam Types The data types of the buffers and textures
 *
 */
template <typename... Types> class CommandManager {

  private:
    MTL::Device *gpu;                    ///< The Metal device object
    Kernel *kernel;                      ///< The kernel object
    MTL::ComputePipelineState *pipeline; ///< The Metal compute pipeline state object
    MTL::CommandQueue *commandQueue;     ///< The Metal command queue object
    MTL::CommandBuffer *commandBuffer;   ///< The Metal command buffer object
    MTL::ComputeCommandEncoder
        *commandEncoder; ///< The Metal compute command encoder object

    template <typename T>
    using Texture = std::variant<std::monostate, Texture1D<T>, Texture2D<T>,
                                 Texture3D<T>>; ///< Variant type for the textures so they
                                                ///< can be 1, 2, or 3D

    std::tuple<vec<Buffer<Types>>...> buffers;   ///< The buffers
    std::tuple<vec<Texture<Types>>...> textures; ///< The textures

    int bufferlength = -1; ///< The length of the buffers
    int texwidth = -1;     ///< The width of the textures
    int texheight = -1;    ///< The height of the textures
    int texdepth = -1;     ///< The depth of the textures

    void checkTextureIndex(int index) {
        if (index < 0 || index >= MAX_TEXTURES)
            throw Error::CommandManagerIndexError("Index out of range");
    }

  public:
    /**
     * @brief Constructor for the CommandManager class
     *
     * Takes in the GPU device and the kernel object and creates a new
     * CommandManager. Also takes in the target buffer type with a template
     * parameter.
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
        ((std::get<vec<Buffer<Types>>>(buffers) = vec<Buffer<Types>>(MAX_BUFFERS)), ...);
        ((std::get<vec<Texture<Types>>>(textures) = vec<Texture<Types>>(MAX_TEXTURES)),
         ...);
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
    ~CommandManager() { this->commandQueue->autorelease(); }

    /**
     * @brief Load a buffer into the CommandManager
     *
     * Takes in a buffer and an index and adds the buffer to an internal array
     *
     * @param buffer The buffer to load
     * @param index The index to load the buffer into
     *
     */
    template <typename T> void loadBuffer(Buffer<T> buffer, int index) {
        static_assert((std::is_same_v<T, Types> || ...),
                      "Type T must be one of the template parameters");

        if (this->bufferlength == -1)
            this->bufferlength = buffer.getLength();

        if (this->bufferlength != buffer.getLength())
            throw Error::CommandManagerItemSizeError("Buffer sizes do not match");

        // this->buffers[index] = buffer;
        std::get<vec<Buffer<T>>>(buffers)[index] = buffer;
    }

    /**
     * @brief Load a 1d texture into the CommandManager
     *
     * Takes in a 1d texture and an index and adds the texture to an internal
     * array
     *
     * @param texture The 1d texture to load
     * @param index The index to load the texture into
     *
     */
    template <typename T> void loadTexture(Texture1D<T> texture, int index) {
        static_assert((std::is_same_v<T, Types> || ...),
                      "Type T must be one of the template parameters");
        if (this->texwidth == -1)
            this->texwidth = texture.getWidth();

        if (this->texwidth != texture.getWidth())
            throw Error::CommandManagerItemSizeError("Texture sizes do not match");

        std::get<vec<Texture<T>>>(textures)[index] = texture;
    }

    /**
     * @brief Load a 2d texture into the CommandManager
     *
     * Takes in a 2d texture and an index and adds the texture to an internal
     * array
     *
     * @param texture The 2d texture to load
     * @param index The index to load the texture into
     *
     */
    template <typename T> void loadTexture(Texture2D<T> texture, int index) {
        static_assert((std::is_same_v<T, Types> || ...),
                      "Type T must be one of the template parameters");
        if (this->texwidth == -1)
            this->texwidth = texture.getWidth();

        if (this->texheight == -1)
            this->texheight = texture.getHeight();

        if (this->texwidth != texture.getWidth() ||
            this->texheight != texture.getHeight())
            throw Error::CommandManagerItemSizeError("Texture sizes do not match");

        std::get<vec<Texture<T>>>(textures)[index] = texture;
    }

    /**
     * @brief Load a 3d texture into the CommandManager
     *
     * Takes in a 3d texture and an index and adds the texture to an internal
     * array
     *
     * @param texture The 3d texture to load
     * @param index The index to load the texture into
     *
     */
    template <typename T> void loadTexture(Texture3D<T> texture, int index) {
        static_assert((std::is_same_v<T, Types> || ...),
                      "Type T must be one of the template parameters");
        if (this->texwidth == -1)
            this->texwidth = texture.getWidth();

        if (this->texheight == -1)
            this->texheight = texture.getHeight();

        if (this->texdepth == -1)
            this->texdepth = texture.getDepth();

        if (this->texwidth != texture.getWidth() ||
            this->texheight != texture.getHeight() ||
            this->texdepth != texture.getDepth())
            throw Error::CommandManagerItemSizeError("Texture sizes do not match");

        std::get<vec<Texture<T>>>(textures)[index] = texture;
    }

    /**
     * @brief Dispatch the kernel
     *
     * Creates new command buffer and command encoder objects,
     * adds the specified buffers at the correct positons, and dispatches the
     * kernel
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

        // Set buffers for each type  (lambda function)
        auto setBuffers = [&]<typename T>(std::vector<Buffer<T>> &typeBuffers) {
            for (size_t i = 0; i < typeBuffers.size(); i++) {
                if (typeBuffers[i].getBuffer() != nullptr) {
                    if (typeBuffers[i].getBuffer() != nullptr &&
                        typeBuffers[i].getLength() == this->bufferlength) {
                        commandEncoder->setBuffer(typeBuffers[i].getBuffer(), 0, i);
                    }
                    usingbuffers = true;
                }
            }
        };

        // Set textures for each type (lambda function)
        auto setTextures = [&]<typename T>(std::vector<Texture<T>> &typeTextures) {
            for (size_t i = 0; i < typeTextures.size(); i++) {
                if (!std::holds_alternative<std::monostate>(typeTextures[i])) {
                    auto t = typeTextures[i];
                    if (std::holds_alternative<Texture1D<T>>(t)) {
                        const auto &texture = std::get<Texture1D<T>>(t);
                        if (texture.getWidth() == texwidth &&
                            texture.getTexture() != nullptr) {
                            commandEncoder->setTexture(texture.getTexture(), i);
                        }
                    } else if (std::holds_alternative<Texture2D<T>>(t)) {
                        const auto &texture = std::get<Texture2D<T>>(t);
                        if (texture.getWidth() == texwidth &&
                            texture.getHeight() == texheight &&
                            texture.getTexture() != nullptr) {
                            commandEncoder->setTexture(texture.getTexture(), i);
                        }
                    } else if (std::holds_alternative<Texture3D<T>>(t)) {
                        const auto &texture = std::get<Texture3D<T>>(t);
                        if (texture.getWidth() == texwidth &&
                            texture.getHeight() == texheight &&
                            texture.getDepth() == texdepth &&
                            texture.getTexture() != nullptr) {
                            commandEncoder->setTexture(texture.getTexture(), i);
                        }
                    }

                    usingtextures = true;
                }
            }
        };

        // Apply to all types
        std::apply([&](auto &...args) { (setBuffers(args), ...); }, buffers);
        std::apply([&](auto &...args) { (setTextures(args), ...); }, textures);

        // Calculate the grid size and thread group size
        MTL::Size threadsPerThreadgroup;
        threadsPerThreadgroup.width = this->pipeline->threadExecutionWidth();
        threadsPerThreadgroup.height =
            this->pipeline->maxTotalThreadsPerThreadgroup() / threadsPerThreadgroup.width;
        threadsPerThreadgroup.depth = 1;

        MTL::Size threadsPerGrid;

        int currentwidth = this->texwidth;
        int currentheight = (this->texheight == -1 ? 1 : this->texheight);
        int currentdepth = (this->texdepth == -1 ? 1 : this->texdepth);

        if (usingbuffers && usingtextures) {
            if (this->bufferlength > currentwidth)
                threadsPerGrid =
                    MTL::Size::Make(this->bufferlength, currentheight, currentdepth);
            else
                threadsPerGrid =
                    MTL::Size::Make(currentwidth, currentheight, currentdepth);

        } else if (usingbuffers && !usingtextures) {
            threadsPerGrid = MTL::Size::Make(this->bufferlength, 1, 1);

        } else if (!usingbuffers && usingtextures) {
            threadsPerGrid = MTL::Size::Make(currentwidth, currentheight, currentdepth);

        } else {
            throw Error::CommandManagerLoadError("No buffers or textures loaded");
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
        ((std::get<vec<Buffer<Types>>>(buffers) = vec<Buffer<Types>>(MAX_BUFFERS)), ...);
        this->bufferlength = -1;
    }

    /**
     * @brief reset the textures and cached width and height
     *
     */
    void resetTextures() {
        ((std::get<vec<Texture<Types>>>(textures) = vec<Texture<Types>>(MAX_TEXTURES)),
         ...);
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
    MTL::Device *getGPU() { return this->gpu; }

    /**
     * @brief Get the kernel object
     *
     * @return Kernel* The kernel object
     *
     */
    Kernel *getKernel() { return this->kernel; }

    /**
     * @brief Get the loaded buffers
     *
     * @return std::vector<Buffer<T>> The buffers
     *
     */
    template <typename T> vec<Buffer<T>> &getBuffers() {
        static_assert((std::is_same_v<T, Types> || ...),
                      "Type T must be one of the template parameters");
        return std::get<vec<Buffer<T>>>(buffers);
    }

    /**
     * @brief Get a loaded 1D texture
     *
     * @return Texture1D<T> The texture
     *
     */
    template <typename T> Texture1D<T> &getTexture1D(int index) {
        static_assert((std::is_same_v<T, Types> || ...),
                      "Type T must be one of the template parameters");
        this->checkTextureIndex(index);
        auto &tex = std::get<vec<Texture<T>>>(textures)[index];
        if (!std::holds_alternative<Texture1D<T>>(tex))
            throw Error::CommandManagerIndexError("No 1D texture at index " +
                                                  std::to_string(index));
        return std::get<Texture1D<T>>(tex);
    }

    /**
     * @brief Get a loaded 2D texture
     *
     * @return Texture2D<T> The texture
     *
     */
    template <typename T> Texture2D<T> &getTexture2D(int index) {
        static_assert((std::is_same_v<T, Types> || ...),
                      "Type T must be one of the template parameters");
        this->checkTextureIndex(index);
        auto &tex = std::get<vec<Texture<T>>>(textures)[index];
        if (!std::holds_alternative<Texture2D<T>>(tex))
            throw Error::CommandManagerIndexError("No 2D texture at index " +
                                                  std::to_string(index));
        return std::get<Texture2D<T>>(tex);
    }

    /**
     * @brief Get a loaded 3D texture
     *
     * @return Texture3D<T> The texture
     *
     */
    template <typename T> Texture3D<T> &getTexture3D(int index) {
        static_assert((std::is_same_v<T, Types> || ...),
                      "Type T must be one of the template parameters");
        this->checkTextureIndex(index);
        auto &tex = std::get<vec<Texture<T>>>(textures)[index];
        if (!std::holds_alternative<Texture3D<T>>(tex))
            throw Error::CommandManagerIndexError("No 3D texture at index " +
                                                  std::to_string(index));
        return std::get<Texture3D<T>>(tex);
    }
};

} // namespace MTLCompute