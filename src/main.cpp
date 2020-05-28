#include <vili/parser/parser.hpp>

#include <iostream>

#include <chrono>
#include <fstream>

void test_colors()
{
    vili::node root = vili::parser::from_string(
        "data: [{r:10, g:20, b:30}, {r: 20, g: 40, b:60}, {r:30, g:60, b:90}]");
    for (const vili::node& test : root["data"])
    {
        for (auto [color_name, color_value] : test.items())
        {
            std::cout << color_name << ", " << color_value << std::endl;
        }
        std::cout << test << std::endl;
    }
}

void test_mount()
{
    vili::node root = vili::parser::from_file("mount.vili");
    std::cout << "DUMP : " << root.dump() << std::endl;
}

int main(int argc, char** argv)
{
    try
    {
        test_mount();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}