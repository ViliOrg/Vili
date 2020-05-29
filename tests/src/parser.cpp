#include <catch/catch.hpp>

#include <vili/parser/parser.hpp>

TEST_CASE("Parsing empty string", "[vili.parser]")
{
    SECTION("Empty file")
    {
        vili::node root = vili::parser::from_string("");
        REQUIRE(root.dump(true) == "");
    }
}

TEST_CASE("Invalid data on root", "[vili.parser]")
{
    SECTION("Integer on root")
    {
        REQUIRE_THROWS_AS(
            vili::parser::from_string("22"), vili::exceptions::invalid_data_type);
    }
    SECTION("String on root")
    {
        REQUIRE_THROWS_AS(
            vili::parser::from_string("\"hello\""), vili::exceptions::invalid_data_type);
    }
    SECTION("Boolean on root")
    {
        REQUIRE_THROWS_AS(
            vili::parser::from_string("false"), vili::exceptions::invalid_data_type);
    }
    SECTION("Float on root")
    {
        REQUIRE_THROWS_AS(
            vili::parser::from_string("3.14"), vili::exceptions::invalid_data_type);
    }
    SECTION("Array on root")
    {
        REQUIRE_THROWS_AS(
            vili::parser::from_string("[1, 2, 3]"), vili::exceptions::invalid_data_type);
    }
    SECTION("Object on root")
    {
        REQUIRE_THROWS_AS(
            vili::parser::from_string("{}"), vili::exceptions::invalid_data_type);
    }
}