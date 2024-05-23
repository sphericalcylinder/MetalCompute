#include <iostream>
#include <vector>
#include "MTLComputeGlobals.hpp"

#pragma once

namespace MTLCompute {

    template<typename T>
    class Buffer {
        private:
            MTL::Buffer *buffer; ///< The Metal buffer object
            
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
                this->length = length;
                this->itemsize = sizeof(T);
                this->buffer = gpu->newBuffer(length*itemsize, static_cast<MTL::ResourceOptions>(storageMode));
            }

            /**
             * @brief Destructor for the Buffer class
             *
             * Releases the MTL::Buffer object
             *
            */
            ~Buffer() {
                this->buffer->release();
            }

            /**
             * @brief Get the contents of the buffer
             * 
             * @return T* The contents of the buffer
             *
            */
            T *contents() {
                return (T *)this->buffer->contents();
            }

            /**
             * @brief Overload the [] operator to get the value at an index
             *
             * @param[in] index The index to get the value from
             * @return T The value at the index
            */
            T operator[](size_t index) {
                if (index >= this->length) {
                    std::cerr << "Index out of bounds" << std::endl;
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
                if (data.size() != this->length) {
                    std::cerr << "Data size does not match buffer size" << std::endl;
                    throw std::invalid_argument("Data size does not match buffer size");
                    return;
                }
                memcpy(this->buffer->contents(), data.data(), this->length*this->itemsize);
            }

            size_t length; ///< The length of the buffer
            size_t itemsize; ///< The size of each item in the buffer
    };

}
