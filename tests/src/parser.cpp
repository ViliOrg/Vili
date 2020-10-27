#include <catch/catch.hpp>

#include <vili/parser.hpp>

TEST_CASE("Parsing empty string", "[vili.parser]")
{
    SECTION("Empty file")
    {
        vili::node root = vili::parser::from_string("");
        REQUIRE(root.dump(true).empty());
    }
}

TEST_CASE("Invalid data on root", "[vili.parser]")
{
    SECTION("Integer on root")
    {
        REQUIRE_THROWS_AS(
            vili::parser::from_string("22"), vili::exceptions::parsing_error);
    }
    SECTION("String on root")
    {
        REQUIRE_THROWS_AS(
            vili::parser::from_string("\"hello\""), vili::exceptions::parsing_error);
    }
    SECTION("Boolean on root")
    {
        REQUIRE_THROWS_AS(
            vili::parser::from_string("false"), vili::exceptions::parsing_error);
    }
    SECTION("Float on root")
    {
        REQUIRE_THROWS_AS(
            vili::parser::from_string("3.14"), vili::exceptions::parsing_error);
    }
    SECTION("Array on root")
    {
        REQUIRE_THROWS_AS(
            vili::parser::from_string("[1, 2, 3]"), vili::exceptions::parsing_error);
    }
    SECTION("Object on root")
    {
        REQUIRE_THROWS_AS(
            vili::parser::from_string("{}"), vili::exceptions::parsing_error);
    }
}

TEST_CASE("Unclosed containers", "[vili.parser]")
{
    SECTION("Unclosed list")
    {
        REQUIRE_THROWS_AS(
            vili::parser::from_string("a:[1, 2"), vili::exceptions::parsing_error);
    }
    SECTION("Unclosed object")
    {
        REQUIRE_THROWS_AS(
            vili::parser::from_string("a:{x: 2"), vili::exceptions::parsing_error);
    }
}

TEST_CASE("Unexpected symbols", "[vili.parser]")
{
    SECTION("Exclamation point after identifier")
    {
        REQUIRE_THROWS_AS(
            vili::parser::from_string("a: !"), vili::exceptions::parsing_error);
    }
}

TEST_CASE("Indentation", "[vili.parser]")
{
    SECTION("Unexpected indentation")
    {
        REQUIRE_THROWS_AS(vili::parser::from_string("a:\n    b:1\n        c:2\n    d:3"),
            vili::exceptions::parsing_error);
    }
    SECTION("Single space indentation")
    {
        REQUIRE_NOTHROW(vili::parser::from_string("a:\n b:1\n c:2\n d:3"));
    }
}