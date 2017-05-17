#include <iostream>
#include <vili>

using vili::DataParser;

/*
This example shows :
  - How to parse a file
  - How to get a simple BaseAttribute with 3 different ways
*/
int main(int argc, char** argv)
{
    DataParser file("hello.vili"); //Creates a DataParser object and parse the file "hello.vili"
    std::string message; //String where we will store message content
    
    //at (From DataParser) + Template-way
    message = std::string(*file.at<BaseAttribute>("message"));
    //at (From root ComplexAttribute) + Template-way
    message = file->at<BaseAttribute>("message")->get<std::string>();
    //getBaseAttribute + Cast-way
    message = std::string(*file->getBaseAttribute("message"));

    std::cout << "The message is : " << message << std::endl; //Display the message
}
