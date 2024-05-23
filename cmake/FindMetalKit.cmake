find_library(METALKIT MetalKit)
if (NOT METALKIT)
    message(FATAL_ERROR "MetalKit not found")
endif()