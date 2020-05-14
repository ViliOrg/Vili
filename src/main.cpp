#include <vili/parser.hpp>

#include <iostream>

int main(int argc, char** argv)
{
    vili::node root = vili::parser::from_file("big_test.vili");
    root["sprites"]["background"].erase("path");
    std::cout << root << std::endl;

    return 0;
}