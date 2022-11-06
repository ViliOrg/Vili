
#include <catch2/catch_test_macros.hpp>

#include <vili/parser.hpp>

TEST_CASE("Merge")
{
    SECTION("Merge and redefine")
    {
        vili::node root
            = vili::parser::from_string("one:{two:{x:1}, z:1}\none:{two:{y:2}, z:3}");
        REQUIRE(root["one"].size() == 2);
        REQUIRE(root["one"]["two"].size() == 2);
        REQUIRE(root["one"]["two"]["x"].as<vili::integer>() == 1);
        REQUIRE(root["one"]["two"]["y"].as<vili::integer>() == 2);
        REQUIRE(root["one"]["z"].as<vili::integer>() == 3);
    }
}
