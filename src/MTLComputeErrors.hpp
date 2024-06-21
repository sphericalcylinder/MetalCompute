#include "MTLComputeGlobals.hpp"

#pragma once

namespace MTLCompute {

    class TextureError : public std::runtime_error {

        public:
            TextureError() : std::runtime_error("Texture Error") {}

            TextureError(const std::string& message) : std::runtime_error(message) {}

    };

    class TextureSizeError : public TextureError {
        public:
            TextureSizeError() : TextureError("Texture Size Error") {}

            TextureSizeError(const std::string& message) : TextureError(message) {}
    };

    class TextureTypeError : public TextureError {
        public:
            TextureTypeError() : TextureError("Texture Item Type Error") {}

            TextureTypeError(const std::string& message) : TextureError(message) {}
    };

    class TextureIndexError : public TextureError {
        public:
            TextureIndexError() : TextureError("Texture Index Error") {}

            TextureIndexError(const std::string& message) : TextureError(message) {}
    };

    class TextureInitError : public TextureError {
        public:
            TextureInitError() : TextureError("Texture Initialization Error") {}

            TextureInitError(const std::string& message) : TextureError(message) {}
    };

    class TextureComponentError : public TextureError {
        public:
            TextureComponentError() : TextureError("Texture Component Error") {}

            TextureComponentError(const std::string& message) : TextureError(message) {}
    };

    class TextureFreeError : public TextureError {
        public:
            TextureFreeError() : TextureError("Texture Free Error") {}

            TextureFreeError(const std::string& message) : TextureError(message) {}
    };




    class BufferError : public std::runtime_error {

        public:
            BufferError() : std::runtime_error("Buffer Error") {}

            BufferError(const std::string& message) : std::runtime_error(message) {}

    };

    class BufferSizeError : public BufferError {
        public:
            BufferSizeError() : BufferError("Buffer Size Error") {}

            BufferSizeError(const std::string& message) : BufferError(message) {}
    };

    class BufferIndexError : public BufferError {
        public:
            BufferIndexError() : BufferError("Buffer Index Error") {}

            BufferIndexError(const std::string& message) : BufferError(message) {}
    };

    class BufferInitError : public BufferError {
        public:
            BufferInitError() : BufferError("Buffer Initialization Error") {}

            BufferInitError(const std::string& message) : BufferError(message) {}
    };

    class BufferFreeError : public BufferError {
        public:
            BufferFreeError() : BufferError("Buffer Free Error") {}

            BufferFreeError(const std::string& message) : BufferError(message) {}
    };

    class BufferComponentError : public BufferError {
        public:
            BufferComponentError() : BufferError("Buffer Component Error") {}

            BufferComponentError(const std::string& message) : BufferError(message) {}
    };




    class CommandManagerError : public std::runtime_error {

        public:
            CommandManagerError() : std::runtime_error("Command Manager Error") {}

            CommandManagerError(const std::string& message) : std::runtime_error(message) {}

    };

    class CommandManagerItemSizeError : public CommandManagerError {
        public:
            CommandManagerItemSizeError() : CommandManagerError("Command Manager Item Size Error") {}

            CommandManagerItemSizeError(const std::string& message) : CommandManagerError(message) {}
    };

    class CommandManagerLoadError : public CommandManagerError {
        public:
            CommandManagerLoadError() : CommandManagerError("Command Manager Load Error") {}

            CommandManagerLoadError(const std::string& message) : CommandManagerError(message) {}
    };

    class CommandManagerIndexError : public CommandManagerError {
        public:
            CommandManagerIndexError() : CommandManagerError("Command Manager Index Error") {}

            CommandManagerIndexError(const std::string& message) : CommandManagerError(message) {}
    };




    class KernelError : public std::runtime_error {

        public:
            KernelError() : std::runtime_error("Kernel Error") {}

            KernelError(const std::string& message) : std::runtime_error(message) {}

    };

    class KernelLoadError : public KernelError {
        public:
            KernelLoadError() : KernelError("Kernel Load Error") {}

            KernelLoadError(const std::string& message) : KernelError(message) {}
    };


}