find_library(METAL REQUIRED Metal)
if (NOT METAL)
    message(FATAL_ERROR "Metal not found")
endif()