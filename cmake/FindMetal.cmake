find_library(METAL Metal)
if (NOT METAL)
    message(FATAL_ERROR "Metal not found")
endif()