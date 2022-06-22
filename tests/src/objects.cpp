#include <catch/catch.hpp>

#include <vili/parser.hpp>

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

    SECTION("Empty object")
    {
        vili::node root
            = vili::parser::from_string("object: {}");
        REQUIRE(root["object"].empty());
    }
}

TEST_CASE("Multiline object")
{
    SECTION("Empty object")
    {
        vili::node root = vili::parser::from_string(
            "object: {\n"
            "   \n"
            "  }");
        REQUIRE(root["object"].empty());
    }

    SECTION("Comma separator")
    {
        vili::node root = vili::parser::from_string(
            "object: {\n"
            "a: 15, b: [16, 17], c: \"18\"\n"
            "}");
        REQUIRE(root["object"]["a"].as<vili::integer>() == 15);
        REQUIRE(root["object"]["b"].as<vili::array>() == vili::array{ 16, 17 });
        REQUIRE(root["object"]["c"].as<vili::string>() == "18");
    }

    SECTION("Indent based")
    {
        vili::node root = vili::parser::from_string(
            "object:\n"
            "   a: 15\n"
            "   b: [16, 17]\n"
            "   c: \"18\"\n");
        REQUIRE(root["object"]["a"].as<vili::integer>() == 15);
        REQUIRE(root["object"]["b"].as<vili::array>() == vili::array{ 16, 17 });
        REQUIRE(root["object"]["c"].as<vili::string>() == "18");
    }

    SECTION("Comma with EOL and spaces")
    {
        vili::node root = vili::parser::from_string(
            "object:     {  \n"
            "    a: 15  \n"
            "      ,      \n"
            "b: [16, 17]  \n"
            "\n"
            " ,  c: \"18\"       \n"
            "}");
        REQUIRE(root["object"]["a"].as<vili::integer>() == 15);
        REQUIRE(root["object"]["b"].as<vili::array>() == vili::array{ 16, 17 });
        REQUIRE(root["object"]["c"].as<vili::string>() == "18");
    }

}

TEST_CASE("Nested objects")
{
    SECTION("Brace based object inside brace based object")
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

    SECTION("Brace based object inside indent based object")
    {
        vili::node root = vili::parser::from_string(
            "object:\n"
            "   a: 15\n"
            "   b: {\n"
            "       a: 16,\n"
            "   b: 17}\n"
            "   c: \"18\"\n");
        REQUIRE(root["object"]["a"].as<vili::integer>() == 15);
        REQUIRE(root["object"]["b"]["a"].as<vili::integer>() == 16);
        REQUIRE(root["object"]["b"]["b"].as<vili::integer>() == 17);
        REQUIRE(root["object"]["c"].as<vili::string>() == "18");
    }

    SECTION("Indent based object inside indent based object")
    {
        vili::node root = vili::parser::from_string(
            "object:\n"
            "   a: 15\n"
            "   b:\n"
            "      a: 16\n"
            "      b: 17\n"
            "   c: \"18\"\n");
        REQUIRE(root["object"]["a"].as<vili::integer>() == 15);
        REQUIRE(root["object"]["b"]["a"].as<vili::integer>() == 16);
        REQUIRE(root["object"]["b"]["b"].as<vili::integer>() == 17);
        REQUIRE(root["object"]["c"].as<vili::string>() == "18");
    }

    SECTION("Brace based object inside array")
    {
        vili::node root = vili::parser::from_string(
            "object:\n"
            "   a: [\n"
            "       {b: 1, c: 2},\n"
            "       {d: 3, e: 4}\n"
            "      ]\n"
            "   h: [\n"
            "       {f: 5, g: 6},\n"
            "       {h: 7, i: 8}\n"
            "   ]"
        );
        REQUIRE(root["object"]["a"][0u]["b"].as<vili::integer>() == 1);
        REQUIRE(root["object"]["a"][0u]["c"].as<vili::integer>() == 2);
        REQUIRE(root["object"]["a"][1u]["d"].as<vili::integer>() == 3);
        REQUIRE(root["object"]["a"][1u]["e"].as<vili::integer>() == 4);
        REQUIRE(root["object"]["h"][0u]["f"].as<vili::integer>() == 5);
        REQUIRE(root["object"]["h"][0u]["g"].as<vili::integer>() == 6);
        REQUIRE(root["object"]["h"][1u]["h"].as<vili::integer>() == 7);
        REQUIRE(root["object"]["h"][1u]["i"].as<vili::integer>() == 8);
    }
}

TEST_CASE("Incorrect object")
{
    SECTION("Missing closing bracket")
    {
        REQUIRE_THROWS_AS(vili::parser::from_string(
            "object: {\n"
            "a: 15,\n"
            "b: [16, 17]\n"
            "c: \"18\"\n"),
        vili::exceptions::parsing_error);
    }

    SECTION("Missing opening bracket")
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

    SECTION("No comma")
    {
        REQUIRE_THROWS_AS(vili::parser::from_string(
            "object: {\n"
            "a: 15\n"
            "b: [16, 17]\n"
            "c: \"18\"\n"
            "}"),
        vili::exceptions::parsing_error);
    }

    SECTION("Newline before opening bracket")
    {
        REQUIRE_THROWS_AS(vili::parser::from_string(
            "object: \n"
            "{\n"
            "a: 15,\n"
            "b: [16, 17] c: \"18\",\n"
            "}"),
        vili::exceptions::parsing_error);
    }

    SECTION("Mixed separator")
    {
        REQUIRE_THROWS_AS(vili::parser::from_string(
            "object: {\n"
            "a: 15,\n"
            "b: [16, 17]\n"
            "c: \"18\"\n"
            "}"),
        vili::exceptions::parsing_error);
    }

    SECTION("Wrong indent")
    {
        REQUIRE_THROWS_AS(vili::parser::from_string(
            "object:\n"
            "   a: 15\n"
            "    b: [16, 17]\n"
            "   c: \"18\"\n"),
        vili::exceptions::parsing_error);
    }
}