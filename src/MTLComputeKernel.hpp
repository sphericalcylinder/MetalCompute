#include "Foundation/NSString.hpp"
#include "Foundation/NSURL.hpp"
#include "MTLComputeGlobals.hpp"
#include "Metal/MTLDevice.hpp"
#include "Metal/MTLLibrary.hpp"
#include "Metal/MTLRenderCommandEncoder.hpp"

#pragma once

namespace MTLCompute {

    class Kernel {

        private:
            MTL::Device *gpu; ///< The Metal device object
            MTL::Library *library; ///< The Metal library object
            MTL::Function *function; ///< The Metal function object
            MTL::ComputePipelineState *pipeline; ///< The Metal compute pipeline state object

        public:

            Kernel(MTL::Device *gpu, const std::string &filename) {
                this->gpu = gpu;

                this->library = this->gpu->newLibrary(NS::URL::fileURLWithPath(NS::String::string(filename.c_str(), \
                            NS::ASCIIStringEncoding)), nullptr);

                if (this->library == nullptr) {
                    std::cerr << "Error: Could not load library " << filename << std::endl;
                }

            }

            Kernel(MTL::Device *gpu, const std::string &filename, const std::string &funcname) {
                this->gpu = gpu;

                this->library = this->gpu->newLibrary(NS::URL::fileURLWithPath(NS::String::string(filename.c_str(), \
                            NS::ASCIIStringEncoding)), nullptr);

                if (this->library == nullptr) {
                    std::cerr << "Error: Could not load library " << filename << std::endl;
                }

                useFunction(funcname);

            }

            ~Kernel() {
                this->pipeline->release();
                this->function->release();
                this->library->release();
            }

            std::vector<std::string> getFunctionNames() {
                std::vector<std::string> names;
                for (int i = 0; i < this->library->functionNames()->count(); i++) {
                    names.push_back(this->library->functionNames()->object(i)->description()->cString(NS::ASCIIStringEncoding));
                }
                return names;
            }

            void useFunction(const std::string &funcname) {
                this->function = this->library->newFunction(NS::String::string(funcname.c_str(), NS::ASCIIStringEncoding));
                if (this->function == nullptr) {
                    throw std::runtime_error("Error: Could not load function " + funcname);
                }

                NS::Error *error = nullptr;
                this->pipeline = gpu->newComputePipelineState(this->function, &error);
            }

            MTL::ComputePipelineState *getPLS() {
                return this->pipeline;
            }

    };

}