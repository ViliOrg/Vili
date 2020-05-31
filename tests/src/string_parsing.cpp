#include <catch/catch.hpp>

#include <vili/parser/parser.hpp>

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
    SECTION("Multiline")
    {
        REQUIRE(vili::parser::from_string(
                    "multiline: \"Line 1\nLine 2\nLine 3\"")["multiline"]
                    .as<vili::string>()
            == "Line 1\nLine 2\nLine 3");
    }
    SECTION("Escape quotes")
    {
        REQUIRE(vili::parser::from_string(
                    "escape: \"Einstein said: \\\"E = mc^2\\\"\"")["escape"]
                    .as<vili::string>()
            == "Einstein said \"E = mc^2\"");
    }
}