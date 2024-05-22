find_library(METALKIT REQUIRED MetalKit)
if (NOT METALKIT)
    message(FATAL_ERROR "MetalKit not found")
endif()