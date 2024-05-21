cmake_minimum_required(VERSION 3.19)

project(MetalCompute VERSION 1.0
                     DESCRIPTION "An API to make GPU compute calls easier"
                     LANGUAGES CXX)

set(DIR "MetalCompute")

# metal library

add_custom_target(metal
    COMMAND xcrun -sdk macosx metal -c ${DIR}/default.metal -o ${CMAKE_CURRENT_BINARY_DIR}/default.air
    COMMAND xcrun -sdk macosx metallib ${CMAKE_CURRENT_BINARY_DIR}/default.air -o ${CMAKE_CURRENT_BINARY_DIR}/default.metallib
    DEPENDS ${DIR}/default.metal
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Compiling Metal shader"
    VERBATIM
)

add_executable(hellogpu ${DIR}/hellogpu.cpp)
add_dependencies(hellogpu metal)
target_compile_features(hellogpu PUBLIC cxx_std_20)
set_target_properties(hellogpu PROPERTIES CXX_EXTENSIONS OFF)

find_library(METAL Metal)
find_library(METALKIT MetalKit)
find_library(FOUNDATION Foundation)

if (NOT METAL)
    message(FATAL_ERROR "Metal not found")
endif()

if (NOT METALKIT)
    message(FATAL_ERROR "MetalKit not found")
endif()

if (NOT FOUNDATION)
    message(FATAL_ERROR "Foundation not found")
endif()

target_include_directories(hellogpu PUBLIC metal-cpp)
target_link_libraries(hellogpu ${METAL} ${METALKIT} ${FOUNDATION})
