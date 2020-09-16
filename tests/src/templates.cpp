#include <catch/catch.hpp>
#include <fmt/format.h>

#include <vili/parser/parser.hpp>
#include <vili/parser/parser_state.hpp>

TEST_CASE("Templates in root")
{
    SECTION("Simple template")
    {
        vili::node root = vili::parser::from_string("template pi: 3.14\ncopi: pi");

        REQUIRE(root.at("copi").as<vili::number>() == 3.14);

        REQUIRE(!root.contains("pi"));
    }
    SECTION("Object template")
    {
        vili::node root = vili::parser::from_string(
            "template human: {kind: \"human\", name: \"\", age: 0}\n"
            "bob: human {name: \"Bob\", age: 18}\n"
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

TEST_CASE("Templates in list")
{
    SECTION("Templates from parser_state")
    {
        vili::parser::state parser_state;
        parser_state.push_template(
            "add", vili::object { { "command", "add" }, { "amount", 1 } });
        parser_state.push_template(
            "multiply", vili::object { { "command", "multiply" }, { "amount", 1 } });
        parser_state.push_template(
            "format", vili::object { { "command", "format" }, { "output", "{}" } });
        vili::node root = vili::parser::from_string("code: ["
                                                    "add,"
                                                    "multiply {amount: 3},"
                                                    "add {amount: 2},"
                                                    "multiply,"
                                                    "multiply {amount: 2},"
                                                    "add,"
                                                    "multiply {amount: 2},"
                                                    "format {output: \"Number: {}\"}"
                                                    "]",
            parser_state);
        int sum = 0;
        std::string output;
        for (vili::node& command : root.at("code"))
        {
            if (command.at("command").as<vili::string>() == "add")
            {
                sum += command.at("amount").as<vili::integer>();
            }
            else if (command.at("command").as<vili::string>() == "multiply")
            {
                sum *= command.at("amount").as<vili::integer>();
            }
            else if (command.at("command").as<vili::string>() == "format")
            {
                output = fmt::format(command.at("output").as<vili::string>(), sum);
            }
        }
        REQUIRE(output == "Number: 22");
    }
}