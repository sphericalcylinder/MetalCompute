#include "MTLComputeGlobals.hpp"
#include "MTLComputeBuffer.hpp"
#include "MTLComputeKernel.hpp"
#include "MTLComputeCommandManager.hpp"
#include "MTLComputeTexture.hpp"

#pragma once

#ifdef USING_MULTIPLE_CLASSES
#error "Cannot include both MTLCompute.hpp and MTLComputeGPU.hpp"
#endif

#define USING_SINGLE_CLASS

namespace MTLCompute {

    template <typename T>
    class GPU {

        private:
            MTL::Device *gpu; ///< The Metal device object
            MTLCompute::Kernel kernel; ///< The MetalCompute kernel object
            bool kernelLoaded = false; ///< Whether the kernel has been loaded
            MTLCompute::CommandManager<T> commandManager; ///< The MetalCompute command manager object

            /**
             * @brief Check if the kernel has been loaded
             *
             * If the kernel has not been loaded, print an error message
             *
            */
            void checkloaded() {
                if (!this->kernelLoaded) {
                    throw std::runtime_error("MTLCompute error: Kernel not loaded");
                }
            }

        public:

            /**
             * @brief Constructor for the GPU class
             *
             * Takes in the name of the Metal library and the name of the function
             * and creates a new GPU object, MTLCompute::Kernel object, and MTLCompute::CommandManager object
             *
             * @param libname The name of the Metal library
             * @param functionName The name of the function
             *
            */
            GPU(std::string libname, std::string functionName) : kernel(), commandManager() {
                this->gpu = MTL::CreateSystemDefaultDevice();
                this->kernel = MTLCompute::Kernel(this->gpu, libname+".metallib");
                this->kernel.useFunction(functionName);
                this->commandManager = MTLCompute::CommandManager<T>(this->gpu, &this->kernel);
                this->kernelLoaded = true;
            }

            /**
             * @brief Default constructor for the GPU class
             *
             * Creates a new GPU object, MTLCompute::Kernel object, and MTLCompute::CommandManager object
             *
            */
            GPU() : kernel(), commandManager() {
                this->gpu = MTL::CreateSystemDefaultDevice();
            };

            /**
             * @brief Destructor for the GPU class
             *
             * Releases the GPU object
             *
            */
            ~GPU() {
                this->gpu->autorelease();
            }

            /**
             * @brief Load a metal library and function
             *
             * Takes in the name of the Metal library and the name of the function
             * and creates a new MTLCompute::Kernel object and MTLCompute::CommandManager object
             *
             * @param libname The name of the Metal library
             * @param functionName The name of the function
             *
            */
            void loadKernel(std::string libname, std::string functionName) {
                this->kernel = MTLCompute::Kernel(this->gpu, libname+".metallib");
                this->kernel.useFunction(functionName);
                this->commandManager = MTLCompute::CommandManager<T>(this->gpu, &this->kernel);
                this->kernelLoaded = true;
            }

            /**
             * @brief Load an array into the GPU
             *
             * Takes in a vector of type T and an index and loads the array into the GPU
             * at that index
             *
             * @param array The array to load
             * @param index The index to load the array into
             *
            */
            void loadArray(vec<T> &array, int index) {
                this->checkloaded();
                MTLCompute::Buffer<T> buffer(gpu, array.size(), MTLCompute::ResourceStorage::Shared);
                buffer = array;
                this->commandManager.loadBuffer(buffer, index);
            }

            /**
             * @brief Load a 2d matrix into the GPU
             *
             * Takes in a vector of vectors of type T and an index and loads the matrix into the GPU
             * at that index
             *
             * @param matrix The matrix to load
             * @param index The index to load the matrix into
             *
            */
            void loadMatrix(vec2<T> &matrix, int index) {
                this->checkloaded();
                MTLCompute::Texture2D<T> texture(gpu, matrix[0].size(), matrix.size());
                texture = matrix;
                this->commandManager.loadTexture(texture, index);
            }

            /**
             * @brief Load a 3d matrix into the GPU
             *
             * Takes in a vector of vectors of vectors of type T and an index and loads the matrix into the GPU
             * at that index
             *
             * @param matrix The matrix to load
             * @param index The index to load the matrix into
             *
            */
            void loadMatrix(vec3<T> &matrix, int index) {
                this->checkloaded();
                MTLCompute::Texture3D<T> texture(gpu, matrix[0][0].size(), matrix[0].size(), matrix.size());
                texture = matrix;
                this->commandManager.loadTexture(texture, index);
            }

            /**
             * @brief Run the kernel
             *
             * Dispatches the command manager
             *
            */
            void runKernel() {
                this->checkloaded();
                this->commandManager.dispatch();
            }

            /**
             * @brief Get an array from the GPU
             *
             * Takes in an index and returns the array at that index
             *
             * @param index The index of the array
             *
             * @return The array at the index
             *
            */
            vec<T> getArray(int index) {
                this->checkloaded();
                MTLCompute::Buffer<T> buffer = commandManager.getBuffers()[index];
                return buffer.getData();
            }

            /**
             * @brief Get a 2d matrix from the GPU
             *
             * Takes in an index and returns the matrix at that index
             *
             * @param index The index of the 2d matrix
             *
             * @return The 2d matrix at the index
             *
            */
            vec2<T> get2DMatrix(int index) {
                this->checkloaded();
                MTLCompute::Texture2D<T> texture = commandManager.getTexture2D(index);
                return texture.getData();
            }

            /**
             * @brief Get a 3d matrix from the GPU
             *
             * Takes in an index and returns the matrix at that index
             *
             * @param index The index of the 3d matrix
             *
             * @return The 3d matrix at the index
             *
            */
            vec3<T> get3DMatrix(int index) {
                this->checkloaded();
                MTLCompute::Texture3D<T> texture = commandManager.getTexture3D(index);
                return texture.getData();
            }

            /**
             * @brief Reset the GPU
             *
             * Resets the command manager
             *
            */
            void reset() {
                this->checkloaded();
                this->commandManager.reset();
            }

            /**
             * @brief Get the GPU device
             *
             * @return The GPU device
             *
            */
            MTL::Device *getGPU() {
                return this->gpu;
            }

    };

}