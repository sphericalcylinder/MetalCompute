find_library(FOUNDATION REQUIRED Foundation)
if (NOT FOUNDATION)
    message(FATAL_ERROR "Foundation not found")
endif()