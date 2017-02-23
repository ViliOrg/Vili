#include "ViliData.hpp"

using namespace vili;

int main(int argc, char** argv)
{
	std::ofstream out("log.txt");
	std::streambuf *coutbuf = std::cout.rdbuf();
	std::cout.rdbuf(out.rdbuf());

	DataParser parser;
	parser.parseFile("Test.vili", true);
	parser.writeFile("Return.vili", true);

	return 0;
}