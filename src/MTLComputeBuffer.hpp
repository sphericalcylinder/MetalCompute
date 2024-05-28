#include <iostream>
#include <vector>
#include "MTLComputeGlobals.hpp"

#pragma once

namespace MTLCompute {

    template<typename T>
    class Buffer {
        private:
            MTL::Device *gpu; ///< The Metal device object
            MTL::Buffer *buffer; ///< The Metal buffer object
            bool freed = false; ///< Whether the buffer has been freed
            MTLCompute::ResourceStorage storageMode; ///< The storage mode of the buffer

            void swap(Buffer &buffer) noexcept {
                using std::swap;
                swap(this->gpu, buffer.gpu);
                swap(this->buffer, buffer.buffer);
                swap(this->length, buffer.length);
                swap(this->itemsize, buffer.itemsize);
                swap(this->storageMode, buffer.storageMode);
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
            Buffer(size_t length, MTL::Device *gpu, ResourceStorage storageMode) {
                this->gpu = gpu;
                this->length = length;
                this->itemsize = sizeof(T);
                this->storageMode = storageMode;
                this->buffer = gpu->newBuffer(length*itemsize, static_cast<MTL::ResourceOptions>(storageMode));
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
             * @brief Destructor for the Buffer class
             *
             * Sets the buffer to be freed but does not actually free it
             * because sometime is segfaults and this is easier
             *
            */
            ~Buffer() {
                if (!this->freed) {
                    //this->buffer->release();
                    this->freed = true;
                }
            }

            /**
             * @brief Get the contents of the buffer
             * 
             * @return T* The contents of the buffer
             *
            */
            T *contents() {
                if (this->freed) {
                    throw std::runtime_error("Buffer already freed");
                }
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
                if (this->freed) {
                    throw std::runtime_error("Buffer already freed");
                }
                if (index >= this->length) {
                    throw std::out_of_range("Index out of bounds");
                }
                return ((T *)this->buffer->contents())[index];
            }

            /**
             * @brief Overload the [] operator to set the value at an index
             *
             * @param index The index to set the value at
             * @return T The value to set at the index
             *
            */
            T& operator[](size_t index) {
                if (this->freed) {
                    throw std::runtime_error("Buffer already freed");
                }
                if (index >= this->length) {
                    throw std::out_of_range("Index out of bounds");
                }
                return ((T *)this->buffer->contents())[index];
            }

            /**
             * @brief Overload the = operator to set buffer contents from a vector
             *
             * @param data The data to set the buffer contents to
             *
            */
            void operator=(std::vector<T> data) {
                if (this->freed) {
                    throw std::runtime_error("Buffer already freed");
                }
                if (data.size() != this->length) {
                    throw std::invalid_argument("Data size does not match buffer size");
                }
                memcpy(this->buffer->contents(), data.data(), this->length*this->itemsize);

                if (this->storageMode == MTLCompute::ResourceStorage::Managed) {
                    this->buffer->didModifyRange(NS::Range(0, this->length*this->itemsize));
                }
            }

            /**
             * @brief Overload the = operator to set buffer contents from another buffer
             *
             * @param other The buffer to set the contents from
             *
            */
            Buffer<T> & operator=(const Buffer &other) {
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
            std::vector<T> getData() {
                if (this->freed) {
                    throw std::runtime_error("Buffer already freed");
                }
                if (this->length == -1) {
                    throw std::runtime_error("Buffer not initialized");
                }
                std::vector<T> data(this->length);
                memcpy(data.data(), this->buffer->contents(), this->length*this->itemsize);
                return data;
            }

            /**
             * @brief Get the MTL::Buffer object
             *
             * @return MTL::Buffer* The MTL::Buffer object
             *
            */
            MTL::Buffer *getBuffer() {
                return this->buffer;
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
             * @brief Get whether the buffer has been freed
             *
             * @return bool Whether the buffer has been freed
             *
            */
            bool getFreed() {
                return this->freed;
            }

            /**
             * @brief Get the storage mode of the buffer
             *
             * @return MTLCompute::ResourceStorage The storage mode of the buffer
             *
            */
            MTLCompute::ResourceStorage getStorageMode() {
                return this->storageMode;
            }

            size_t length; ///< The length of the buffer
            size_t itemsize; ///< The size of each item in the buffer
    };

}

