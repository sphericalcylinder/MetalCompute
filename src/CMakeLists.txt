
include(FindFoundation)
include(FindMetal)
include(FindMetalKit)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
message(STATUS "Headers: ${HEADERS}")
add_library(mtlcompute SHARED ${HEADERS})
set_target_properties(mtlcompute PROPERTIES 
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      VERSION 1.0
                      SOVERSION 1
)

target_include_directories(mtlcompute PRIVATE .)

target_include_directories(mtlcompute PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${metalcpp_SOURCE_DIR}/SingleHeader)
target_link_libraries(mtlcompute PUBLIC "-framework Foundation" "-framework Metal" "-framework MetalKit")


install(TARGETS mtlcompute DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/mtlcompute)

install(FILES ${metalcpp_SOURCE_DIR}/SingleHeader/Metal.hpp DESTINATION include/mtlcompute/)
