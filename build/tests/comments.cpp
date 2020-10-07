#include <catch/catch.hpp>

#include <vili/parser/parser.hpp>


TEST_CASE("Inline comment", "[vili.parser]")
{
    SECTION("Comment only")
    {
        REQUIRE_NOTHROW(vili::parser::from_string("# This is a comment"));
    }

    SECTION("Comment and any")
    {
        REQUIRE(
            vili::parser::from_string("test: 42# This is a comment")
        .at("test").as<vili::integer>() == 42);

        REQUIRE(
            vili::parser::from_string("test: 42     #    This is a comment with spaces and indents")
        .at("test").as<vili::integer>() == 42);

        REQUIRE(
            vili::parser::from_string("test: 42 ## Comment with # multiples '#'")
        .at("test").as<vili::integer>() == 42);
    }
}