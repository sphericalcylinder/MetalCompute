
add_custom_target(metal
    COMMAND xcrun -sdk macosx metal -c default.metal -o ${CMAKE_CURRENT_BINARY_DIR}/default.air
    COMMAND xcrun -sdk macosx metallib ${CMAKE_CURRENT_BINARY_DIR}/default.air -o ${CMAKE_CURRENT_BINARY_DIR}/default.metallib
    DEPENDS default.metal
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Compiling Metal shader"
    VERBATIM
)

add_executable(hellogpu hellogpu.cpp)
add_dependencies(hellogpu metal)

include(FindMetal)
include(FindMetalKit)
include(FindFoundation)

target_include_directories(hellogpu PUBLIC ${metalcpp_SOURCE_DIR})

target_link_libraries(hellogpu ${METAL} ${METALKIT} ${FOUNDATION})