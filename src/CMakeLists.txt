
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
target_include_directories(hellogpu PRIVATE ${metalcpp_SOURCE_DIR})

find_package(doctest REQUIRED)
if (NOT doctest_FOUND)
    message(FATAL_ERROR "Doctest not found!")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
