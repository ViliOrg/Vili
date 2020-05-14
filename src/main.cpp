#include <vili/parser/parser.hpp>

#include <iostream>

#include <chrono>
#include <fstream>

int main(int argc, char** argv)
{
    double tt = 0;
    std::ifstream t("float.vili");
    std::string str(
        (std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    for (int i = 0; i < 10; i++)
    {
        auto start = std::chrono::steady_clock::now();
        try
        {
            vili::node root = vili::parser::from_string(str);
            std::cout << root["data"].size() << std::endl;
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
        auto end = std::chrono::steady_clock::now();
        auto duration = end - start;
        auto current
            = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        std::cout << "Current time : " << current << std::endl;
        tt += current;
    }
    std::cout << "Total time : " << tt / 10.f << std::endl;

    // vili::node root = vili::parser::from_string("a: 9");
    // std::cout << root["data"].size() << std::endl;
    // vili_full();

    return 0;
}