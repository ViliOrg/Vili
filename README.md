# Vili
A nice and readable data format !

![build](https://github.com/Sygmei/Vili/workflows/build/badge.svg)

# Examples :

## Animals (Simple tree-like structure)

### Vili file

`Animalia.vili`

```
Spacing (2);

Animalia:
  Chordate:
    Mammal:
      Carnivora:
        Felidae:
          Felis:
            Domestica:
              HouseCat:
                name:"House Cat"
                size:24.5
                cool:True
            Leo:
              Lion:
                name:"Lion"
                size:280.7
                cool:True
      Primate:
        Pongidae:
          Pan:
            Troglodytes:
              Chimpanzee:
                name:"Chimpanzee"
                size:81.6
                cool:True
        Hominidae:
          Homo:
            Sapiens:
              Human:
                name:"Human"
                size:170
                cool:False
  Arthopoda:
    Insect:
      Diptera:
        Muscidae:
          Musca:
            Domestica:
              Housefly:
                name:"Housefly"
                size:0.12
                cool:False
```

### Generation

`Animalia.cpp`

```cpp
#include <iostream>

#include "Vili.hpp"

using vili::DataParser;
using vili::ComplexAttribute;

class Animal
{
    private:
        std::string m_path;
        std::string m_name;
        double m_size;
        bool m_cool;
    public:
    Animal(const std::string& path, const std::string& name, double size, bool cool)
    {
        m_path = path;
        m_name = name;
        m_size = size;
        m_cool = cool;
    }
    void write(ComplexAttribute& writeInto)
    {
        ComplexAttribute* currentPart = &writeInto;
        for (const std::string& pathPart : vili::Functions::String::split(m_path, "/"))
        {
            if (currentPart->contains(vili::Types::ComplexAttribute, pathPart))
                currentPart = &currentPart->at(pathPart);
            else
            {
                currentPart->createComplexAttribute(pathPart);
                currentPart = &currentPart->at(pathPart);
            }
        }
        std::string lastNodeName = vili::Functions::String::replaceString(m_name, " ", "");
        currentPart->createComplexAttribute(lastNodeName);
        currentPart = &currentPart->at(lastNodeName);
        currentPart->createBaseAttribute("name", m_name);
        currentPart->createBaseAttribute("size", m_size);
        currentPart->createBaseAttribute("cool", m_cool);
    }
};

int main()
{
    DataParser file;
    file.setSpacing(2);
    ComplexAttribute& root = file.root();

    std::vector<Animal> animals;

    animals.push_back(Animal("Animalia/Chordate/Mammal/Carnivora/Felidae/Felis/Domestica", "House Cat", 24.5, true));
    animals.push_back(Animal("Animalia/Chordate/Mammal/Carnivora/Felidae/Felis/Leo", "Lion", 280.7, true));
    animals.push_back(Animal("Animalia/Chordate/Mammal/Primate/Pongidae/Pan/Troglodytes", "Chimpanzee", 81.6, true));
    animals.push_back(Animal("Animalia/Chordate/Mammal/Primate/Hominidae/Homo/Sapiens", "Human", 170, false));
    animals.push_back(Animal("Animalia/Arthopoda/Insect/Diptera/Muscidae/Musca/Domestica", "Housefly", 0.12, false));

    for (Animal& animal : animals)
        animal.write(root);
    
    file.writeFile("Animalia.vili");
    return 0;
}
```

## Countries and Cities (Lists)

### Vili file

`Countries.vili`

```
France:
  cities:["Paris", "Marseille", "Lyon", "Nice"]
Germany:
  cities:["Berlin", "Hamburg", "Munich", "Cologne"]
UnitedKingdom:
  cities:["London", "Manchester", "Liverpool", "Glasgow"]
```

### Generation

`Countries.cpp`

```cpp
#include <iostream>

#include "Vili.hpp"

using vili::DataParser;
using vili::ComplexAttribute;
using vili::ListAttribute;

class Country
{
    private:
        std::string m_name;
        std::vector<std::string> m_cities;
    public:
        Country(const std::string& name, const std::vector<std::string>& cities)
        {
            m_name = name;
            m_cities = cities;
        }
        void write(ComplexAttribute& writeInto)
        {
            writeInto.createComplexAttribute(m_name);
            writeInto.at(m_name).createListAttribute("cities");
            for (const std::string& city : m_cities)
                writeInto.at<ListAttribute>(m_name, "cities").push(city);
        }
};

int main()
{
    DataParser file;
    file.setSpacing(2);
    ComplexAttribute& root = file.root();
    std::vector<Country> countries;

    countries.push_back(Country("France", {"Paris", "Marseille", "Lyon", "Nice"}));
    countries.push_back(Country("Germany", {"Berlin", "Hamburg", "Munich", "Cologne"}));
    countries.push_back(Country("UnitedKingdom", {"London", "Manchester", "Liverpool", "Glasgow"}));

    for (Country& country : countries)
        country.write(root);
    
    file.writeFile("Countries.vili");
    return 0;
}
```

## Little company (Inheritance)

### Vili file

`Employees.vili`

```
Employees:
  TeamBusiness:
    team:"Business"
    rank:1
  TeamDeveloppers:
    team:"Developpers"
    rank:2
  TeamSupport:
    team:"Support"
    rank:3
  Bob(TeamBusiness):
    name:"Bob Dupont"
  Mark(TeamDeveloppers):
    name:"Mark Mark"
  Gregory(TeamSupport):
    name:"Gregory No"
```

### Generation

`Employees.cpp`

```cpp
#include <iostream>

#include "Vili.hpp"

using vili::DataParser;
using vili::ComplexAttribute;

int main()
{
    DataParser file;
    file.setSpacing(2);
    ComplexAttribute& root = file.root();
    root.createComplexAttribute("Employees");
    ComplexAttribute& employees = root.at("Employees");

    auto createTeam = [&employees](const std::string& teamName) {
        employees.createComplexAttribute("Team" + teamName);
        employees.at("Team" + teamName).createBaseAttribute("team", teamName);
        employees.at("Team" + teamName).createBaseAttribute("rank", int(employees.getAll().size()));
    };

    auto createEmployee = [&employees](const std::string& employeeName) {
        std::string firstName = vili::Functions::String::split(employeeName, " ")[0];
        employees.createComplexAttribute(firstName);
        employees.at(firstName).heritage(
            &employees.at(employees.getAll()[(employees.getAll().size() - 1) % 3])
        );
        employees.at(firstName).createBaseAttribute("name", employeeName);
    };

    createTeam("Business");
    createTeam("Developpers");
    createTeam("Support");

    createEmployee("Bob Dupont");
    createEmployee("Mark Mark");
    createEmployee("Gregory No");
    
    file.writeFile("Employees.vili");
    return 0;
}
```

## Colors (Templates)

### Vili Template File

`ColorTemplate.vili`

```
Color:    
    IntComposant:
      type:"Int"
    FaculativeIntComposant(IntComposant):
      defaultValue:255

    __init__:
        0(IntComposant):
        1(IntComposant):
        2(IntComposant):
        3(FaculativeIntComposant):

    __body__:
        type:"Color"
        r:&(0/value)
        g:&(1/value)
        b:&(2/value)
        a:&(3/value)

Template(Color);
```

### Vili File

`Color.vili`

```
Include (ColorTemplate);

black:Color(0, 0, 0)
white:Color(255, 255, 255)
red:Color(255, 0, 0)
green:Color(0, 255, 0)
blue:Color(0, 0, 255)
yellow:Color(255, 255, 0)
cyan:Color(0, 255, 255)
magenta:Color(255, 0, 255)
transparent:Color(0, 0, 0, 0)
transparent_red:Color(255, 0, 0, 128)
```

### Generation

`Colors.cpp`

```cpp
#include <iostream>

#include "Vili.hpp"

using vili::DataParser;
using vili::ComplexAttribute;

int main()
{
    DataParser templateFile;
    templateFile->createComplexAttribute("Color");
    ComplexAttribute& color = templateFile->at("Color");
    color.createComplexAttribute("IntComposant");
    color.at("IntComposant").createBaseAttribute("type", "Int");
    color.createComplexAttribute("FacultativeIntComposant");
    color.at("FacultativeIntComposant").createBaseAttribute("defaultValue", 255);
    color.at("FacultativeIntComposant").heritage(&color.at("IntComposant"));
    color.createComplexAttribute("__init__");
    ComplexAttribute& init = color.at("__init__");

    auto createTemplateArg = [&color, &init](const std::string& argType) {
        init.createComplexAttribute(std::to_string(init.getAll().size()));
        init.at(std::to_string(init.getAll().size() - 1)).heritage(&color.at(argType));
    };

    for (int i = 0; i < 4; i++)
        createTemplateArg((i != 3) ? "IntComposant" : "FacultativeIntComposant");

    color.createComplexAttribute("__body__");
    ComplexAttribute& body = color.at("__body__");
    body.createBaseAttribute("type", "Color");

    auto createTemplateComponent = [&body](const std::string& component) {
        body.createLinkAttribute(component, std::to_string(body.getAll().size() - 1) + "/value");
    };

    createTemplateComponent("r");
    createTemplateComponent("g");
    createTemplateComponent("b");
    createTemplateComponent("a");

    templateFile.generateTemplate("Color");
    
    templateFile.writeFile("ColorTemplate.vili");

    DataParser file;
    
    file.includeFile("ColorTemplate");
    file.setSpacing(2);
    ComplexAttribute& root = file.root();


    auto createColor = [&file, &root](const std::string& colorName, int r, int g, int b, int a = 255) {
        root.createComplexAttribute(colorName);
        root.at(colorName).createBaseAttribute("r", r);
        root.at(colorName).createBaseAttribute("g", g);
        root.at(colorName).createBaseAttribute("b", b);
        root.at(colorName).createBaseAttribute("a", a);
        root.at(colorName).useTemplate(file.getTemplate("Color"));
    };

    createColor("black", 0, 0, 0);
    createColor("white", 255, 255, 255);
    createColor("red", 255, 0, 0);
    createColor("green", 0, 255, 0);
    createColor("blue", 0, 0, 255);
    createColor("yellow", 255, 255, 0);
    createColor("cyan", 0, 255, 255);
    createColor("magenta", 255, 0, 255);
    createColor("transparent", 0, 0, 0, 0);
    createColor("transparent_red", 255, 0, 0, 100);
    
    file.writeFile("Color.vili");
    return 0;
}
```

## File list (Ranges)

```
Images:
  files:[0..499]
  model:"%s.png"
Documents:
  files:[99..63]
  model:"%s.doc"
```
