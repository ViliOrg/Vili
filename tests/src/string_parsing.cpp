
#include <catch2/catch_test_macros.hpp>

#include <vili/parser.hpp>

TEST_CASE("Incorrect strings", "[vili.parser]")
{
    SECTION("Unterminated string")
    {
        REQUIRE_THROWS_AS(vili::parser::from_string("untermined_string: \"Hello, world!"),
            vili::exceptions::parsing_error);
    }
}

TEST_CASE("Simple strings", "[vili.parser]")
{
    SECTION("Hello world")
    {
        REQUIRE(vili::parser::from_string("hello_world: \"Hello, world!\"")["hello_world"]
                    .as<vili::string>()
            == "Hello, world!");
    }
    SECTION("Escape quotes")
    {
        std::string quote = "\\\"E = mc^2\\\"";
        REQUIRE(vili::parser::from_string(
                    "escape: \"Einstein said: " + quote + "\"")["escape"]
                    .as<vili::string>()
            == "Einstein said: " + quote);
    }
}
