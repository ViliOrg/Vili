#include "vili/Types.hpp"
#include "Functions.hpp"

namespace vili
{
    namespace Types
    {
        std::string getDefaultValueForType(Types::DataType type)
        {
            std::string value = "";
            if (type == Types::String) value.clear();
            else if (type == Types::Int) value = "0";
            else if (type == Types::Float) value = "0.0";
            else if (type == Types::Bool) value = "False";
            else if (type == Types::Link) value = "&()";
            else if (type == Types::Template) value = "T()";
            return value;
        }

        Types::DataType getVarType(const std::string& var)
        {
            Types::DataType attributeType;
            std::vector<std::string> boolType = { "True", "False" };
            if (Functions::Vector::isInList(var, boolType))
                attributeType = Types::Bool;
            else if (Functions::String::occurencesInString(var, "..") == 1
                && Functions::String::isStringInt(Functions::String::split(var, "..")[0])
                && Functions::String::isStringInt(Functions::String::split(var, "..")[1]))
            {
                attributeType = Types::Range;
            }
            else if (var.substr(0, 2) == "&(" && var.substr(var.size() - 1, 1) == ")")
                attributeType = Types::Link;
            else if (isalpha(var[0]) && Functions::String::occurencesInString(var, "(") == 1 && var.substr(var.size() - 1, 1) == ")")
                attributeType = Types::Template;
            else if (var.substr(0, 1) == "\"" && var.substr(var.size() - 1, 1) == "\"")
                attributeType = Types::String;
            else if (Functions::String::isStringFloat(var))
                attributeType = Types::Float;
            else if (Functions::String::isStringInt(var))
                attributeType = Types::Int;
            else
                attributeType = Types::Unknown;
            return attributeType;
        }

        Types::DataType stringToDataType(const std::string& type)
        {
            if (type == "string" || type == "String")
                return Types::String;
            else if (type == "int" || type == "Int")
                return Types::Int;
            else if (type == "float" || type == "Float")
                return Types::Float;
            else if (type == "bool" || type == "Bool")
                return Types::Bool;
            else if (type == "range" || type == "Range")
                return Types::Range;
            else if (type == "template" || type == "Template")
                return Types::Template;
            else
                return Types::Unknown;
        }

        std::string dataTypeToString(Types::DataType type)
        {
            if (type == Types::String) return "String";
            else if (type == Types::Int) return "Int";
            else if (type == Types::Float) return "Float";
            else if (type == Types::Bool) return "Bool";
            else if (type == Types::Range) return "Range";
            else if (type == Types::Template) return "Template";
            else if (type == Types::Unknown) return "Unknown";
            return "Error-Unknown";
        }

        std::string attributeTypeToString(Types::AttributeType type)
        {
            if (type == Types::Attribute) return "Attribute";
            else if (type == Types::ContainerAttribute) return "ContainerAttribute";
            else if (type == Types::BaseAttribute) return "BaseAttribute";
            else if (type == Types::ListAttribute) return "ListAttribute";
            else if (type == Types::ComplexAttribute) return "ComplexAttribute";
            else if (type == Types::LinkAttribute) return "LinkAttribute";
            return "Error-Unknown";
        }
    }
}
