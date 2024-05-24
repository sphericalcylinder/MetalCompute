
add_custom_target(metal
    COMMAND xcrun -sdk macosx metal -c default.metal -o ${CMAKE_CURRENT_BINARY_DIR}/default.air
    COMMAND xcrun -sdk macosx metallib ${CMAKE_CURRENT_BINARY_DIR}/default.air -o ${CMAKE_CURRENT_BINARY_DIR}/default.metallib
    DEPENDS default.metal
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Compiling Metal shader"
    VERBATIM
)

# get frameworks
include(FindMetal)
include(FindMetalKit)
include(FindFoundation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(hellogpu hellogpu.cpp)
add_dependencies(hellogpu metal)
target_include_directories(hellogpu PRIVATE ${metalcpp_SOURCE_DIR})
target_link_libraries(hellogpu PRIVATE ${METAL} ${METALKIT} ${FOUNDATION})

add_executable(MTLComputeHello hellometalcompute.cpp)
add_dependencies(MTLComputeHello metal)
target_include_directories(MTLComputeHello PRIVATE ${metalcpp_SOURCE_DIR})
target_link_libraries(MTLComputeHello PRIVATE ${METAL} ${METALKIT} ${FOUNDATION})
