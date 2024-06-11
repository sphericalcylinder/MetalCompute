#include <iostream>
#include <vector>
#include "MTLComputeGlobals.hpp"

#pragma once

namespace MTLCompute {

    template <typename T>
    class Texture1D {
        protected:
            MTL::Device *gpu; ///< The Metal device object
            MTL::Texture *texture; ///< The Metal texture object
            MTL::TextureDescriptor *descriptor; ///< The Metal texture descriptor object
            int width = -1; ///< The width and height of the texture
            bool freed = false; ///< Whether the texture has been freed

            /**
             * @brief Swap the contents of two Textures
             *
             * @param buf The texture to swap with
             *
            */
            void swap(Texture1D &tex) noexcept {
                using std::swap;
                swap(this->gpu, tex.gpu);
                swap(this->texture, tex.texture);
                swap(this->width, tex.width);
                swap(this->freed, tex.freed);
            }

        private:

            // Functions that should be overridden

            virtual void setTextureDimension() const {
                this->descriptor->setTextureType(MTL::TextureType1D);
            }

            virtual MTL::Region textureReigon() const {
                return MTL::Region::Make1D(0, this->width);
            }

            void checkMaxSize(int width) const {
                if (width > MAX_TEXTURE_SIZE) {
                    throw std::invalid_argument("Texture size too large, max size is 16384");
                }
            }

            void checkDataSize(std::vector<T> data) const {
                if (data.size() != this->width) {
                    throw std::invalid_argument("Data size does not match texture size");
                }
            }

            virtual void checkInit() const {
                if (this->width == -1) {
                    throw std::runtime_error("Texture not initialized");
                }
            }

            virtual void checkIndex(size_t index) const {
                if (index >= (long)this->width) {
                    throw std::out_of_range("Index out of bounds");
                }
            }

        public:

            /**
             * @brief Constructor for the Texture class
             *
             * Constructs a new texture object
             *
             * @param gpu The Metal device object
             * @param width The width of the texture
             * @param tt The texture type
             *
            */
            Texture1D(MTL::Device *gpu, int width, TextureType tt) {
                this->gpu = gpu;
                this->checkMaxSize(width);
                this->width = width;
                this->descriptor = MTL::TextureDescriptor::alloc()->init();
                this->setTextureDimension();
                this->descriptor->setPixelFormat(static_cast<MTL::PixelFormat>(tt));

                if (sizeof(T) != TextureTypeSizes[tt]) {
                    throw std::invalid_argument("Texture type does not match template type");
                }
                this->descriptor->setWidth(width);
                this->texture = this->gpu->newTexture(this->descriptor);
            }

            /**
             * @brief Constructor for the Texture class
             *
             * Constructs a new texture object and tries to infer the texture type
             *
             * @param gpu The Metal device object
             * @param width The width of the texture
             *
            */
            Texture1D(MTL::Device *gpu, int width) {
                this->gpu = gpu;
                this->checkMaxSize(width);
                this->width = width;
                this->descriptor = MTL::TextureDescriptor::alloc()->init();
                this->setTextureDimension();

                if (typeid(uint8_t) == typeid(T)) {
                    this->descriptor->setPixelFormat(MTL::PixelFormatR8Uint);
                } else if (typeid(uint16_t) == typeid(T)) {
                    this->descriptor->setPixelFormat(MTL::PixelFormatR16Uint);
                } else if (typeid(uint32_t) == typeid(T)) {
                    this->descriptor->setPixelFormat(MTL::PixelFormatR32Uint);
                } else if (typeid(float) == typeid(T)) {
                    this->descriptor->setPixelFormat(MTL::PixelFormatR32Float);
                } else if (typeid(int8_t) == typeid(T)) {
                    this->descriptor->setPixelFormat(MTL::PixelFormatR8Sint);
                } else if (typeid(int16_t) == typeid(T)) {
                    this->descriptor->setPixelFormat(MTL::PixelFormatR16Sint);
                } else if (typeid(int32_t) == typeid(T)) {
                    this->descriptor->setPixelFormat(MTL::PixelFormatR32Sint);
                } else if (typeid(float) == typeid(T)) {
                    this->descriptor->setPixelFormat(MTL::PixelFormatR32Float);
                } else {
                    throw std::invalid_argument("Texture type not supported");
                }
                this->descriptor->setWidth(width);
                this->texture = this->gpu->newTexture(this->descriptor);
            }

            /**
             * @brief Copy constructor for the Texture1D class
             *
             * Constructs a new texture from an existing texture
             *
             * @param other The texture to copy
             *
            */
            Texture1D(const Texture1D &other) {
                this->gpu = other.gpu;
                this->texture = other.texture;
                this->width = other.width;
                this->descriptor = other.descriptor;
            }

