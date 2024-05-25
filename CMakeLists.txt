cmake_minimum_required(VERSION 3.19)

option(MTLCOMPUTE_BUILD_TESTS "Build the tests")
option(MTLCOMPUTE_BUILD_DOCS "Build the documentation")

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
        message(WARNING "Doxygen not found, not building docs")
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
    set(metalcpp_SOURCE_DIR "${MTLCPPDIR}/_deps/metalcpp-src")
endif()

add_subdirectory(src)

if (MTLCOMPUTE_BUILD_TESTS)

    message(STATUS "Adding tests to build")
    find_package(doctest QUIET)
    set(doctest_FOUND OFF)
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

        # include(ExternalProject)
        # find_package(Git REQUIRED)

        # ExternalProject_Add(
        #     doctest
        #     PREFIX ${CMAKE_BINARY_DIR}/test/doctest
        #     GIT_REPOSITORY https://github.com/doctest/doctest.git
        #     TIMEOUT 10
        #     UPDATE_COMMAND ${GIT_EXECUTABLE} pull
        #     CONFIGURE_COMMAND ""
        #     BUILD_COMMAND ""
        #     INSTALL_COMMAND ""
        #     LOG_DOWNLOAD ON
        # )

        # # Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
        # ExternalProject_Get_Property(doctest SOURCE_DIR)
        # set(DOCTEST_INCLUDE_DIR ${SOURCE_DIR}/doctest CACHE INTERNAL "Path to include folder for doctest")
        # ExternalProject_Get_Property(doctest DOWNLOAD_DIR)
        # set(DOCTEST_SCRIPT_DIR ${DOWNLOAD_DIR}/doctest CACHE INTERNAL "Path to include folder for doctest")
        # list(APPEND CMAKE_MODULE_PATH "${DOCTEST_INCLUDE_DIR}")
        # include(${DOCTEST_SCRIPT_DIR}/scripts/cmake/doctest.cmake)
    else()
        message(STATUS "Doctest found")
        include(doctest)
    endif()

    add_subdirectory(test)

endif()
