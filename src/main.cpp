#include <vili/parser.hpp>

#include <iostream>

int main(int argc, char** argv)
{
    vili::node root = vili::parser::from_string("a: 9");
    std::cout << root["a"].as<vili::integer>();
    root["zephyr"] = 50;
    root["mille"] = 1000;
    root["coucou"] = "c'est moi :D";
    root["0"] = 0;
    std::cout << root.dump() << std::endl;

    return 0;
}