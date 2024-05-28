
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

file(GLOB SOURCEFILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

foreach(SOURCEFILE ${SOURCEFILES})

    get_filename_component(EXENAME ${SOURCEFILE} NAME_WE)
    message(STATUS "Adding executable ${EXENAME}")

    add_executable(${EXENAME} ${SOURCEFILE})
    
    add_dependencies(${EXENAME} metal)
    target_include_directories(${EXENAME} PRIVATE ${metalcpp_SOURCE_DIR})
    target_link_libraries(${EXENAME} PRIVATE ${METAL} ${METALKIT} ${FOUNDATION})

endforeach()


