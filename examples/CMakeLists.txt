
add_custom_target(metal
    COMMAND xcrun -sdk macosx metal -c default.metal -o ${CMAKE_CURRENT_BINARY_DIR}/default.air
    COMMAND xcrun -sdk macosx metallib ${CMAKE_CURRENT_BINARY_DIR}/default.air -o ${CMAKE_CURRENT_BINARY_DIR}/default.metallib
    DEPENDS default.metal
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Compiling example Metal shader"
    VERBATIM
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB EXAMPLES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

foreach(EXAMPLE ${EXAMPLES})

    get_filename_component(EXENAME ${EXAMPLE} NAME_WE)
    message(STATUS "Adding example: ${EXENAME}")

    add_executable(${EXENAME} ${EXAMPLE})
    list(APPEND EXAMPLE_TARGETS ${EXENAME})
    
    add_dependencies(${EXENAME} metal)
    target_link_libraries(${EXENAME} PRIVATE "-framework Foundation" "-framework Metal" "-framework MetalKit")
    target_include_directories(${EXENAME} PUBLIC ${CMAKE_SOURCE_DIR}/src ${metalcpp_SOURCE_DIR})

endforeach()

add_custom_target(examples DEPENDS ${EXAMPLE_TARGETS})

if (MTLCOMPUTE_INSTALL_EXAMPLES)
    install(FILES ${EXAMPLES} DESTINATION examples)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/default.metal DESTINATION examples)
endif()

