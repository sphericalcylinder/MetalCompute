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

add_custom_target(testingmetal
    COMMAND xcrun -sdk macosx metal -c ${CMAKE_SOURCE_DIR}/src/default.metal -o ${CMAKE_CURRENT_BINARY_DIR}/default.air
    COMMAND xcrun -sdk macosx metallib ${CMAKE_CURRENT_BINARY_DIR}/default.air -o ${CMAKE_CURRENT_BINARY_DIR}/default.metallib
    DEPENDS ${CMAKE_SOURCE_DIR}/src/default.metal
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Compiling Metal shader for testing"
    VERBATIM
)

file(GLOB TESTFILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

foreach(TESTFILE ${TESTFILES})
    get_filename_component(TESTNAME ${TESTFILE} NAME_WE)
    message(STATUS "Adding test: ${TESTNAME}")

    add_executable(${TESTNAME} ${TESTFILE})
    list(APPEND TESTLIST ${TESTNAME})

    target_include_directories(${TESTNAME} PRIVATE ${CMAKE_SOURCE_DIR}/src ${metalcpp_SOURCE_DIR} ${DOCTEST_INCLUDE_DIR})
    target_link_libraries(${TESTNAME} PRIVATE ${METAL} ${METALKIT} ${FOUNDATION})
    target_link_libraries(${TESTNAME} PRIVATE doctest::doctest)
    add_dependencies(${TESTNAME} testingmetal)

    #add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
    string(REPLACE "Test" "" TESTPREFIX ${TESTNAME})
    string(APPEND TESTPREFIX "---")
    doctest_discover_tests(${TESTNAME} TEST_PREFIX ${TESTPREFIX})

endforeach()

# add all tests to a custom target
add_custom_target(alltests DEPENDS ${TESTLIST})
