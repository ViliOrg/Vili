#include <limits>

#include <catch/catch.hpp>

#include <vili/parser.hpp>

TEST_CASE("Integers", "[vili.parser]")
{
    SECTION("Positive integers")
    {
        REQUIRE(
            vili::parser::from_string("integer: 959514")["integer"].as<vili::integer>()
            == 959514);
        REQUIRE(
            vili::parser::from_string("integer: 395530")["integer"].as<vili::integer>()
            == 395530);
        REQUIRE(
            vili::parser::from_string("integer: 987524")["integer"].as<vili::integer>()
            == 987524);
        REQUIRE(
            vili::parser::from_string("integer: 489770")["integer"].as<vili::integer>()
            == 489770);
        REQUIRE(
            vili::parser::from_string("integer: 964384")["integer"].as<vili::integer>()
            == 964384);
        REQUIRE(
            vili::parser::from_string("integer: 545647")["integer"].as<vili::integer>()
            == 545647);
        REQUIRE(
            vili::parser::from_string("integer: 820595")["integer"].as<vili::integer>()
            == 820595);
        REQUIRE(
            vili::parser::from_string("integer: 438484")["integer"].as<vili::integer>()
            == 438484);
        REQUIRE(
            vili::parser::from_string("integer: 780794")["integer"].as<vili::integer>()
            == 780794);
        REQUIRE(vili::parser::from_string("integer: 636")["integer"].as<vili::integer>()
            == 636);
        REQUIRE(
            vili::parser::from_string("integer: 676762")["integer"].as<vili::integer>()
            == 676762);
        REQUIRE(
            vili::parser::from_string("integer: 856945")["integer"].as<vili::integer>()
            == 856945);
        REQUIRE(
            vili::parser::from_string("integer: 626870")["integer"].as<vili::integer>()
            == 626870);
        REQUIRE(vili::parser::from_string("integer: 75320")["integer"].as<vili::integer>()
            == 75320);
        REQUIRE(
            vili::parser::from_string("integer: 160500")["integer"].as<vili::integer>()
            == 160500);
        REQUIRE(
            vili::parser::from_string("integer: 170347")["integer"].as<vili::integer>()
            == 170347);
        REQUIRE(
            vili::parser::from_string("integer: 605915")["integer"].as<vili::integer>()
            == 605915);
        REQUIRE(
            vili::parser::from_string("integer: 397473")["integer"].as<vili::integer>()
            == 397473);
        REQUIRE(vili::parser::from_string("integer: 7592")["integer"].as<vili::integer>()
            == 7592);
        REQUIRE(
            vili::parser::from_string("integer: 813588")["integer"].as<vili::integer>()
            == 813588);
        REQUIRE(
            vili::parser::from_string("integer: 336345")["integer"].as<vili::integer>()
            == 336345);
        REQUIRE(
            vili::parser::from_string("integer: 223396")["integer"].as<vili::integer>()
            == 223396);
        REQUIRE(
            vili::parser::from_string("integer: 774227")["integer"].as<vili::integer>()
            == 774227);
        REQUIRE(
            vili::parser::from_string("integer: 813573")["integer"].as<vili::integer>()
            == 813573);
        REQUIRE(vili::parser::from_string("integer: 68926")["integer"].as<vili::integer>()
            == 68926);
        REQUIRE(
            vili::parser::from_string("integer: 458473")["integer"].as<vili::integer>()
            == 458473);
        REQUIRE(
            vili::parser::from_string("integer: 999263")["integer"].as<vili::integer>()
            == 999263);
        REQUIRE(
            vili::parser::from_string("integer: 102954")["integer"].as<vili::integer>()
            == 102954);
        REQUIRE(
            vili::parser::from_string("integer: 213609")["integer"].as<vili::integer>()
            == 213609);
        REQUIRE(
            vili::parser::from_string("integer: 433147")["integer"].as<vili::integer>()
            == 433147);
    }
    SECTION("Negative integers")
    {
        REQUIRE(
            vili::parser::from_string("integer: -865315")["integer"].as<vili::integer>()
            == -865315);
        REQUIRE(
            vili::parser::from_string("integer: -833080")["integer"].as<vili::integer>()
            == -833080);
        REQUIRE(
            vili::parser::from_string("integer: -743043")["integer"].as<vili::integer>()
            == -743043);
        REQUIRE(
            vili::parser::from_string("integer: -344880")["integer"].as<vili::integer>()
            == -344880);
        REQUIRE(
            vili::parser::from_string("integer: -837413")["integer"].as<vili::integer>()
            == -837413);
        REQUIRE(
            vili::parser::from_string("integer: -28265")["integer"].as<vili::integer>()
            == -28265);
        REQUIRE(
            vili::parser::from_string("integer: -909992")["integer"].as<vili::integer>()
            == -909992);
        REQUIRE(
            vili::parser::from_string("integer: -504758")["integer"].as<vili::integer>()
            == -504758);
        REQUIRE(
            vili::parser::from_string("integer: -823220")["integer"].as<vili::integer>()
            == -823220);
        REQUIRE(
            vili::parser::from_string("integer: -702604")["integer"].as<vili::integer>()
            == -702604);
        REQUIRE(
            vili::parser::from_string("integer: -535777")["integer"].as<vili::integer>()
            == -535777);
        REQUIRE(
            vili::parser::from_string("integer: -198134")["integer"].as<vili::integer>()
            == -198134);
        REQUIRE(
            vili::parser::from_string("integer: -50182")["integer"].as<vili::integer>()
            == -50182);
        REQUIRE(
            vili::parser::from_string("integer: -136308")["integer"].as<vili::integer>()
            == -136308);
        REQUIRE(
            vili::parser::from_string("integer: -547952")["integer"].as<vili::integer>()
            == -547952);
        REQUIRE(
            vili::parser::from_string("integer: -705657")["integer"].as<vili::integer>()
            == -705657);
        REQUIRE(
            vili::parser::from_string("integer: -501141")["integer"].as<vili::integer>()
            == -501141);
        REQUIRE(
            vili::parser::from_string("integer: -168866")["integer"].as<vili::integer>()
            == -168866);
        REQUIRE(
            vili::parser::from_string("integer: -37816")["integer"].as<vili::integer>()
            == -37816);
        REQUIRE(
            vili::parser::from_string("integer: -258036")["integer"].as<vili::integer>()
            == -258036);
        REQUIRE(
            vili::parser::from_string("integer: -339518")["integer"].as<vili::integer>()
            == -339518);
        REQUIRE(
            vili::parser::from_string("integer: -780621")["integer"].as<vili::integer>()
            == -780621);
        REQUIRE(
            vili::parser::from_string("integer: -325632")["integer"].as<vili::integer>()
            == -325632);
        REQUIRE(
            vili::parser::from_string("integer: -588373")["integer"].as<vili::integer>()
            == -588373);
        REQUIRE(
            vili::parser::from_string("integer: -410510")["integer"].as<vili::integer>()
            == -410510);
        REQUIRE(
            vili::parser::from_string("integer: -156210")["integer"].as<vili::integer>()
            == -156210);
        REQUIRE(
            vili::parser::from_string("integer: -737839")["integer"].as<vili::integer>()
            == -737839);
        REQUIRE(
            vili::parser::from_string("integer: -745034")["integer"].as<vili::integer>()
            == -745034);
        REQUIRE(
            vili::parser::from_string("integer: -208817")["integer"].as<vili::integer>()
            == -208817);
        REQUIRE(
            vili::parser::from_string("integer: -678745")["integer"].as<vili::integer>()
            == -678745);
    }
    SECTION("Maximum value")
    {
        REQUIRE(vili::parser::from_string("max_value: 9223372036854775807")["max_value"]
                    .as<vili::integer>()
            == 9223372036854775807);
    }
    SECTION("Minimum value")
    {
        REQUIRE(vili::parser::from_string("min_value: -9223372036854775807")["min_value"]
                    .as<vili::integer>()
            == -9223372036854775807);
    }
    SECTION("Zero")
    {
        REQUIRE(vili::parser::from_string("zero: 0")["zero"].as<vili::integer>() == 0);
    }
    SECTION("Single digit")
    {
        REQUIRE(vili::parser::from_string("one: 1")["one"].as<vili::integer>() == 1);
        REQUIRE(vili::parser::from_string("two: 2")["two"].as<vili::integer>() == 2);
        REQUIRE(vili::parser::from_string("three: 3")["three"].as<vili::integer>() == 3);
        REQUIRE(vili::parser::from_string("four: 4")["four"].as<vili::integer>() == 4);
        REQUIRE(vili::parser::from_string("five: 5")["five"].as<vili::integer>() == 5);
        REQUIRE(vili::parser::from_string("six: 6")["six"].as<vili::integer>() == 6);
        REQUIRE(vili::parser::from_string("seven: 7")["seven"].as<vili::integer>() == 7);
        REQUIRE(vili::parser::from_string("eight: 8")["eight"].as<vili::integer>() == 8);
        REQUIRE(vili::parser::from_string("nine: 9")["nine"].as<vili::integer>() == 9);
    }
    SECTION("Negative single digit")
    {
        REQUIRE(
            vili::parser::from_string("minus_one: -1")["minus_one"].as<vili::integer>()
            == -1);
        REQUIRE(
            vili::parser::from_string("minus_two: -2")["minus_two"].as<vili::integer>()
            == -2);
        REQUIRE(vili::parser::from_string("minus_three: -3")["minus_three"]
                    .as<vili::integer>()
            == -3);
        REQUIRE(
            vili::parser::from_string("minus_four: -4")["minus_four"].as<vili::integer>()
            == -4);
        REQUIRE(
            vili::parser::from_string("minus_five: -5")["minus_five"].as<vili::integer>()
            == -5);
        REQUIRE(
            vili::parser::from_string("minus_six: -6")["minus_six"].as<vili::integer>()
            == -6);
        REQUIRE(vili::parser::from_string("minus_seven: -7")["minus_seven"]
                    .as<vili::integer>()
            == -7);
        REQUIRE(vili::parser::from_string("minus_eight: -8")["minus_eight"]
                    .as<vili::integer>()
            == -8);
        REQUIRE(
            vili::parser::from_string("minus_nine: -9")["minus_nine"].as<vili::integer>()
            == -9);
    }
}