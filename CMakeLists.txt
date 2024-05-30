cmake_minimum_required(VERSION 3.19)

option(MTLCOMPUTE_BUILD_TESTS "Build the tests")
option(MTLCOMPUTE_BUILD_DOCS "Build the documentation")
option(MTLCOMPUTE_INSTALL_DOCS "Install the documentation")
option(MTLCOMPUTE_INSTALL_EXAMPLES "Install the example source files")
option(MTLCOMPUTE_INSTALL_TESTS "Install the tests")

project(MetalCompute VERSION 1.0
                     DESCRIPTION "An API to make GPU compute calls easier"
                     LANGUAGES CXX)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

enable_testing()
include(FetchContent)

# generate compile_commands.json
# https://stackoverflow.com/questions/23960835/cmake-not-generating-compile-commands-json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# get_filename_component(JSONFILE "${CMAKE_BINARY_DIR}/compile_commands.json" REALPATH)
# if (EXISTS "${JSONFILE}")
#     # add the compile_commands.json to the src directory
#     configure_file(${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/src/compile_commands.json COPYONLY)
#     # add the compile_commands.json to the test directory
#     configure_file(${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/test/compile_commands.json COPYONLY)
# endif()


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
else()
    set(metalcpp_SOURCE_DIR "${MTLCPPDIR}")
endif()

add_subdirectory(src)
add_subdirectory(examples)

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
