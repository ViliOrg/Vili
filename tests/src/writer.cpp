#include <fstream>

#include <catch/catch.hpp>
#include <fmt/format.h>

#include <vili/node.hpp>
#include <vili/parser.hpp>
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
        options.array.items_per_line.any = 1;
        REQUIRE(vili::writer::dump(names, options)
            == "[\n    \"Bob\",\n    \"Carl\",\n    \"Jack\"\n]");
        options.indent = 1;
        REQUIRE(vili::writer::dump(names, options)
            == "[\n \"Bob\",\n \"Carl\",\n \"Jack\"\n]");
    }
    SECTION("Inline spacing")
    {
        vili::writer::dump_options options;
        options.array.inline_spacing = 2;
        REQUIRE(
            vili::writer::dump(names, options) == "[\"Bob\",  \"Carl\",  \"Jack\"]");
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
        options.array.items_per_line.any = 1;
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
    vili::object simple_list
        = vili::object { { "simple_list", vili::array { 1, 2, 3 } } };
    vili::object family
        = vili::object { { "Bob", 20 }, { "Jack", 30 }, { "Hughes", 55 } };
    vili::object other_family
        = vili::object { { "Tom", 10 }, { "Bob", 40 }, { "Leo", 65 } };
    vili::object family_collection = vili::object
    {
        { "families", vili::array { family, other_family } }
    };
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
    vili::object more_complex_object = vili::object {
        { "name", "Runner" },
        { "framerate", 30 },
        { "mode", "OneTime" },
        { "images", vili::array { "runner_1.png", "runner_2.png" } },
        { "groups", vili::object { { "main", vili::array { 0, 1 } } } },
        { "code", vili::array { vili::object { { "command", "PlayGroup" }, {"group", "main" } } } }
    };
    // clang-format on
    SECTION("Simple object dump")
    {
        REQUIRE(vili::writer::dump(family) == "Bob: 20\nJack: 30\nHughes: 55");
    }
    SECTION("Simple object with list dump")
    {
        REQUIRE(vili::writer::dump(simple_list) == "simple_list: [1, 2, 3]");
        vili::writer::dump_options options;
        options.array.items_per_line.any = 1;
        constexpr std::string_view expected_result = "simple_list: [\n"
                                                 "    1,\n"
                                                 "    2,\n"
                                                 "    3\n"
                                                 "]";
        REQUIRE(vili::writer::dump(simple_list, options) == expected_result);
    }
    SECTION("Multilevel object dump")
    {
        vili::writer::dump_options options;
        std::string_view expected_result = "Bob:\n"
                                           "    age: 20\n"
                                           "    weight: 68\n"
                                           "    height: 170\n"
                                           "\n"
                                           "Jack:\n"
                                           "    age: 30\n"
                                           "    weight: 55\n"
                                           "    height: 160\n"
                                           "\n"
                                           "Hughes:\n"
                                           "    age: 55\n"
                                           "    weight: 95\n"
                                           "    height: 190";
        std::string dump_result = vili::writer::dump(family_advanced, options);
        REQUIRE(dump_result == expected_result);
    }
    SECTION("Objects in array dump")
    {
        vili::writer::dump_options options;
        std::string_view expected_result = "families: [\n"
                                           "    {\n"
                                           "        Bob: 20,\n"
                                           "        Jack: 30,\n"
                                           "        Hughes: 55\n"
                                           "    },\n"
                                           "    {\n"
                                           "        Tom: 10,\n"
                                           "        Bob: 40,\n"
                                           "        Leo: 65\n"
                                           "    }\n"
                                           "]";
        std::string dump_result = vili::writer::dump(family_collection, options);
        REQUIRE(dump_result == expected_result);
    }
    SECTION("More complex object")
    {
        vili::writer::dump_options options;
        std::string_view expected_result
            = "name: \"Runner\"\n"
              "framerate: 30\n"
              "mode: \"OneTime\"\n"
              "images: [\"runner_1.png\", \"runner_2.png\"]\n"
              "groups:\n"
              "    main: [0, 1]\n"
              "\n"
              "code: [\n"
              "    {\n"
              "        command: \"PlayGroup\",\n"
              "        group: \"main\"\n"
              "    }\n"
              "]";
        std::string dump_result = vili::writer::dump(more_complex_object, options);
        REQUIRE(dump_result == expected_result);
    }
}