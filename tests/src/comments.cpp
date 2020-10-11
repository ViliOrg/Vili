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

        REQUIRE(
            vili::parser::from_string(
                "test: [42,# comment with array\n"
                "43, # comment with array\n"
                "44]# comment with array")
        .at("test").as<vili::array>() == vili::array{42, 43, 44});

        vili::node root = vili::parser::from_string(
            "test: {int1: 42, # comment with object\n"
            "tab1: [43, 44], # comment with object\n"
            "int2: 45}# comment with object");
        REQUIRE(root["test"]["int1"].as<vili::integer>() == 42);
        REQUIRE(root["test"]["tab1"].as<vili::array>() == vili::array{43, 44});
        REQUIRE(root["test"]["int2"].as<vili::integer>() == 45);
    }

    SECTION("Multiline comment")
    {
        REQUIRE_NOTHROW(vili::parser::from_string("/* This is a multiline comment on one line */"));

        REQUIRE_NOTHROW(vili::parser::from_string("/* This is a\nmultiline\ncomment */"));

        REQUIRE_NOTHROW(vili::parser::from_string("/* This is an\nunclosed\ncomment"));

        REQUIRE_NOTHROW(
            vili::parser::from_string("/*This is\n"
            "a /* nested */ multiline\n"
            "comment*/"));
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
        REQUIRE(root["test1"].as<vili::integer>() == 42);
        REQUIRE(root["test2"].as<vili::integer>() == 43);

        REQUIRE(
            vili::parser::from_string(
                "test: [42,/* multiline comment\n"
                "with array */43, /* one line comment */\n"
                "44] /* one line comment */")
        .at("test").as<vili::array>() == vili::array{42, 43, 44});

        root = vili::parser::from_string(
            "test: {int1: 42, /* multiline comment\n"
            "with object */tab1: [43, 44], /* one line comment */\n"
            "int2: 45}/* one line comment */");
        REQUIRE(root["test"]["int1"].as<vili::integer>() == 42);
        REQUIRE(root["test"]["tab1"].as<vili::array>() == vili::array{43, 44});
        REQUIRE(root["test"]["int2"].as<vili::integer>() == 45);
    }
}