            /**
             * @brief Default constructor for the Texture1D class
             *
             * Creates a new empty texture object
             *
            */
            Texture1D() {
                this->gpu = nullptr;
                this->descriptor = nullptr;
                this->texture = nullptr;
                this->width = -1;
            }

            /**
             * @brief Destructor for the Texture1D class
             *
             * Calls autorelease on the texture and descriptor objects
             * and sets the freed flag to true
             *
            */
            ~Texture1D() {
                if (!this->freed) {
                    this->texture->autorelease();
                    this->descriptor->autorelease();
                    this->freed = true;
                }
            }

            /**
             * @brief Overload the = operator to set texture contents from a vector
             *
             * @param data The data to set the texture contents to
             *
            */
            void operator=(std::vector<T> data) {
                if (this->freed) {
                    throw std::runtime_error("Texture already freed");
                }
                this->checkDataSize(data);
                this->checkMaxSize(data.size());
                this->texture->replaceRegion(this->textureReigon(), 0, data.data(), this->width*sizeof(T));
            }

            /**
             * @brief Overload the = operator to set texture contents from another texture
             *
             * @param other The texture to set the contents from
             *
            */
            Texture1D & operator=(const Texture1D &other) {

                Texture1D temp(other);
                swap(temp);

                return *this;
            }

            /**
             * @brief Overload the [] operator to get an item from the texture
             *
             * @param index The index of the item to get
             *
             * @return T The item from the texture
             *
            */
            T operator[](size_t index) const {
                if (this->freed) {
                    throw std::runtime_error("Texture already freed");
                }
                checkIndex(index);
                std::vector<T> flat(this->width);
                this->texture->getBytes(flat.data(), this->width*sizeof(T), this->textureReigon(), 0);
                return flat[index];
            }

            /**
             * @brief Get the data from the texture as a vector
             *
             * @return std::vector<T> The data from the texture
             *
            */
            std::vector<T> getData() {
                if (this->freed) {
                    throw std::runtime_error("Texture already freed");
                }
                this->checkInit();
                std::vector<T> out(this->width);
                this->texture->getBytes(out.data(), this->width*sizeof(T), this->textureReigon(), 0);
                return out;
            }

            /**
             * @brief Get the MTL::Texture object
             *
             * @return MTL::Texture* The MTL::Texture object
             *
            */
            MTL::Texture *getTexture() const {
                return this->texture;
            }

            /**
             * @brief Get the GPU device
             *
             * @return MTL::Device* The GPU device
             *
            */
            MTL::Device *getGPU() const {
                return this->gpu;
            }

            /**
             * @brief Get whether the texture has been freed
             *
             * @return bool Whether the texture has been freed
             *
            */
            bool getFreed() const {
                return this->freed;
            }

            /**
             * @brief Get the texture descriptor
             *
             * @return MTL::TextureDescriptor* The texture descriptor
             *
            */
            MTL::TextureDescriptor *getDescriptor() const {
                return this->descriptor;
            }

            /**
             * @brief Get the width of the texture
             *
             * @return int The width of the texture
             *
            */
            int getWidth() const {
                return this->width;
            }
    };

    template<typename T>
    class Texture2D : public Texture1D<T> {;
        protected:
            int height = -1; ///< The width and height of the texture
        
        private:
            /**
             * @brief Swap the contents of two Textures
             *
             * @param buf The texture to swap with
             *
            */
            void swap(Texture2D &tex) noexcept {
                using std::swap;
                Texture1D<T>::swap(tex);
                swap(this->height, tex.height);
            }

            /**
             * @brief Flatten a 2D vector into a 1D vector
             *
             * @param v The 2D vector to flatten
             *
             * @return std::vector<T> The flattened vector
             *
            */
            std::vector<T> flatten(std::vector<std::vector<T>> &v) const {
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
            std::vector<std::vector<T>> unflatten(std::vector<T> &v, int width, int height) const {
                std::vector<std::vector<T>> result(height, std::vector<T>(width));
                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < width; j++) {
                        result[i][j] = v[i*width + j];
                    }
                }
                return result;
            }


            // Functions that should be overridden


            virtual void setTextureDimension() const override {
                this->descriptor->setTextureType(MTL::TextureType2D);
            }

            virtual MTL::Region textureReigon() const override {
                return MTL::Region::Make2D(0, 0, this->width, this->height);
            }
            
            void checkMaxSize(int width, int height) const {
                if (width > MAX_TEXTURE_SIZE || height > MAX_TEXTURE_SIZE) {
                    throw std::invalid_argument("Texture size too large, max size is 16384");
                }
            }

            void checkDataSize(std::vector<std::vector<T>> data) const {
                if (data.size() != this->height || data[0].size() != this->width) {
                    throw std::invalid_argument("Data size does not match texture size");
                }
            }

