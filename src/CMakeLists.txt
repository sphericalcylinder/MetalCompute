
include(FindFoundation)
include(FindMetal)
include(FindMetalKit)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
add_library(mtlcompute SHARED ${HEADERS})
set_target_properties(mtlcompute PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(mtlcompute PROPERTIES CXX_STANDARD 17)
target_include_directories(mtlcompute PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${metalcpp_SOURCE_DIR})
target_link_libraries(mtlcompute PUBLIC ${METAL} ${METALKIT} ${FOUNDATION})

install(TARGETS mtlcompute DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/mtlcompute)