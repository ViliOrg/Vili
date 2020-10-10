#include <catch/catch.hpp>

#include <vili/parser/parser.hpp>


TEST_CASE("Inline comment", "[vili.parser]")
{
    SECTION("Comment only")
    {
        REQUIRE_NOTHROW(vili::parser::from_string("# This is a comment"));

        REQUIRE_NOTHROW(vili::parser::from_string("# This is\n# multiple comments"));

        REQUIRE_NOTHROW(vili::parser::from_string("         # This is a comment with spaces and indents     "));
    }

    SECTION("Comment with code")
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

        REQUIRE(
            vili::parser::from_string(
                "# Multiple\n"
                "test: 42\n"
                "# comments")
        .at("test").as<vili::integer>() == 42);
    }

    SECTION("Multiline comment")
    {
        REQUIRE_NOTHROW(vili::parser::from_string("/* This is a multiline comment on one line */"));

        REQUIRE_NOTHROW(vili::parser::from_string("/* This is a\nmultiline\ncomment */"));

        REQUIRE_NOTHROW(vili::parser::from_string("/* This is an\nunclosed\ncomment"));
    }

    SECTION("Multiline comment with code")
    {
        REQUIRE(
            vili::parser::from_string("test: 42 /* comment on one line */")
        .at("test").as<vili::integer>() == 42);

        vili::node root = vili::parser::from_string(
                    "test1: 42 /* multiline\n"
                    "comments */\n"
                    "test2: 43");

        REQUIRE(root.at("test1").as<vili::integer>() == 42);
        REQUIRE(root.at("test2").as<vili::integer>() == 43);
    }
}