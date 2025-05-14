# Errors

There are currently 22 errors defined in the MTLCompute::Errors namespace in the `MTLComputeErrors.hpp` file. These stem from 5 base error types. These types are:
- [TextureError](\ref MTLCompute::Error::TextureError)
- [BufferError](\ref MTLCompute::Error::BufferError)
- [CommandManagerError](\ref MTLCompute::Error::CommandManagerError)
- [KernelError](\ref MTLCompute::Error::KernelError)
- [ConversionError](\ref MTLCompute::Error::ConversionError)

There is really no difference between these except for their names. This is because it's much easier to diagnose a broken program that says
'**uncaught exception of type MTLCompute::Error::KernelLoadError**'
than a program that says
'**uncaught exception of type MTLCompute::WhoopsThereWasAnError**' 
or something else unhelpful like that.

> [!TIP]
> You don't have to use the full `MTLCompute::Error::SomethingError` name when handling exceptions. Feel free to leave out that middle namespace, shortening it to `MTLCompute::SomethingError`

<br />

Whenever an error is thrown, it should be thrown with an accompanying error message that describes the problem in more detail, if possible.
If you experience otherwise, please file an issue in the GitHub repository of this project.