#pragma once

#include <string>

namespace vili
{
	namespace Types
	{
		enum AttributeType
		{
			Attribute,
			ContainerAttribute,
			BaseAttribute,
			ListAttribute,
			ComplexAttribute,
			LinkAttribute
		};

		enum DataType
		{
			String,
			Int,
			Float,
			Bool,
			Range,
			Link,
			Template,
			Unknown
		};

		Types::DataType stringToDataType(const std::string& type);
		Types::DataType getVarType(const std::string& var);
		std::string getDefaultValueForType(Types::DataType type);
		std::string dataTypeToString(Types::DataType type);
		std::string attributeTypeToString(Types::AttributeType type);
	}
}
