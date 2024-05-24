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
            
        public:
            /**
             * @brief Constructor for the Buffer class 
             *
             * Takes in the length of the buffer, the GPU device, 
             * and the storage mode and creates a new buffer
             *
             * @param[in] length The length of the buffer
             * @param[in] gpu The GPU device
             * @param[in] storageMode The storage mode of the buffer
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
             * @brief Destructor for the Buffer class
             *
             * Releases the MTL::Buffer object
             *
            */
            ~Buffer() {
                if (!this->freed) {
                    //this->buffer->release();
                    this->freed = true;
                }
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
             * @param[in] index The index to get the value from
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
             * @param[in] index The index to set the value at
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
             * @param[in] data The data to set the buffer contents to
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
             * @brief Get the data from the buffer as a vector
             *
             * @return std::vector<T> The data from the buffer
             *
            */
            std::vector<T> getData() {
                if (this->freed) {
                    throw std::runtime_error("Buffer already freed");
                }
                std::vector<T> data(this->length);
                memcpy(data.data(), this->buffer->contents(), this->length*this->itemsize);
                return data;
            }

            size_t length; ///< The length of the buffer
            size_t itemsize; ///< The size of each item in the buffer
    };

}

template class MTLCompute::Buffer<float>;
