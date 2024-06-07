
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

install(FILES ${HEADERS} DESTINATION include/mtlcompute)

install(FILES ${metalcpp_SOURCE_DIR}/SingleHeader/Metal.hpp DESTINATION include/mtlcompute/)
