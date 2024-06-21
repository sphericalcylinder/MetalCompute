#include "MTLComputeGlobals.hpp"
#include "MTLComputeErrors.hpp"

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
            int components = numComponents<T>(); ///< The number of components in the texture

            /**
             * @brief Swap the contents of two 1D Textures
             *
             * @param buf The 1D texture to swap with
             *
            */
            void swap(Texture1D &tex) noexcept {
                using std::swap;
                swap(this->gpu, tex.gpu);
                swap(this->texture, tex.texture);
                swap(this->width, tex.width);
                swap(this->freed, tex.freed);
            }

            MTL::PixelFormat guessFormat() const {
                if (typeid(T) == typeid(uchar)) return MTL::PixelFormatR8Uint;
                if (typeid(T) == typeid(ushort)) return MTL::PixelFormatR16Uint;
                if (typeid(T) == typeid(uint)) return MTL::PixelFormatR32Uint;
                if (typeid(T) == typeid(char)) return MTL::PixelFormatR8Sint;
                if (typeid(T) == typeid(short)) return MTL::PixelFormatR16Sint;
                if (typeid(T) == typeid(int)) return MTL::PixelFormatR32Sint;
                if (typeid(T) == typeid(float)) return MTL::PixelFormatR32Float;

                if (typeid(T) == typeid(uchar2)) return MTL::PixelFormatRG8Uint;
                if (typeid(T) == typeid(ushort2)) return MTL::PixelFormatRG16Uint;
                if (typeid(T) == typeid(uint2)) return MTL::PixelFormatRG32Uint;
                if (typeid(T) == typeid(char2)) return MTL::PixelFormatRG8Sint;
                if (typeid(T) == typeid(short2)) return MTL::PixelFormatRG16Sint;
                if (typeid(T) == typeid(int2)) return MTL::PixelFormatRG32Sint;
                if (typeid(T) == typeid(float2)) return MTL::PixelFormatRG32Float;

                if (typeid(T) == typeid(uchar4)) return MTL::PixelFormatRGBA8Uint;
                if (typeid(T) == typeid(ushort4)) return MTL::PixelFormatRGBA16Uint;
                if (typeid(T) == typeid(uint4)) return MTL::PixelFormatRGBA32Uint;
                if (typeid(T) == typeid(char4)) return MTL::PixelFormatRGBA8Sint;
                if (typeid(T) == typeid(short4)) return MTL::PixelFormatRGBA16Sint;
                if (typeid(T) == typeid(int4)) return MTL::PixelFormatRGBA32Sint;
                if (typeid(T) == typeid(float4)) return MTL::PixelFormatRGBA32Float;

                throw TextureTypeError("Texture type not supported");
            }

            // Functions that should be overridden

            /**
             * @brief Set the texture's dimension
             *
             * Sets the texture's dimension to 1D
             *
            */
            virtual void setTextureDimension() const {
                this->descriptor->setTextureType(MTL::TextureType1D);
            }

            /**
             * @brief Get the texture region
             *
             * @return MTL::Region The texture region
             *
            */
            virtual MTL::Region textureReigon() const {
                return MTL::Region::Make1D(0, this->width);
            }

            /**
             * @brief Check the maximum texture size
             *
             * @param width The width of the texture
             *
            */
            void checkMaxSize(int width) const {
                if (width > MAX_TEXTURE1D_SIZE) 
                    throw TextureSizeError("Texture size too large, max size is 16384");
                
            }

            /**
             * @brief Check the data size
             *
             * @param data The data to check
             *
            */
            void checkDataSize(vec<T> data) const {
                if (data.size() != this->width) 
                    throw TextureSizeError("Data size does not match texture size");
                
            }

            /**
             * @brief Check if the texture has been initialized
             *
            */
            virtual void checkInit() const {
                if (this->width == -1)
                    throw TextureInitError("Texture not initialized");
                
            }

            /**
             * @brief Check if the index is in bounds (0 to this->width)
             *
             * @param index The index to check
             *
            */
            void checkIndex(size_t index) const {
                if (index >= (long)this->width || index < 0) 
                    throw TextureIndexError("Texture index out of bounds");
                
            }

            /**
             * @brief Check if the texture has been freed
             *
            */
            void checkFreed() const {
                if (this->freed)
                    throw TextureFreeError("Texture already freed");
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
            Texture1D(MTL::Device *gpu, int width) : gpu(gpu), width(width) {
                this->gpu = gpu;
                this->checkMaxSize(width);
                this->width = width;
                this->descriptor = MTL::TextureDescriptor::alloc()->init();
                this->setTextureDimension();
                this->descriptor->setPixelFormat(this->guessFormat());
                
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
            Texture1D(const Texture1D &other) : gpu(other.gpu), width(other.width), 
                descriptor(other.descriptor), texture(other.texture) {}

            /**
             * @brief Default constructor for the Texture1D class
             *
             * Creates a new empty texture object
             *
            */
            Texture1D() : gpu(nullptr), width(-1), descriptor(nullptr), texture(nullptr) {}

            /**
             * @brief Destructor for the Texture1D class
             *
             * Calls autorelease on the texture and descriptor objects
             * and sets the freed flag to true
             *
            */
            virtual ~Texture1D() {
                if (!this->freed) {
                    this->texture->autorelease();
                    this->descriptor->autorelease();
                    this->freed = true;
                }
            }

            /**
             * @brief Overload the = operator to set 1D texture contents from a vector
             *
             * @param data The data to set the texture contents to
             *
            */
            void operator=(vec<T> data) {
                this->checkFreed();
                this->checkDataSize(data);
                this->texture->replaceRegion(this->textureReigon(), 0, data.data(), this->width*sizeof(T));
            }


            /**
             * @brief Overload the = operator to set 1D texture contents from another texture
             *
             * @param other The 1D texture to set the contents from
             *
            */
            Texture1D & operator=(const Texture1D &other) {

                Texture1D temp(other);
                swap(temp);

                return *this;
            }

            /**
             * @brief Overload the [] operator to get an item from the 1D texture
             *
             * @param index The index of the item to get
             *
             * @return T The item from the texture
             *
            */
            T operator[](size_t index) const {
                this->checkFreed();
                this->checkInit();
                this->checkIndex(index);
                vec<T> flat(this->width);
                this->texture->getBytes(flat.data(), this->width*sizeof(T), this->textureReigon(), 0);
                return flat[index];
            }

            /**
             * @brief Get the data from the 1D texture as a vector
             *
             * @return std::vector<T> The data from the texture
             *
            */
            vec<T> getData() {
                this->checkFreed();
                this->checkInit();
                vec<T> out(this->width);
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
             * @brief Get the number of components in the texture
             *
             * @return int The number of components in the texture
             *
            */
            int getComponents() const {
                return this->components;
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

            /**
             * @brief Swap the contents of two 2D textures
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
            vec<T> flatten(vec2<T> &v) const {
                vec<T> result;
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
            vec2<T> unflatten(vec<T> &v, int width, int height) const {
                vec2<T> result(height, vec<T>(width));
                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < width; j++) {
                        result[i][j] = v[i*width + j];
                    }
                }
                return result;
            }


            // Functions that should be overridden

            /**
             * @brief Set the texture's dimension
             *
             * Sets the texture's dimension to 2D
             *
            */
            virtual void setTextureDimension() const override {
                this->descriptor->setTextureType(MTL::TextureType2D);
            }

            /**
             * @brief Get the texture region
             *
             * @return MTL::Region The texture region
             *
            */
            virtual MTL::Region textureReigon() const override {
                return MTL::Region::Make2D(0, 0, this->width, this->height);
            }
            
            /**
             * @brief Check the maximum texture size
             *
             * @param width The width of the texture
             * @param height The height of the texture
             *
            */
            void checkMaxSize(int width, int height) const {
                if (width > MAX_TEXTURE2D_SIZE || height > MAX_TEXTURE2D_SIZE) 
                    throw TextureSizeError("Texture size too large, max size is 16384");
                
            }

            /**
             * @brief Check the data size
             *
             * @param data The data to check
             *
            */
            void checkDataSize(vec2<T> data) const {
                if (data.size() != this->height || data[0].size() != this->width) 
                    throw TextureSizeError("Data size does not match texture size");
                
            }

            /**
             * @brief Check if the texture has been initialized
             *
            */
            virtual void checkInit() const override {
                if (this->width == -1 || this->height == -1) 
                    throw TextureInitError("Texture not initialized");
                
            }

        public:

            /**
             * @brief Constructor for the Texture2D class
             *
             * Constructs a new 2D texture object
             *
             * @param gpu The Metal device object
             * @param width The width of the texture
             * @param height The height of the texture
             * @param tt The texture type
             *
            */
            Texture2D(MTL::Device *gpu, int width, int height) : Texture1D<T>(gpu, width) {
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
             * @brief Overload the = operator to set 2D texture contents from a 2D vector
             *
             * @param data The data to set the texture contents to
             *
            */
            void operator=(vec2<T> data) {
                this->checkFreed();
                
                this->checkDataSize(data);
                vec<T> flat = flatten(data);
                this->texture->replaceRegion(this->textureReigon(), 0, flat.data(), this->width*sizeof(T));
            }

            /**
             * @brief Overload the = operator to set 2D texture contents from another texture
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
             * @brief Overload the [] operator to get a row from the 2D texture
             *
             * @param index The index of the row to get
             *
             * @return std::vector<T> The row from the texture
             *
            */
            vec<T> operator[](size_t index) const {
                this->checkFreed();
                this->checkInit();
                this->checkIndex(index);
                vec<T> flat((long)this->width*(long)this->height);
                this->texture->getBytes(flat.data(), this->width*sizeof(T), this->textureReigon(), 0);
                return unflatten(flat, this->width, this->height)[index];
            }

            /**
             * @brief Get the data from the 2D texture as a 2D vector
             *
             * @return std::vector<std::vector<T>> The data from the texture
             *
            */
            vec2<T> getData() {
                this->checkFreed();
                this->checkInit();
                vec<T> flat((long)this->width*(long)this->height);
                this->texture->getBytes(flat.data(), this->width*sizeof(T), this->textureReigon(), 0);
                return unflatten(flat, this->width, this->height);
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
             * @brief Swap the contents of two 3D extures
             *
             * @param buf The texture to swap with
             *
            */
            void swap(Texture3D &tex) noexcept {
                using std::swap;
                Texture2D<T>::swap(tex);
                swap(this->depth, tex.depth);
            }

            /**
             * @brief Flatten a 3D vector into a 1D vector
             *
             * @param v The 3D vector to flatten
             *
             * @return std::vector<T> The flattened vector
             *
            */
            vec<T> flatten(vec3<T> &v) const {
                vec<T> result;
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
            vec3<T> unflatten(vec<T> &v, int width, int height, int depth) const {
                vec3<T> result(depth, vec2<T>(height, vec<T>(width)));
                for (int i = 0; i < depth; i++) {
                    for (int j = 0; j < height; j++) {
                        for (int k = 0; k < width; k++) {
                            result[i][j][k] = v[i*height*width + j*width + k];
                        }
                    }
                }
                return result;
            }


            /**
             * @brief Set the texture's dimension
             *
             * Sets the texture's dimension to 3D
             *
            */
            void setTextureDimension() const override {
                this->descriptor->setTextureType(MTL::TextureType3D);
            }

            /**
             * @brief Get the texture region
             *
             * @return MTL::Region The texture region
             *
            */
            MTL::Region textureReigon() const override {
                return MTL::Region::Make3D(0, 0, 0, this->width, this->height, this->depth);
            }

            /**
             * @brief Check the maximum texture size
             *
             * @param width The width of the texture
             * @param height The height of the texture
             * @param depth The depth of the texture
             *
            */
            void checkMaxSize(int width, int height, int depth) const {
                if (width > MAX_TEXTURE3D_SIZE || height > MAX_TEXTURE3D_SIZE || depth > MAX_TEXTURE3D_SIZE) {
                    throw TextureSizeError("Texture size too large, max size is 16384");
                }
            }

            /**
             * @brief Check the data size
             *
             * @param data The data to check
             *
            */
            void checkDataSize(vec3<T> data) const {
                if (data.size() != this->depth || data[0].size() != this->height || data[0][0].size() != this->width) {
                    throw TextureSizeError("Data size does not match texture size");
                }
            }

            /**
             * @brief Check if the texture has been initialized
             *
            */
            void checkInit() const override {
                if (this->width == -1 || this->height == -1 || this->depth == -1)
                    throw TextureInitError("Texture not initialized");
                
            }


        public:

            /**
             * @brief Constructor for the Texture3D class
             *
             * Constructs a new texture object
             *
             * @param gpu The Metal device object
             * @param width The width of the texture
             * @param height The height of the texture
             * @param depth The depth of the texture
             * @param tt The texture type
             *
            */
            Texture3D(MTL::Device *gpu, int width, int height, int depth) : Texture2D<T>(gpu, width, height) {
                this->checkMaxSize(width, height, depth);
                this->depth = depth;
                this->setTextureDimension();
                this->descriptor->setDepth(depth);
                this->texture = this->gpu->newTexture(this->descriptor);
            }

            /**
             * @brief Copy constructor for the Texture3D class
             *
             * Constructs a new texture from an existing texture
             *
             * @param other The texture to copy
             *
            */
            Texture3D(const Texture3D &other) : Texture2D<T>(other) {
                this->depth = other.depth;
            }

            /**
             * @brief Default constructor for the Texture3D class
             *
             * Creates a new empty texture object
             *
            */
            Texture3D() : Texture2D<T>() {
                this->depth = -1;
            }

            /**
             * @brief Overload the = operator to set 3D texture contents from a vector
             *
             * @param data The data to set the texture contents to
             *
            */
            void operator=(vec3<T> data) {
                this->checkFreed();
                this->checkDataSize(data);
                this->checkMaxSize(data[0][0].size(), data[0].size(), data.size());
                vec<T> flat = flatten(data);
                this->texture->replaceRegion(this->textureReigon(), 0, flat.data(), this->width*sizeof(T));
            }

            /**
             * @brief Overload the = operator to set 3D texture contents from a 3D texture
             *
             * @param other The texture to set the contents from
             *
             * @return std::vector<std::vector<T>> The data from the texture
             *
            */
            vec2<T> operator[](size_t index) const {
                this->checkFreed();
                this->checkInit();
                this->checkIndex(index);
                vec<T> flat((long)this->width*(long)this->height*(long)this->depth);
                this->texture->getBytes(flat.data(), this->width*sizeof(T), this->textureReigon(), 0);
                
                return unflatten(flat, this->width, this->height, this->depth)[index];
            }

            /**
             * @brief Get the data from the 3D texture as a 3D vector
             *
             * @return std::vector<std::vector<std::vector<T>>> The data from the texture
             *
            */
            vec3<T> getData() {
                this->checkFreed();
                this->checkInit();
                vec<T> flat((long)this->width*(long)this->height*(long)this->depth);
                this->texture->getBytes(flat.data(), this->width*sizeof(T), this->textureReigon(), 0);

                return unflatten(flat, this->width, this->height, this->depth);
            }

            /**
             * @brief Get the depth of the texture
             *
             * @return int The depth of the texture
             *
            */
            int getDepth() const {
                return this->depth;
            }
    };

}
