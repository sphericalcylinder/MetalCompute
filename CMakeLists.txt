cmake_minimum_required(VERSION 3.19)

option(MTLCOMPUTE_BUILD_TESTING "Build the tests" ON)
option(MTLCOMPUTE_BUILD_DOCS "Build the documentation" OFF)

project(MetalCompute VERSION 1.0
                     DESCRIPTION "An API to make GPU compute calls easier"
                     LANGUAGES CXX)


if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    include(CTest)
    include(FetchContent)

    # generate compile_commands.json
    # https://stackoverflow.com/questions/23960835/cmake-not-generating-compile-commands-json
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

    get_filename_component(JSONFILE "${CMAKE_BINARY_DIR}/compile_commands.json" REALPATH)
    if (EXISTS "${JSONFILE}")
        # add the compile_commands.json to the src directory
        configure_file(${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/src/compile_commands.json COPYONLY)
        # add the compile_commands.json to the test directory
        configure_file(${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/test/compile_commands.json COPYONLY)
    endif()


    if (MTLCOMPUTE_BUILD_DOCS MATCHES ON OR MTLCOMPUTE_BUILD_DOCS MATCHES TRUE)
        find_package(Doxygen REQUIRED doxygen)
        if (Doxygen_FOUND)
            message(STATUS "Adding docs to build")
            add_subdirectory(docs)
        else()
            message(FATAL_ERROR "Doxygen not found!")
        endif()
    endif()

    # Fetch metal-cpp if it doesn't exist
    get_filename_component(FETCHDIR "${CMAKE_BINARY_DIR}/_deps" REALPATH)
    if (NOT EXISTS "${FETCHDIR}")
        message(STATUS "Creating directory ${FETCHDIR}")
        FetchContent_Declare(
            METALCPP
            GIT_REPOSITORY https://github.com/bkaradzic/metal-cpp.git
            GIT_TAG        a63bd17
        )
        FetchContent_MakeAvailable(METALCPP)
    else()
        set(metalcpp_SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/metalcpp-src")
    endif()

    find_package(doctest REQUIRED)
    if (NOT doctest_FOUND)
        message(FATAL_ERROR "Doctest not found!")
    endif()


endif()

add_subdirectory(src)

if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MTLCOMPUTE_BUILD_TESTING)
    AND BUILD_TESTING)

    add_subdirectory(test)

endif()
