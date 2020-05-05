#include <vili/parser.hpp>

int main(int argc, char** argv)
{
    vili::node root = vili::parser::from_string("a: 9");
    std::cout << root["a"].as<vili::integer>();
    return 0;
}