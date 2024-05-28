#include <iostream>
#include <vector>
#include "MTLComputeGlobals.hpp"

#pragma once

namespace MTLCompute {

    template<typename T>
    class Texture {
        private:
            MTL::Device *gpu; ///< The Metal device object
            MTL::Texture *texture; ///< The Metal texture object
            MTL::TextureDescriptor *descriptor; ///< The Metal texture descriptor object
            int width = -1; ///< The width and height of the texture
            int height = -1; ///< The width and height of the texture
            bool freed = false; ///< Whether the texture has been freed

            /**
             * @brief Swap the contents of two Textures
             *
             * @param buf The texture to swap with
             *
            */
            void swap(Texture &tex) noexcept {
                using std::swap;
                swap(this->gpu, tex.gpu);
                swap(this->texture, tex.texture);
                swap(this->width, tex.width);
                swap(this->height, tex.height);
                swap(this->freed, tex.freed);
            }

            /**
             * @brief Flatten a 2D vector into a 1D vector
             *
             * @param v The 2D vector to flatten
             *
             * @return std::vector<T> The flattened vector
             *
            */
            std::vector<T> flatten(std::vector<std::vector<T>> &v) {
                std::vector<T> result;
                for (auto &inner : v) {
                    result.insert(result.end(), inner.begin(), inner.end());
                }
                return result;
            }

            /**
             * @brief Unflatten a 1D vector into a 2D vector
             *
             * @param v The 1D vector to unflatten
             * @param width The width of the 2D vector
             * @param height The height of the 2D vector
             *
             * @return std::vector<std::vector<T>> The unflattened vector
             *
            */
            std::vector<std::vector<T>> unflatten(std::vector<T> &v, int width, int height) {
                std::vector<std::vector<T>> result(height, std::vector<T>(width));
                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < width; j++) {
                        result[i][j] = v[i*width + j];
                    }
                }
                return result;
            }
            
        public:

            /**
             * @brief Constructor for the Texture class
             *
             * Constructs a new texture object
             *
             * @param gpu The Metal device object
             * @param width The width of the texture
             * @param height The height of the texture
             * @param tt The texture type
             *
            */
            Texture(MTL::Device *gpu, int width, int height, TextureType tt) {
                this->gpu = gpu;
                this->width = width;
                this->height = height;
                this->descriptor = MTL::TextureDescriptor::alloc()->init();
                this->descriptor->setTextureType(MTL::TextureType2D);
                this->descriptor->setPixelFormat(static_cast<MTL::PixelFormat>(tt));
                if (sizeof(T) != TextureTypeSizes[tt]) {
                    std::cout << sizeof(T) << " " << TextureTypeSizes[tt] << std::endl;
                    throw std::invalid_argument("Texture type does not match template type");
                }
                this->descriptor->setWidth(width);
                this->descriptor->setHeight(height);
                this->texture = this->gpu->newTexture(this->descriptor);
            }


            /**
             * @brief Copy constructor for the Texture class
             *
             * Constructs a new texture from an existing texture
             *
             * @param other The texture to copy
             *
            */
            Texture(const Texture &other) {
                this->gpu = other.gpu;
                this->texture = other.texture;
                this->width = other.width;
                this->height = other.height;
                this->descriptor = other.descriptor;
            }


            /**
             * @brief Default constructor for the Texture class
             *
             * Creates a new empty texture object
             *
            */
            Texture() {
                this->gpu = nullptr;
                this->descriptor = nullptr;
                this->texture = nullptr;
                this->width = -1;
                this->height = -1;
            }

            /**
             * @brief Destructor for the Texture class
             *
             * Sets the texture to be freed but does not actually free it
             * because sometime it segfaults and this is easier
             *
            */
            ~Texture() {
                if (!this->freed) {
                    //this->buffer->release();
                    this->freed = true;
                }
            }

            /**
             * @brief Overload the = operator to set texture contents from a vector
             *
             * @param data The data to set the texture contents to
             *
            */
            void operator=(std::vector<std::vector<T>> data) {
                if (this->freed) {
                    throw std::runtime_error("Texture already freed");
                }
                if (data.size() != this->height || data[0].size() != this->width) {
                    throw std::invalid_argument("Data size does not match texture size");
                }
                std::vector<T> flat = flatten(data);
                this->texture->replaceRegion(MTL::Region::Make2D(0, 0, this->width, this->height), 0, flat.data(), this->width*sizeof(T));
            }

            /**
             * @brief Overload the = operator to set texture contents from another texture
             *
             * @param other The texture to set the contents from
             *
            */
            Texture & operator=(const Texture &other) {

                Texture temp(other);
                swap(temp);

                return *this;
            }

            /**
             * @brief Get the data from the texture as a vector
             *
             * @return std::vector<std::vector<T>> The data from the texture
             *
            */
            std::vector<std::vector<T>> getData() {
                if (this->freed) {
                    throw std::runtime_error("Buffer already freed");
                }
                if (this->width == -1 || this->height == -1) {
                    throw std::runtime_error("Buffer not initialized");
                }
                std::vector<T> flat(this->width*this->height);
                this->texture->getBytes(flat.data(), this->width*sizeof(T), MTL::Region::Make2D(0, 0, this->width, this->height), 0);
                return unflatten(flat, this->width, this->height);
            }

            /**
             * @brief Get the MTL::Texture object
             *
             * @return MTL::Texture* The MTL::Texture object
             *
            */
            MTL::Texture *getTexture() {
                return this->texture;
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
             * @brief Get whether the texture has been freed
             *
             * @return bool Whether the texture has been freed
             *
            */
            bool getFreed() {
                return this->freed;
            }

            /**
             * @brief Get the texture descriptor
             *
             * @return MTL::TextureDescriptor* The texture descriptor
             *
            */
            MTL::TextureDescriptor *getDescriptor() {
                return this->descriptor;
            }

            /**
             * @brief Get the width of the texture
             *
             * @return int The width of the texture
             *
            */
            int getWidth() {
                return this->width;
            }

            /**
             * @brief Get the height of the texture
             *
             * @return int The height of the texture
             *
            */
            int getHeight() {
                return this->height;
            }
    };

}
