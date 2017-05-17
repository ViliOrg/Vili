#include <iostream>
#include <vili>

int main(int argc, char** argv)
{
    DataParser file("values.vili"); //Opening and parsing the file 'values.vili'

    //Get BaseAttribute "name"
    std::cout << "Hello my name is " << *file.at<BaseAttribute>("name") << ", "; 
    //Get BaseAttribute "age"
    std::cout << "I'm " << *file.at<BaseAttribute>("age") << " and I'm";
    //Display "not" if BaseAttribute "beautiful" is set to "False"
    std::cout << (file.at<BaseAttribute>("beautiful")->get<bool>() ? " " : " not ") << "beautiful. "; <<
    //Displat BaseAttribute "height"
    std::cout << "Also, I'm about " << *file.at<BaseAttribute>("height") << "m tall";
    std::cout << std::endl;

    std::cout << "TRANSFORMATION ! Start the program again to see the other person" << std::endl;
    
    //If the BaseAttribute "name" is equal to "Albert"
    if (file.at<BaseAttribute>("name")->get<std::string>() == "Albert")
    {
        //Set BaseAttribute "name" value to "Frank"
        file.at<BaseAttribute>("name")->set("Frank");
        //Set BaseAttribute "age" value to 13
        file.at<BaseAttribute>("age")->set(13);
        //Set BaseAttribute "beautiful" value to False
        file.at<BaseAttribute>("beautiful")->set(false);
        //Set BaseAttribute "height" value to 1.25
        file.at<BaseAttribute>("height")->set(1.25);
    }
    else
    {
        //Set BaseAttribute "name" value to "Albert"
        file.at<BaseAttribute>("name")->set("Albert");
        //Set BaseAttribute "age" value to 42
        file.at<BaseAttribute>("age")->set(42);
        //Set BaseAttribute "beautiful" value to True
        file.at<BaseAttribute>("beautiful")->set(true);
        //Set BaseAttribute "height" value to 1.81
        file.at<BaseAttribute>("height")->set(1.81);
    }

    file.writeFile("values.vili"); //Write changes in the file 'values.vili' (Overwriting the file)
}
