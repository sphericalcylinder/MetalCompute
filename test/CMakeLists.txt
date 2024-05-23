# require old policy to allow for source files containing the name "test"
set(CMAKE_WARN_DEPRECATED FALSE)
cmake_policy(SET CMP0037 OLD)

project(Tests VERSION 1.0
        DESCRIPTION "All the tests"
        LANGUAGES CXX
)

# set c++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# get frameworks
include(FindMetal)
include(FindMetalKit)
include(FindFoundation)

file(GLOB TESTFILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

foreach(TESTFILE ${TESTFILES})
    get_filename_component(TESTNAME ${TESTFILE} NAME_WE)
    message(STATUS "Adding test: ${TESTNAME}")

    add_executable(${TESTNAME} ${TESTFILE})

    target_include_directories(${TESTNAME} PRIVATE ${CMAKE_SOURCE_DIR}/src ${metalcpp_SOURCE_DIR})
    target_link_libraries(${TESTNAME} PUBLIC doctest::doctest)
    target_link_libraries(${TESTNAME} PRIVATE ${METAL} ${METALKIT} ${FOUNDATION})

    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})

endforeach()

