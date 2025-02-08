#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

bool is_positive(int value) {
    return value > 0;
}

TEST_CASE("Custom assertion test", "[custom]") {
    REQUIRE(is_positive(5) == true);   // Custom check for positive number
    REQUIRE(is_positive(-1) == false);  // Custom check for negative number
}
