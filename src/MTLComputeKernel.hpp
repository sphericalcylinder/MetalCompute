#include "MTLComputeGlobals.hpp"

#pragma once

namespace MTLCompute {

    class Kernel {

        private:
            MTL::Device *gpu; ///< The Metal device object
            MTL::Library *library; ///< The Metal library object
            MTL::Function *function; ///< The Metal function object
            MTL::ComputePipelineState *pipeline; ///< The Metal compute pipeline state object

        public:

            /**
             * @brief Constructor for the Kernel class
             *
             * Takes in the GPU device and the filename of the Metal library
             * and creates a new Kernel object and MTL::Library
             *
             * @param gpu The GPU device
             * @param filename The filename of the Metal library
             *
            */
            Kernel(MTL::Device *gpu, const std::string &filename) {
                this->gpu = gpu;

                this->library = this->gpu->newLibrary(NS::URL::fileURLWithPath(NS::String::string(filename.c_str(), \
                            NS::ASCIIStringEncoding)), nullptr);

                if (this->library == nullptr) {
                    std::cerr << "Error: Could not load library " << filename << std::endl;
                }

            }

            /**
             * @brief Constructor for the Kernel class with function name
             *
             * Takes in the GPU device, the filename of the Metal library, and the name of the function
             * and creates a new Kernel object and MTL::Library. Also calls useFunction with the function name
             *
             * @param gpu The GPU device
             * @param filename The filename of the Metal library
             * @param funcname The name of the function
             *
            */
            Kernel(MTL::Device *gpu, const std::string &filename, const std::string &funcname) {
                this->gpu = gpu;

                this->library = this->gpu->newLibrary(NS::String::string(filename.c_str(), NS::ASCIIStringEncoding), nullptr);

                if (this->library == nullptr) {
                    std::cerr << "Error: Could not load library " << filename << std::endl;
                }

                useFunction(funcname);

            }

            /**
             * @brief Default constructor for the Kernel class
             *
            */
            Kernel() = default;

            /**
             * @brief Destructor for the Kernel class
             *
             * Releases the library object
             *
            */
            ~Kernel() {
                this->library->autorelease();
            }

            /**
             * @brief Get the names of the functions in the library
             *
             * @return A vector of strings containing the names of the functions in the library
             *
            */
            vec<std::string> getFunctionNames() {
                vec<std::string> names;
                for (int i = 0; i < this->library->functionNames()->count(); i++) {
                    names.push_back(this->library->functionNames()->object(i)->description()->cString(NS::ASCIIStringEncoding));
                }
                return names;
            }

            /**
             * @brief Use a function in the library
             *
             * Takes in the name of a function in the library and creates a new function and compute pipeline state
             *
             * @param funcname The name of the function
             *
            */
            void useFunction(const std::string &funcname) {
                this->function = this->library->newFunction(NS::String::string(funcname.c_str(), NS::ASCIIStringEncoding));
                if (this->function == nullptr) {
                    throw std::runtime_error("Error: Could not load function " + funcname);
                }

                NS::Error *error = nullptr;
                this->pipeline = gpu->newComputePipelineState(this->function, &error);
            }

            /**
             * @brief Get the MTL::ComputePipelineState object
             *
             * @return MTL::ComputePipelineState* The MTL::ComputePipelineState object
             *
            */
            MTL::ComputePipelineState *getPLS() {
                return this->pipeline;
            }

    };

}