project(Test VERSION 1.0
        DESCRIPTION "Test for testing"
        LANGUAGES CXX
)

find_package(doctest REQUIRED)
if (NOT doctest_FOUND)
    message(FATAL_ERROR "Doctest not found!")
endif()

add_executable(mytest test.cpp)
target_compile_features(mytest PUBLIC cxx_std_20)
set_target_properties(mytest PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(mytest PRIVATE doctest::doctest)

add_test(NAME mytest COMMAND mytest)
