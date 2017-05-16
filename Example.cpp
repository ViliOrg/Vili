#include "Vili.hpp"

using vili::DataParser;
using vili::ComplexAttribute;
using vili::NodeIterator;
using vili::NodeValidator;

typedef std::pair<std::string, int> StringIntPair;

std::function<void(NodeValidator<StringIntPair>&)> Query(std::string content)
{
    std::vector<std::string> cBlocks = vili::Functions::String::split(content, " ");
    if (cBlocks[0] == "equal")
    {
        std::string id = cBlocks[1];
        int value = std::stoi(cBlocks[2]);

        return [id, value](NodeValidator<StringIntPair>& node){
            if (node->contains(id) && int(*node->getBaseAttribute(id)) == value) {
                node.validate(StringIntPair(node->getID(), value));
                node.terminate();
            }
        };
    }
    else if (cBlocks[0] == "max")
    {
        std::string id = cBlocks[1];
        return [id](NodeValidator<StringIntPair>& node) {
            static StringIntPair max = StringIntPair("", -1);
            if (node->contains(id) && int(*node->getBaseAttribute(id)) >= max.second) {
                max = StringIntPair(node->getID(), int(*node->getBaseAttribute(id)));
            }
            if (node.last()) {
                node.validate(max);
                max = StringIntPair("", -1);
            }
        };
    }
    else if (cBlocks[0] == "min")
    {
        std::string id = cBlocks[1];
        return [id](NodeValidator<StringIntPair>& node) {
            static StringIntPair min = StringIntPair("", -1);
            if (node->contains(id) && (min.second == -1 || int(*node->getBaseAttribute(id)) <= min.second)) {
                min = StringIntPair(node->getID(), int(*node->getBaseAttribute(id)));
            }
            if (node.last()) {
                node.validate(min);
                min = StringIntPair("", -1);
            }
        };
    }
}

int main(int argc, char** argv)
{
    DataParser file;
    file.parseFile("test.vili");

    StringIntPair sizeEqual4 = file->walk(Query("equal size 4"));
    std::cout << "The first one with size = 4 is : " << sizeEqual4.first << std::endl;

    StringIntPair minSpeed = file->walk(Query("min speed"), true);
    std::cout << "The slowest animal is " << minSpeed.first << " with speed " << minSpeed.second << std::endl;

    StringIntPair maxSpeed = file->walk(Query("max speed"), true);
    std::cout << "The fastest animal is " << maxSpeed.first << " with speed " << maxSpeed.second << std::endl;

    StringIntPair minSize = file->walk(Query("min size"), true);
    std::cout << "The smallest animal is " << minSize.first << std::endl;

    StringIntPair maxSize = file->walk(Query("max size"), true);
    std::cout << "The biggest animal is " << maxSize.first << std::endl;

    return 0;
}
