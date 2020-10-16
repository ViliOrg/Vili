#include <catch/catch.hpp>

#include <vili/parser/parser.hpp>

TEST_CASE("Inline object")
{
    SECTION("Simple object")
    {
        vili::node root
            = vili::parser::from_string("object: {a: 15, b: [16, 17], c: \"18\"}");
        REQUIRE(root["object"]["a"].as<vili::integer>() == 15);
        REQUIRE(root["object"]["b"].as<vili::array>() == vili::array { 16, 17 });
        REQUIRE(root["object"]["c"].as<vili::string>() == "18");
    }
}

TEST_CASE("Multiline object")
{
    SECTION("Comma separator")
    {
        vili::node root = vili::parser::from_string(
            "object: { a: 15, b: [16, 17], c: \"18\"}");
        REQUIRE(root["object"]["a"].as<vili::integer>() == 15);
        REQUIRE(root["object"]["b"].as<vili::array>() == vili::array{ 16, 17 });
        REQUIRE(root["object"]["c"].as<vili::string>() == "18");
    }

    SECTION("Newline separator")
    {
        vili::node root = vili::parser::from_string(
            "object: {\n"
            "a: 15\n"
            "b: [16, 17]\n"
            "c: \"18\"\n"
            "}");
        REQUIRE(root["object"]["a"].as<vili::integer>() == 15);
        REQUIRE(root["object"]["b"].as<vili::array>() == vili::array{ 16, 17 });
        REQUIRE(root["object"]["c"].as<vili::string>() == "18");
    }

    SECTION("Mixed separator")
    {
        vili::node root = vili::parser::from_string(
            "object: {\n"
            "a: 15,\n"
            "b: [16, 17]\n"
            "c: \"18\"\n"
            "}");
        REQUIRE(root["object"]["a"].as<vili::integer>() == 15);
        REQUIRE(root["object"]["b"].as<vili::array>() == vili::array{ 16, 17 });
        REQUIRE(root["object"]["c"].as<vili::string>() == "18");
    }

    SECTION("Eol and spaces")
    {
        vili::node root = vili::parser::from_string(
            "object: {\n"
            "a: 15,\n"
            "b: [16, 17]\n"
            "c: \"18\"\n"
            "}");
        REQUIRE(root["object"]["a"].as<vili::integer>() == 15);
        REQUIRE(root["object"]["b"].as<vili::array>() == vili::array{ 16, 17 });
        REQUIRE(root["object"]["c"].as<vili::string>() == "18");
    }

}

TEST_CASE("Nested objects")
{
    SECTION("One object inside object")
    {
        vili::node root = vili::parser::from_string(
            "object: {\n"
            "a: 15,\n"
            "b: {\n"
            "a: 16, b: 17},\n"
            "c: \"18\"\n"
            "}");
        REQUIRE(root["object"]["a"].as<vili::integer>() == 15);
        REQUIRE(root["object"]["b"]["a"].as<vili::integer>() == 16);
        REQUIRE(root["object"]["b"]["b"].as<vili::integer>() == 17);
        REQUIRE(root["object"]["c"].as<vili::string>() == "18");
    }
}

TEST_CASE("Incorrect object")
{
    SECTION("Missing end bracket")
    {
        REQUIRE_THROWS_AS(vili::parser::from_string(
            "object: {\n"
            "a: 15,\n"
            "b: [16, 17]\n"
            "c: \"18\"\n"),
        vili::exceptions::parsing_error);
    }

    SECTION("Missing start bracket")
    {
        REQUIRE_THROWS_AS(vili::parser::from_string(
            "object: \n"
            "a: 15,\n"
            "b: [16, 17]\n"
            "c: \"18\"\n"
            "}"),
        vili::exceptions::parsing_error);
    }

    SECTION("Comma after last element")
    {
        REQUIRE_THROWS_AS(vili::parser::from_string(
            "object: {\n"
            "a: 15,\n"
            "b: [16, 17]\n"
            "c: \"18\",\n"
            "}"),
        vili::exceptions::parsing_error);
    }

    SECTION("Missing comma")
    {
        REQUIRE_THROWS_AS(vili::parser::from_string(
            "object: {\n"
            "a: 15,\n"
            "b: [16, 17] c: \"18\",\n"
            "}"),
        vili::exceptions::parsing_error);
    }
}