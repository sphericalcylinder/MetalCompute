#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <iostream>

/**
 * @brief A function that returns true (for testing purposes)
 *
 * @return bool
 *
*/
bool returntrue() {
    return true;
}

/**
 * @brief A function that returns x - 1
 *
 * @param x
 * @return int
 *
*/
int minusone(int x) {
    return x - 1;
}

TEST_CASE("Test returntrue") {
    CHECK(returntrue());
    CHECK(minusone(10) == 9);
    CHECK(minusone(0) == -1);
    CHECK_FALSE(minusone(0) == 0);
}