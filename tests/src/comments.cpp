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
        SECTION("Comment with one-lined affectation")
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

        SECTION("Comment in array")
        {
            REQUIRE(
                vili::parser::from_string(
                    "test: [42,# comment with array\n"
                    "43, # comment with array\n"
                    "44]# comment with array")
            .at("test").as<vili::array>() == vili::array{42, 43, 44});
        }

        SECTION("Comment in brace based object")
        {
            vili::node root = vili::parser::from_string(
                "test: {int1: 42, # comment with object\n"
                "tab1: [43, 44], # comment with object\n"
                "int2: 45}# comment with object");
            REQUIRE(root["test"]["int1"].as<vili::integer>() == 42);
            REQUIRE(root["test"]["tab1"].as<vili::array>() == vili::array{43, 44});
            REQUIRE(root["test"]["int2"].as<vili::integer>() == 45);
        }

        SECTION("Comment in indent based object")
        {
            vili::node root = vili::parser::from_string(
                "test: # comment with object\n"
                "   int1: 42 # comment with object\n"
                "   tab1: [43, 44] # comment with object\n"
                "# comment with object\n"
                "   int2: 45 # comment with object\n");
            REQUIRE(root["test"]["int1"].as<vili::integer>() == 42);
            REQUIRE(root["test"]["tab1"].as<vili::array>() == vili::array{43, 44});
            REQUIRE(root["test"]["int2"].as<vili::integer>() == 45);
        }

        SECTION("Comment in template")
        {
            vili::node root = vili::parser::from_string(
                "template human: {kind: \"human\", name: \"\", age: 0} # comment in template\n"
                "bob: human {name: \"Bob\", age: 18}   #  comment in template\n"
                "mark: human {name: \"Mark\"} #   comment in template");

            REQUIRE(root.at("bob").at("kind").as<vili::string>() == "human");
            REQUIRE(root.at("bob").at("name").as<vili::string>() == "Bob");
            REQUIRE(root.at("bob").at("age").as<vili::integer>() == 18);
            REQUIRE(root.at("mark").at("kind").as<vili::string>() == "human");
            REQUIRE(root.at("mark").at("name").as<vili::string>() == "Mark");
            REQUIRE(root.at("mark").at("age").as<vili::integer>() == 0);
            REQUIRE(!root.contains("human"));
        }
    }
}

TEST_CASE("Multiline comment")
{
    SECTION("Comment only")
    {
        REQUIRE_NOTHROW(vili::parser::from_string("  /* This is a multiline comment on one line */  "));

        REQUIRE_NOTHROW(vili::parser::from_string("  /* This is a\nmultiline\ncomment */  "));

        REQUIRE_NOTHROW(
            vili::parser::from_string("  /*This is\n"
            "a /* nested */ multiline\n"
            "comment*/  "));
    }

    SECTION("Incorrect comment")
    { 
        REQUIRE_THROWS_AS(vili::parser::from_string("  /* This is an\nunclosed\ncomment") 
            ,vili::exceptions::parsing_error);

        REQUIRE_THROWS_AS(vili::parser::from_string("   This is an\nunclosed\ncomment */"),
            vili::exceptions::parsing_error);

        REQUIRE_THROWS_AS(vili::parser::from_string(
            "test:\n"
            "   int1: 42\n"
            "  /* comment */ int2: 43\n"),
            vili::exceptions::parsing_error);
    }

    SECTION("Comment with code")
    {
        SECTION("Comment with one-lined affectation")
        {
            REQUIRE(
                vili::parser::from_string("test: 42 /* comment on one line */")
            .at("test").as<vili::integer>() == 42);

            REQUIRE(
                vili::parser::from_string("/* comment on one line */test: 42")
            .at("test").as<vili::integer>() == 42);

            vili::node root = vili::parser::from_string(
                "test1: 42 /* multiline\n"
                "comments */\n"
                "test2: 43");
            REQUIRE(root["test1"].as<vili::integer>() == 42);
            REQUIRE(root["test2"].as<vili::integer>() == 43);
        }

        SECTION("Comment in array")
        {
            REQUIRE(
                vili::parser::from_string(
                    "test: [42,/* multiline comment\n"
                    "with array */43, /* one line comment */\n"
                    "44] /* one line comment */")
            .at("test").as<vili::array>() == vili::array{42, 43, 44});
        }

        SECTION("Comment in brace based object")
        {
            vili::node root = vili::parser::from_string(
                "test: {int1: 42, /* multiline comment\n"
                "with object */tab1: [43, 44], /* one line comment */\n"
                "int2: 45}/* one line comment */");
            REQUIRE(root["test"]["int1"].as<vili::integer>() == 42);
            REQUIRE(root["test"]["tab1"].as<vili::array>() == vili::array{43, 44});
            REQUIRE(root["test"]["int2"].as<vili::integer>() == 45);
        }

        SECTION("Comment in indent based object")
        {
            vili::node root = vili::parser::from_string(
                "test: /* multiline comment \n"
                "with object */\n"
                "   int1: 42   /* one line comment */\n"
                "   tab1: [43, 44] /* multiline comment\n"
                "   with object */\n"
                "   /* multiline comment */int2: 45\n");
            REQUIRE(root["test"]["int1"].as<vili::integer>() == 42);
            REQUIRE(root["test"]["tab1"].as<vili::array>() == vili::array{43, 44});
            REQUIRE(root["test"]["int2"].as<vili::integer>() == 45);
        }

        SECTION("Comment in template")
        {
            vili::node root = vili::parser::from_string(
                "/* one line comment */template human: {kind: \"human\", name: \"\", age: 0}/* multiline\n"
                "comment */\n"
                "/* one line comment */bob: human {name: \"Bob\", age: 18}\n"
                "mark: human {name: \"Mark\"}");

            REQUIRE(root.at("bob").at("kind").as<vili::string>() == "human");
            REQUIRE(root.at("bob").at("name").as<vili::string>() == "Bob");
            REQUIRE(root.at("bob").at("age").as<vili::integer>() == 18);
            REQUIRE(root.at("mark").at("kind").as<vili::string>() == "human");
            REQUIRE(root.at("mark").at("name").as<vili::string>() == "Mark");
            REQUIRE(root.at("mark").at("age").as<vili::integer>() == 0);
            REQUIRE(!root.contains("human"));
        }
    }
}