            virtual void checkInit() const override {
                if (this->width == -1 || this->height == -1) {
                    throw std::runtime_error("Texture not initialized");
                }
            }

            virtual void checkIndex(size_t index) const override {
                if (index >= (long)this->width*(long)this->height) {
                    throw std::out_of_range("Index out of bounds");
                }
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
            Texture2D(MTL::Device *gpu, int width, int height, TextureType tt) : Texture1D<T>(gpu, width, tt) {
                this->height = height;
                this->setTextureDimension();

                this->descriptor->setHeight(height);
                this->texture = this->gpu->newTexture(this->descriptor);
            }

            /**
             * @brief Constructor for the Texture class
             *
             * Constructs a new texture object and tries to infer the texture type
             *
             * @param gpu The Metal device object
             * @param width The width of the texture
             * @param height The height of the texture
             *
            */
            Texture2D(MTL::Device *gpu, int width, int height) : Texture1D<T>(gpu, width) {
                this->checkMaxSize(width, height);
                this->height = height;
                this->setTextureDimension();

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
            Texture2D(const Texture2D &other) : Texture1D<T>(other) {
                this->height = other.height;
            }


            /**
             * @brief Default constructor for the Texture class
             *
             * Creates a new empty texture object
             *
            */
            Texture2D() : Texture1D<T>() {
                this->height = -1;
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
                this->checkDataSize(data);
                this->checkMaxSize(data[0].size(), data.size());
                std::vector<T> flat = flatten(data);
                this->texture->replaceRegion(this->textureReigon(), 0, flat.data(), this->width*sizeof(T));
            }

            /**
             * @brief Overload the = operator to set texture contents from another texture
             *
             * @param other The texture to set the contents from
             *
            */
            Texture2D & operator=(const Texture2D &other) {

                Texture2D temp(other);
                swap(temp);

                return *this;
            }

            /**
             * @brief Overload the [] operator to get a row from the texture
             *
             * @param index The index of the row to get
             *
             * @return std::vector<T> The row from the texture
             *
            */
            std::vector<T> operator[](size_t index) const {
                if (this->freed) {
                    throw std::runtime_error("Texture already freed");
                }
                checkIndex(index);
                std::vector<T> flat((long)this->width*(long)this->height);
                this->texture->getBytes(flat.data(), this->width*sizeof(T), this->textureReigon(), 0);
                return unflatten(flat, this->width, this->height)[index];
            }

            /**
             * @brief Get the data from the texture as a vector
             *
             * @return std::vector<std::vector<T>> The data from the texture
             *
            */
            std::vector<std::vector<T>> getData() {
                if (this->freed) {
                    throw std::runtime_error("Texture already freed");
                }
                this->checkInit();
                std::vector<T> flat((long)this->width*(long)this->height);
                this->texture->getBytes(flat.data(), this->width*sizeof(T), this->textureReigon(), 0);
                return unflatten(flat, this->width, this->height);
            }

            /**
             * @brief Get the MTL::Texture object
             *
             * @return MTL::Texture* The MTL::Texture object
             *
            */
            MTL::Texture *getTexture() const {
                return this->texture;
            }

            /**
             * @brief Get the GPU device
             *
             * @return MTL::Device* The GPU device
             *
            */
            MTL::Device *getGPU() const {
                return this->gpu;
            }

            /**
             * @brief Get whether the texture has been freed
             *
             * @return bool Whether the texture has been freed
             *
            */
            bool getFreed() const {
                return this->freed;
            }

            /**
             * @brief Get the texture descriptor
             *
             * @return MTL::TextureDescriptor* The texture descriptor
             *
            */
            MTL::TextureDescriptor *getDescriptor() const {
                return this->descriptor;
            }

            /**
             * @brief Get the width of the texture
             *
             * @return int The width of the texture
             *
            */
            int getWidth() const {
                return this->width;
            }

            /**
             * @brief Get the height of the texture
             *
             * @return int The height of the texture
             *
            */
            int getHeight() const {
                return this->height;
            }
    };


    template<typename T>
    class Texture3D : public Texture2D<T> {

        protected:
            int depth = -1; ///< The depth of the texture

        
            /**
             * @brief Swap the contents of two Textures
             *
             * @param buf The texture to swap with
             *
            */
            void swap(Texture3D &tex) noexcept {
                using std::swap;
                swap(this->gpu, tex.gpu);
                swap(this->texture, tex.texture);
                swap(this->descriptor, tex.descriptor);
                swap(this->width, tex.width);
                swap(this->height, tex.height);
                swap(this->depth, tex.depth);
                swap(this->freed, tex.freed);
            }

            /**
             * @brief Flatten a 3D vector into a 1D vector
             *
             * @param v The 3D vector to flatten
             *
             * @return std::vector<T> The flattened vector
             *
            */
            std::vector<T> flatten(std::vector<std::vector<std::vector<T>>> &v) const {
                std::vector<T> result;
                for (auto &inner : v) {
                    for (auto &inner2 : inner) {
                        result.insert(result.end(), inner2.begin(), inner2.end());
                    }
                }
                return result;
            }

            /**
             * @brief Unflatten a 1D vector into a 3D vector
             *
             * @param v The 1D vector to unflatten
             * @param width The width of the 3D vector
             * @param height The height of the 3D vector
             * @param depth The depth of the 3D vector
             *
             * @return std::vector<std::vector<std::vector<T>>> The unflattened vector
             *
            */
            std::vector<std::vector<std::vector<T>>> unflatten(std::vector<T> &v, int width, int height, int depth) const {
                std::vector<std::vector<std::vector<T>>> result(depth, std::vector<std::vector<T>>(height, std::vector<T>(width)));
                for (int i = 0; i < depth; i++) {
                    for (int j = 0; j < height; j++) {
                        for (int k = 0; k < width; k++) {
                            result[i][j][k] = v[i*height*width + j*width + k];
                        }
                    }
                }
                return result;
            }

            void setTextureDimension() const override {
                this->descriptor->setTextureType(MTL::TextureType3D);
            }

            MTL::Region textureReigon() const override {
                return MTL::Region::Make3D(0, 0, 0, this->width, this->height, this->depth);
            }

            void checkMaxSize(int width, int height, int depth) const {
                if (width > MAX_TEXTURE_SIZE || height > MAX_TEXTURE_SIZE || depth > MAX_TEXTURE_SIZE) {
                    throw std::invalid_argument("Texture size too large, max size is 16384");
                }
            }

            void checkDataSize(std::vector<std::vector<std::vector<T>>> data) const {
                if (data.size() != this->depth || data[0].size() != this->height || data[0][0].size() != this->width) {
                    throw std::invalid_argument("Data size does not match texture size");
                }
            }

            void checkInit() const override {
                if (this->width == -1 || this->height == -1 || this->depth == -1) {
                    throw std::runtime_error("Texture not initialized");
                }
            }

            void checkIndex(size_t index) const override {
                if (index >= (long)this->width*(long)this->height*(long)this->depth) {
                    throw std::out_of_range("Index out of bounds");
                }
            }


        public:

            Texture3D(MTL::Device *gpu, int width, int height, int depth, TextureType tt) : Texture2D<T>(gpu, width, height) {
                this->checkMaxSize(width, height, depth);
                this->depth = depth;
                this->setTextureDimension();
                this->descriptor->setDepth(depth);
                this->texture = this->gpu->newTexture(this->descriptor);
            }

            Texture3D(MTL::Device *gpu, int width, int height, int depth) : Texture2D<T>(gpu, width, height) {
                this->checkMaxSize(width, height, depth);
                this->depth = depth;
                this->setTextureDimension();
                this->descriptor->setDepth(depth);
                this->texture = this->gpu->newTexture(this->descriptor);
            }

            Texture3D(const Texture3D &other) : Texture2D<T>(other) {
                this->depth = other.depth;
            }

            Texture3D() : Texture2D<T>() {
                this->depth = -1;
            }

            void operator=(std::vector<std::vector<std::vector<T>>> data) {
                if (this->freed) {
                    throw std::runtime_error("Texture already freed");
                }
                this->checkDataSize(data);
                this->checkMaxSize(data[0][0].size(), data[0].size(), data.size());
                std::vector<T> flat = flatten(data);
                this->texture->replaceRegion(this->textureReigon(), 0, flat.data(), this->width*sizeof(T));
            }

            std::vector<std::vector<T>> operator[](size_t index) const {
                if (this->freed) {
                    throw std::runtime_error("Texture already freed");
                }
                checkIndex(index);
                std::vector<T> flat((long)this->width*(long)this->height*(long)this->depth);
                this->texture->getBytes(flat.data(), this->width*sizeof(T), this->textureReigon(), 0);
                return unflatten(flat, this->width, this->height, this->depth)[index];
            }

            std::vector<std::vector<std::vector<T>>> getData() {
                if (this->freed) {
                    throw std::runtime_error("Texture already freed");
                }
                this->checkInit();
                std::vector<T> flat((long)this->width*(long)this->height*(long)this->depth);
                this->texture->getBytes(flat.data(), this->width*sizeof(T), this->textureReigon(), 0);
                return unflatten(flat, this->width, this->height, this->depth);
            }

            int getDepth() const {
                return this->depth;
            }
    };

}
