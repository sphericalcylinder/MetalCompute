#pragma once

#include <MTLComputeGlobals.hpp>

namespace MTLCompute {
inline namespace Error {

/**
 * @brief The base class for all Texture errors.
 *
 */
class TextureError : public std::runtime_error {

  public:
    TextureError() : std::runtime_error("Texture Error") {}

    TextureError(const std::string &message) : std::runtime_error(message) {}

};

/**
 * @brief An error that occurs when the texture size is invalid.
 *
 */
class TextureSizeError : public TextureError {
  public:
    TextureSizeError() : TextureError("Texture Size Error") {}

    TextureSizeError(const std::string &message) : TextureError(message) {}
};

/**
 * @brief An error that occurs when the texture item type is invalid.
 *
 */
class TextureTypeError : public TextureError {
  public:
    TextureTypeError() : TextureError("Texture Item Type Error") {}

    TextureTypeError(const std::string &message) : TextureError(message) {}
};

/**
 * @brief An error that occurs when the texture index is invalid.
 *
 */
class TextureIndexError : public TextureError {
  public:
    TextureIndexError() : TextureError("Texture Index Error") {}

    TextureIndexError(const std::string &message) : TextureError(message) {}
};

/**
 * @brief An error that occurs if the texture has not been initialized.
 *
 */
class TextureInitError : public TextureError {
  public:
    TextureInitError() : TextureError("Texture Initialization Error") {}

    TextureInitError(const std::string &message) : TextureError(message) {}
};

/**
 * @brief actually not sure what this is for; it's not used in the code
 *
 */
class TextureComponentError : public TextureError {
  public:
    TextureComponentError() : TextureError("Texture Component Error") {}

    TextureComponentError(const std::string &message) : TextureError(message) {}
};

/**
 * @brief An error that occurs when the texture has already been freed.
 *
 */
class TextureFreeError : public TextureError {
  public:
    TextureFreeError() : TextureError("Texture Free Error") {}

    TextureFreeError(const std::string &message) : TextureError(message) {}
};

/**
 * @brief The base class for all Buffer errors.
 *
 */
class BufferError : public std::runtime_error {

  public:
    BufferError() : std::runtime_error("Buffer Error") {}

    BufferError(const std::string &message) : std::runtime_error(message) {}
};

/**
 * @brief An error that occurs if the size of the data does not match the size of the buffer.
 *
 */
class BufferSizeError : public BufferError {
  public:
    BufferSizeError() : BufferError("Buffer Size Error") {}

    BufferSizeError(const std::string &message) : BufferError(message) {}
};

/**
 * @brief An error that occurs if an index is out of bounds.
 *
 */
class BufferIndexError : public BufferError {
  public:
    BufferIndexError() : BufferError("Buffer Index Error") {}

    BufferIndexError(const std::string &message) : BufferError(message) {}
};

/**
 * @brief An error that occurs if the buffer has not been initialized.
 *
 */
class BufferInitError : public BufferError {
  public:
    BufferInitError() : BufferError("Buffer Initialization Error") {}

    BufferInitError(const std::string &message) : BufferError(message) {}
};

/**
 * @brief An error that occurs if the buffer has already been freed.
 *
 */
class BufferFreeError : public BufferError {
  public:
    BufferFreeError() : BufferError("Buffer Free Error") {}

    BufferFreeError(const std::string &message) : BufferError(message) {}
};

/**
 * @brief Why is there another one of these? They're useless
 *
 */
class BufferComponentError : public BufferError {
  public:
    BufferComponentError() : BufferError("Buffer Component Error") {}

    BufferComponentError(const std::string &message) : BufferError(message) {}
};

/**
 * @brief The base class for all CommandManager errors.
 *
 */
class CommandManagerError : public std::runtime_error {

  public:
    CommandManagerError() : std::runtime_error("Command Manager Error") {}

    CommandManagerError(const std::string &message)
        : std::runtime_error(message) {}
};

/**
 * @brief An error that occurs if the size of the data put in is inconsistent.
 *
 */
class CommandManagerItemSizeError : public CommandManagerError {
  public:
    CommandManagerItemSizeError()
        : CommandManagerError("Command Manager Item Size Error") {}

    CommandManagerItemSizeError(const std::string &message)
        : CommandManagerError(message) {}
};

/**
 * @brief An error that occurs if there is no data loaded into the CommandManager.
 *
 */
class CommandManagerLoadError : public CommandManagerError {
  public:
    CommandManagerLoadError()
        : CommandManagerError("Command Manager Load Error") {}

    CommandManagerLoadError(const std::string &message)
        : CommandManagerError(message) {}
};

/**
 * @brief An error that occurs if the index to access data in the CommandManager is out of bounds.
 *
 */
class CommandManagerIndexError : public CommandManagerError {
  public:
    CommandManagerIndexError()
        : CommandManagerError("Command Manager Index Error") {}

    CommandManagerIndexError(const std::string &message)
        : CommandManagerError(message) {}
};

/**
 * @brief The base class for all Kernel errors.
 *
 */
class KernelError : public std::runtime_error {

  public:
    KernelError() : std::runtime_error("Kernel Error") {}

    KernelError(const std::string &message) : std::runtime_error(message) {}
};

/**
 * @brief An error that occurs if the kernel cannot load a library or function.
 *
 */
class KernelLoadError : public KernelError {
  public:
    KernelLoadError() : KernelError("Kernel Load Error") {}

    KernelLoadError(const std::string &message) : KernelError(message) {}
};

/**
 * @brief The base class for all conversion errors.
 *
 */
class ConversionError : public std::runtime_error {

  public:
    ConversionError() : std::runtime_error("Conversion Error") {}

    ConversionError(const std::string &message) : std::runtime_error(message) {}
};

/**
 * @brief An error that occurs if the type being converted to is invalid.
 *
 */
class ConversionTypeError : public ConversionError {
  public:
    ConversionTypeError() : ConversionError("Conversion Component Error") {}

    ConversionTypeError(const std::string &message)
        : ConversionError(message) {}
};

/**
 * @brief An error that occurs if the size of the data being converted is too large.
 *
 */
class ConversionSizeError : public ConversionError {
  public:
    ConversionSizeError() : ConversionError("Conversion Size Error") {}

    ConversionSizeError(const std::string &message)
        : ConversionError(message) {}
};

} // namespace Error
} // namespace MTLCompute