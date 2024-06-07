cmake_minimum_required(VERSION 3.19)

option(MTLCOMPUTE_BUILD_TESTS "Build the tests")
option(MTLCOMPUTE_INSTALL_TESTS "Install the tests")

option(MTLCOMPUTE_BUILD_DOCS "Build the documentation")
option(MTLCOMPUTE_INSTALL_DOCS "Install the documentation")

option(MTLCOMPUTE_BUILD_EXAMPLES "Build the example source files")
option(MTLCOMPUTE_INSTALL_EXAMPLES "Install the example source files")

if (MTLCOMPUTE_INSTALL_TESTS MATCHES ON OR MTLCOMPUTE_INSTALL_TESTS MATCHES TRUE)
    set(MTLCOMPUTE_BUILD_TESTS ON)
endif()

if (MTLCOMPUTE_INSTALL_DOCS MATCHES ON OR MTLCOMPUTE_INSTALL_DOCS MATCHES TRUE)
    set(MTLCOMPUTE_BUILD_DOCS ON)
endif()

if (MTLCOMPUTE_INSTALL_EXAMPLES MATCHES ON OR MTLCOMPUTE_INSTALL_EXAMPLES MATCHES TRUE)
    set(MTLCOMPUTE_BUILD_EXAMPLES ON)
endif()

project(MetalCompute VERSION 1.0
                     DESCRIPTION "An API to make GPU compute calls easier"
                     LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

enable_testing()
include(FetchContent)

# generate compile_commands.json
# https://stackoverflow.com/questions/23960835/cmake-not-generating-compile-commands-json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")


if (MTLCOMPUTE_BUILD_DOCS MATCHES ON OR MTLCOMPUTE_BUILD_DOCS MATCHES TRUE)
    find_package(Doxygen REQUIRED doxygen)
    if (Doxygen_FOUND)
        message(STATUS "Adding docs to build")
        add_subdirectory(docs)
    else()
        message(WARNING "Doxygen not found, not building docs")
    endif()
    if (MTLCOMPUTE_INSTALL_DOCS)
        install(DIRECTORY ${CMAKE_BINARY_DIR}/docs/html DESTINATION docs)
    endif()
endif()

# Fetch metal-cpp if it doesn't exist
get_filename_component(MTLCPPDIR "${CMAKE_BINARY_DIR}/_deps/metalcpp-src" REALPATH)
if (NOT EXISTS "${MTLCPPDIR}")
    message(STATUS "Fetching metal-cpp...")
    FetchContent_Declare(
        METALCPP
        GIT_REPOSITORY https://github.com/bkaradzic/metal-cpp.git
        GIT_TAG        a63bd17
    )
    FetchContent_MakeAvailable(METALCPP)
endif()
set(metalcpp_SOURCE_DIR "${MTLCPPDIR}")
include_directories(${metalcpp_SOURCE_DIR} ${metalcpp_SOURCE_DIR}/SingleHeader)

get_filename_component(SINGLEHEADER "${metalcpp_SOURCE_DIR}/SingleHeader/Metal.hpp" REALPATH)
if (NOT EXISTS "${SINGLEHEADER}")
    # run the script to generate the single header
    message(STATUS "Generating single header for metal-cpp")
    execute_process(COMMAND ${metalcpp_SOURCE_DIR}/SingleHeader/MakeSingleHeader.py 
                            ${metalcpp_SOURCE_DIR}/Metal/Metal.hpp 
                            ${metalcpp_SOURCE_DIR}/Foundation/Foundation.hpp 
                            WORKING_DIRECTORY ${metalcpp_SOURCE_DIR}
                    )
endif()



add_subdirectory(src)

if (MTLCOMPUTE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (MTLCOMPUTE_BUILD_TESTS)

    message(STATUS "Adding tests to build")
    find_package(doctest QUIET)
    if (NOT doctest_FOUND)

        get_filename_component(DOCTESTDIR "${CMAKE_BINARY_DIR}/_deps/doctest-src" REALPATH)
        if (NOT EXISTS "${DOCTESTDIR}")
            message(STATUS "Doctest not found, fetching...")
            message(STATUS "Ignore the warning about the doctest version being too old, it's fine.")
            FetchContent_Declare(
            doctest
            GIT_REPOSITORY https://github.com/doctest/doctest.git
            GIT_TAG v2.4.11
            )
            FetchContent_MakeAvailable(doctest)
            list(APPEND CMAKE_MODULE_PATH "${doctest_SOURCE_DIR}/scripts/cmake/")
        else()
            message(STATUS "Doctest found cached in ${DOCTESTDIR}")
            set(doctest_SOURCE_DIR "${DOCTESTDIR}")
        endif()

        include_directories(${doctest_SOURCE_DIR})
        include(doctest)

    else()
        message(STATUS "Doctest found")
        include(doctest)
    endif()

    add_subdirectory(test)

endif()
