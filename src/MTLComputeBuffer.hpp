#pragma once

#include "MTLComputeErrors.hpp"
#include "MTLComputeGlobals.hpp"
#include "MTLComputeTexture.hpp"

namespace MTLCompute {

template <typename T> class Buffer {
  private:
    MTL::Device *gpu;                        ///< The Metal device object
    MTL::Buffer *buffer;                     ///< The Metal buffer object
    MTLCompute::ResourceStorage storageMode; ///< The storage mode of the buffer
    bool freed = false;                      ///< Whether the buffer has been freed
    size_t length;                           ///< The length of the buffer
    size_t itemsize;                         ///< The size of each item in the buffer

    void swap(Buffer &buffer) noexcept {
        using std::swap;
        swap(this->gpu, buffer.gpu);
        swap(this->buffer, buffer.buffer);
        swap(this->length, buffer.length);
        swap(this->itemsize, buffer.itemsize);
        swap(this->storageMode, buffer.storageMode);
    }

    /**
     * @brief Check if the buffer has been freed
     *
     */
    void checkFreed() const {
        if (this->freed)
            throw Error::BufferFreeError("Buffer already freed");
    }

    /**
     * @brief Check if the index is in bounds (0 to this->length)
     *
     * @param index The index to check
     *
     */
    void checkIndex(size_t index) const {
        if (index >= this->length)
            throw Error::BufferIndexError("Index out of bounds");
    }

    /**
     * @brief Check if the size of the data matches the buffer size
     *
     * @param size The size of the data
     *
     */
    void checkDataSize(size_t size) const {
        if (size != this->length)
            throw Error::BufferSizeError("Data size does not match buffer size");
    }

    /**
     * @brief Check if the buffer has been initialized
     *
     */
    void checkInit() const {
        if (this->length == -1)
            throw Error::BufferInitError("Buffer not initialized");
    }

  public:
    /**
     * @brief Constructor for the Buffer class
     *
     * Takes in the length of the buffer, the GPU device,
     * and the storage mode and creates a new buffer
     *
     * @param length The length of the buffer
     * @param gpu The GPU device
     * @param storageMode The storage mode of the buffer
     *
     */
    Buffer(MTL::Device *gpu, size_t length, ResourceStorage storageMode) {
        this->gpu = gpu;
        this->length = length;
        this->itemsize = sizeof(T);
        this->storageMode = storageMode;
        this->buffer = gpu->newBuffer(length * itemsize,
                                      static_cast<MTL::ResourceOptions>(storageMode));
        this->buffer->retain();
    }

    /**
     * @brief Constructor for the Buffer class
     *
     * Takes in the length of the buffer and the GPU device
     * and creates a new buffer with the default storage mode
     *
     * @param length The length of the buffer
     * @param gpu The GPU device
     *
     */
    Buffer(MTL::Device *gpu, size_t length) {
        this->gpu = gpu;
        this->length = length;
        this->itemsize = sizeof(T);
        this->storageMode = MTLCompute::ResourceStorage::Shared;
        this->buffer = gpu->newBuffer(length * itemsize,
                                      static_cast<MTL::ResourceOptions>(storageMode));
        this->buffer->retain();
    }

    /**
     * @brief Copy constructor for the Buffer class
     *
     * Constructs a new buffer from an existing buffer
     *
     * @param other The buffer to copy
     *
     */
    Buffer(const Buffer &other) {
        this->gpu = other.gpu;
        this->length = other.length;
        this->itemsize = other.itemsize;
        this->storageMode = other.storageMode;
        this->buffer = other.buffer;
    }

    /**
     * @brief Default constructor for the Buffer class
     *
     * Creates a new empty buffer object
     *
     */
    Buffer() {
        this->gpu = nullptr;
        this->length = -1;
        this->itemsize = -1;
        this->storageMode = MTLCompute::ResourceStorage::Shared;
        this->buffer = nullptr;
    }

    /**
     * @brief Free the buffer object
     *
     * Calls autorelease on the buffer object and
     * sets the freed flag to true
     *
     */
    constexpr void free() {
        if (!this->freed) {
            this->buffer->autorelease();
            this->freed = true;
        }
    }

    /**
     * @brief Destructor for the Buffer class
     *
     * Calls the free function to release the buffer object
     *
     */
    ~Buffer() { free(); }

    /**
     * @brief Get the contents of the buffer
     *
     * @return T* The contents of the buffer
     *
     */
    T *contents() {
        this->checkFreed();

        return (T *)this->buffer->contents();
    }

    /**
     * @brief Overload the [] operator to get the value at an index
     *
     * @param index The index to get the value from
     * @return T The value at the index
     *
     */
    T operator[](size_t index) const {
        this->checkFreed();
        this->checkIndex(index);
        return ((T *)this->buffer->contents())[index];
    }

    /**
     * @brief Overload the [] operator to set the value at an index
     *
     * @param index The index to set the value at
     * @return T The value to set at the index
     *
     */
    T &operator[](size_t index) {
        this->checkFreed();
        this->checkIndex(index);
        return ((T *)this->buffer->contents())[index];
    }

    /**
     * @brief Overload the = operator to set buffer contents from a vector
     *
     * @param data The data to set the buffer contents to
     *
     */
    void operator=(vec<T> data) {
        this->checkFreed();
        this->checkDataSize(data.size());

        // flawfinder: ignore
        memcpy(this->buffer->contents(), data.data(), this->length * this->itemsize);

        if (this->storageMode == MTLCompute::ResourceStorage::Managed) {
            this->buffer->didModifyRange(NS::Range(0, this->length * this->itemsize));
        }
    }

    /**
     * @brief Overload the = operator to set buffer contents from a Texture1D
     * object
     *
     * @param texture The Texture1D object to set the buffer contents to
     *
     */
    void operator=(Texture1D<T> texture) {
        this->checkFreed();
        this->checkDataSize(texture.getWidth());
        vec<T> data = texture.getData();

        // flawfinder: ignore
        memcpy(this->buffer->contents(), data.data(), this->length * this->itemsize);

        if (this->storageMode == MTLCompute::ResourceStorage::Managed) {
            this->buffer->didModifyRange(NS::Range(0, this->length * this->itemsize));
        }
    }

    void operator=(TextureBuffer<T> texture) {
        this->checkFreed();
        this->checkDataSize(texture.getWidth());
        vec<T> data = texture.getData();

        // flawfinder: ignore
        memcpy(this->buffer->contents(), data.data(), this->length * this->itemsize);

        if (this->storageMode == MTLCompute::ResourceStorage::Managed) {
            this->buffer->didModifyRange(NS::Range(0, this->length * this->itemsize));
        }
    }

    /**
     * @brief Overload the = operator to set buffer contents from another
     * buffer
     *
     * @param other The buffer to set the contents from
     *
     */
    Buffer<T> &operator=(const Buffer &other) {
        // copy and swap because apparently it's good
        // i don't know what this is
        Buffer temp(other);
        swap(temp);

        return *this;
    }

    /**
     * @brief Get the data from the buffer as a vector
     *
     * @return std::vector<T> The data from the buffer
     *
     */
    vec<T> getData() const {
        this->checkFreed();
        this->checkInit();
        vec<T> data(this->length);

        // flawfinder: ignore
        memcpy(data.data(), this->buffer->contents(), this->length * this->itemsize);

        return data;
    }

    /**
     * @brief Convert the buffer to a Texture1D object
     *
     * @return MTLCompute::Texture1D<T> The Texture1D object
     *
     */
    Texture1D<T> toTexture1D() const {
        try {
            Texture1D<T> texture(this->gpu, this->length);
            vec<T> data = this->getData();
            texture = data;
            return texture;
        } catch (const Error::TextureTypeError &e) {
            throw Error::ConversionTypeError("Invalid type for conversion to Texture1D");
        } catch (const Error::TextureSizeError &e) {
            throw Error::ConversionSizeError("Invalid size for conversion to "
                                             "Texture1D (must be less than + " +
                                             std::to_string(MAX_TEXTURE1D_SIZE) + ")");
        }
    }

    TextureBuffer<T> toTextureBuffer() const {
        try {
            TextureBuffer<T> texture(this->gpu, this->length);
            vec<T> data = this->getData();
            texture = data;
            return texture;
        } catch (const Error::TextureTypeError &e) {
            throw Error::ConversionTypeError(
                "Invalid type for conversion to TextureBuffer");
        } catch (const Error::TextureSizeError &e) {
            throw Error::ConversionSizeError("Invalid size for conversion to "
                                             "TextureBuffer (must be less than + " +
                                             std::to_string(MAX_TEXTUREBUFFER_SIZE) +
                                             ")");
        }
    }

    /**
     * @brief Get the MTL::Buffer object
     *
     * @return MTL::Buffer* The MTL::Buffer object
     *
     */
    MTL::Buffer *getBuffer() { return this->buffer; }

    /**
     * @brief Get the GPU device
     *
     * @return MTL::Device* The GPU device
     *
     */
    MTL::Device *getGPU() { return this->gpu; }

    /**
     * @brief Get whether the buffer has been freed
     *
     * @return bool Whether the buffer has been freed
     *
     */
    bool getFreed() { return this->freed; }

    /**
     * @brief Get the storage mode of the buffer
     *
     * @return MTLCompute::ResourceStorage The storage mode of the buffer
     *
     */
    MTLCompute::ResourceStorage getStorageMode() { return this->storageMode; }

    /**
     * @brief Get the length of the buffer
     *
     * @return size_t The length of the buffer
     *
     */
    size_t getLength() { return this->length; }

    /**
     * @brief Get the size of each item in the buffer
     *
     * @return size_t The size of each item in the buffer
     *
     */
    size_t getItemSize() { return this->itemsize; }
};

} // namespace MTLCompute
