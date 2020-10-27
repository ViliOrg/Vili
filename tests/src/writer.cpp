#include <catch/catch.hpp>
#include <fmt/format.h>

#include <vili/node.hpp>
#include <vili/writer.hpp>

TEST_CASE("Primitives dumps")
{
    SECTION("Integer dump")
    {
        REQUIRE(vili::writer::dump(vili::integer { 0 }) == "0");
        REQUIRE(vili::writer::dump(vili::integer { 3 }) == "3");
        REQUIRE(vili::writer::dump(vili::integer { -1 }) == "-1");
        REQUIRE(vili::writer::dump(vili::integer { 22 }) == "22");
        REQUIRE(vili::writer::dump(vili::integer { 9223372036854775807 })
            == "9223372036854775807");
        REQUIRE(vili::writer::dump(vili::integer { -9223372036854775807 })
            == "-9223372036854775807");
    }
    SECTION("Boolean dump")
    {
        REQUIRE(vili::writer::dump(vili::boolean { true }) == "true");
        REQUIRE(vili::writer::dump(vili::boolean { false }) == "false");
    }
    SECTION("String dump")
    {
        REQUIRE(
            vili::writer::dump(vili::string { "Hello, world!" }) == "\"Hello, world!\"");
        REQUIRE(vili::writer::dump(vili::string {
                    "What's your favorite food ? \\\"Eggplants\\\" I replied" })
            == "\"What's your favorite food ? \\\"Eggplants\\\" I replied\"");
    }
    SECTION("Number dump")
    {
        REQUIRE(vili::writer::dump(vili::number { 3.14 }) == "3.14");
        REQUIRE(vili::writer::dump(vili::number { 1 }) == "1.0");
        REQUIRE(vili::writer::dump(vili::number { 22.23 }) == "22.23");
    }
}

TEST_CASE("Array dumps")
{
    vili::array numbers = vili::array { 1, 2, 3, 4, 5 };
    vili::array names = vili::array { "Bob", "Carl", "Jack" };
    vili::array long_array
        = vili::array { "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
                        "eiusmod tempor incididunt ut labore et dolore magna aliqua",
              "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi "
              "ut aliquip ex ea commodo consequat",
              "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum "
              "dolore eu fugiat nulla pariatur",
              "a", "b", "c", "Not so long string", "This one is okay too",
              "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia "
              "deserunt mollit anim id est laborum" };
    SECTION("Simple dump")
    {
        vili::writer::dump_options options;
        REQUIRE(vili::writer::dump(numbers, options) == "[1, 2, 3, 4, 5]");
        REQUIRE(vili::writer::dump(names, options) == "[\"Bob\", \"Carl\", \"Jack\"]");
    }
    SECTION("Single element per line")
    {
        vili::writer::dump_options options;
        options.array.items_per_line = 1;
        REQUIRE(vili::writer::dump(names, options)
            == "[\n    \"Bob\",\n    \"Carl\",\n    \"Jack\"\n]");
        options.indent = 1;
        REQUIRE(vili::writer::dump(names, options)
            == "[\n \"Bob\",\n \"Carl\",\n \"Jack\"\n]");
    }
    SECTION("Brackets spacing")
    {
        vili::writer::dump_options options;
        options.array.left_bracket_spacing = 1;
        options.array.right_bracket_spacing = 0;
        REQUIRE(vili::writer::dump(names, options) == "[ \"Bob\", \"Carl\", \"Jack\"]");
        options.array.left_bracket_spacing = 0;
        options.array.right_bracket_spacing = 1;
        REQUIRE(vili::writer::dump(names, options) == "[\"Bob\", \"Carl\", \"Jack\" ]");
        options.array.left_bracket_spacing = 2;
        options.array.right_bracket_spacing = 2;
        REQUIRE(
            vili::writer::dump(names, options) == "[  \"Bob\", \"Carl\", \"Jack\"  ]");
    }
    SECTION("Comma spacing")
    {
        vili::writer::dump_options options;
        options.array.comma_spacing = vili::writer::comma_spacing_policy::both;
        options.array.inline_spacing = 2;
        REQUIRE(
            vili::writer::dump(names, options) == "[\"Bob\"  ,  \"Carl\"  ,  \"Jack\"]");
    }
    SECTION("Force newline on brackets")
    {
        vili::writer::dump_options options;
        options.array.starts_with_newline
            = vili::writer::delimiter_newline_policy::always;
        options.array.ends_with_newline = vili::writer::delimiter_newline_policy::never;
        REQUIRE(
            vili::writer::dump(names, options) == "[\n    \"Bob\", \"Carl\", \"Jack\"]");
        options.array.starts_with_newline = vili::writer::delimiter_newline_policy::never;
        options.array.ends_with_newline = vili::writer::delimiter_newline_policy::always;
        REQUIRE(vili::writer::dump(names, options) == "[\"Bob\", \"Carl\", \"Jack\"\n]");
        options.array.starts_with_newline
            = vili::writer::delimiter_newline_policy::always;
        options.array.ends_with_newline = vili::writer::delimiter_newline_policy::always;
        REQUIRE(vili::writer::dump(names, options)
            == "[\n    \"Bob\", \"Carl\", \"Jack\"\n]");
        options.array.items_per_line = 1;
        options.array.starts_with_newline = vili::writer::delimiter_newline_policy::never;
        options.array.ends_with_newline = vili::writer::delimiter_newline_policy::never;
        REQUIRE(vili::writer::dump(names, options)
            == "[\"Bob\",\n    \"Carl\",\n    \"Jack\"]");
    }
    SECTION("Max line length")
    {
        vili::writer::dump_options options;
        options.array.max_line_length = 40;
        vili::writer::dump(long_array, options); // TODO: Test this
    }
}

TEST_CASE("Object dumps")
{
    vili::object family
        = vili::object { { "Bob", 20 }, { "Jack", 30 }, { "Hughes", 55 } };
    // clang-format off
    vili::object family_advanced = vili::object {
        {"Bob", vili::object {
            {"age", 20},
            {"weight", 68},
            {"height", 170}
        }},
        {"Jack", vili::object {
            {"age", 30},
            {"weight", 55},
            {"height", 160}
        }},
        {"Hughes", vili::object {
            {"age", 55},
            {"weight", 95},
            {"height", 190}
        }},
    };
    // clang-format on
    SECTION("Simple object dump")
    {
        REQUIRE(vili::writer::dump(family) == "Bob: 20\nJack: 30\nHughes: 55");
    }
    SECTION("Multilevel object dump")
    {
        vili::writer::dump_options options;
        // REQUIRE(vili::writer::dump(family_advanced, options) == "lol");
        options.object.style = vili::writer::object_style::braces;
        options.object.items_per_line = 1;
        REQUIRE(vili::writer::dump(family_advanced, options) == "lol");
    }
}