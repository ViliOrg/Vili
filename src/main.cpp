#include <vili/parser/parser.hpp>

#include <iostream>

#include <chrono>
#include <fstream>

int main(int argc, char** argv)
{
    vili::node root = vili::parser::from_string(
        "data: [{r:10, g:20, b:30}, {r: 20, g: 40, b:60}, {r:30, g:60, b:90}]");
    for (auto test : root["data"])
    {
        for (auto [color_name, color_value] : test.items())
        {
            std::cout << color_name << ", " << color_value << std::endl;
        }
        std::cout << test << std::endl;
    }

    return 0;
}