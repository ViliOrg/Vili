#include <catch/catch.hpp>

#include <vili/parser/parser.hpp>

TEST_CASE("Array iterators")
{
    SECTION("Integer array")
    {
        long long int sum = 0;
        vili::node root = vili::parser::from_string("array: [2, 20, 200, 2000]");
        for (vili::node& number : root["array"])
        {
            sum += number.as<vili::integer>();
        }
        REQUIRE(sum == 2222);
    }
}

TEST_CASE("Object iterators")
{
    SECTION("Integer object")
    {
        std::string keys;
        long long int sum = 0;
        vili::node root
            = vili::parser::from_string("object: {a: 2, b: 20, c: 200, d: 2000}");
        for (auto [key, number] : root["object"].items())
        {
            keys += key;
            sum += number.as<vili::integer>();
        }
        REQUIRE(keys == "abcd");
        REQUIRE(sum == 2222);
    }
    SECTION("In const context")
    {
        std::string keys;
        long long int sum = 0;
        const vili::node root
            = vili::parser::from_string("object: {a: 2, b: 20, c: 200, d: 2000}");
        for (auto [key, number] : root.at("object").items())
        {
            keys += key;
            sum += number.as<vili::integer>();
        }
        REQUIRE(keys == "abcd");
        REQUIRE(sum == 2222);
    }
}