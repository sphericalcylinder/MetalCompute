find_library(FOUNDATION Foundation)
if (NOT FOUNDATION)
    message(FATAL_ERROR "Foundation not found")
endif()