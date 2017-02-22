//Author : Sygmei
//Key : 976938ef7d46c286a2027d73f3a99467bcfa8ff0c1e10bd0016139744ef5404f4eb4d069709f9831f6de74a094944bf0f1c5bf89109e9855290336a66420376f

#pragma once

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <memory>

#include "Functions.hpp"

namespace vili
{
	namespace Types
	{
		enum AttributeType
		{
			Attribute = 0x0,
			ContainerAttribute = 0x1,
			BaseAttribute = 0x2,
			ListAttribute = 0x3,
			ComplexAttribute = 0x4,
			LinkAttribute = 0x5
		};

		enum DataType
		{
			String = 0x0,
			Int = 0x1,
			Float = 0x2,
			Bool = 0x3,
			Range = 0x4,
			Link = 0x5,
			Unknown = 0x6
		};

		Types::DataType stringToDataType(std::string type);
		Types::DataType getVarType(std::string var);
		std::string getDefaultValueForType(Types::DataType type);
		std::string dataTypeToString(Types::DataType type);
		std::string attributeTypeToString(Types::AttributeType type);
	}

	std::vector<std::string> convertPath(std::string path);

	class DataParserNavigator
	{
	private:
		std::string currentRootAttribute;
		std::string currentPath;
	public:
		std::string getFullPath();
		std::string getCurrentRootAttribute();
		std::string getCurrentPath();
		void setCurrentRootAttribute(const std::string& object);
		void setCurrentRootAttribute(const std::string& object, const std::string& path);
		void setCurrentPath(const std::string& path);
		void goTo(const std::string& path);
		void goRoot();
		void goBack();
	};

	class ContainerAttribute;
	class Attribute
	{
	protected:
		std::string id;
		Types::AttributeType type = Types::Attribute;
		std::string annotation = "";
		ContainerAttribute* parent = nullptr;
		virtual void removeParent(ContainerAttribute* currentParent);
		virtual ContainerAttribute* getParent();
		friend class ContainerAttribute;
	public:
		Attribute(ContainerAttribute* parent, const std::string& id, const Types::AttributeType& type);
		virtual ~Attribute() {}
		virtual void setAnnotation(const std::string& annotation);
		virtual std::string getAnnotation();
		virtual std::string getID();
		virtual Types::AttributeType getType();
		virtual void setParent(ContainerAttribute* parent);
		virtual std::string getNodePath();
		virtual void setID(const std::string& id);
	};

	class ContainerAttribute : public Attribute
	{
	protected:
	public:
		ContainerAttribute(ContainerAttribute* parent, const std::string& id, const Types::AttributeType& type);
		virtual ~ContainerAttribute() {}
		virtual Attribute* removeOwnership(Attribute* element);
		virtual Attribute* extractElement(Attribute* element) = 0;
	};

	class BaseAttribute : public Attribute
	{
	protected:
		std::string data;
		Types::DataType dtype;
	public:
		BaseAttribute(ContainerAttribute* parent, const std::string& id, const Types::DataType& btype, const std::string& bdata);
		BaseAttribute(const std::string& id, const Types::DataType& btype, const std::string& bdata);
		virtual ~BaseAttribute() {}
		std::string returnData();
		void set(int var);
		void set(double var);
		void set(const std::string& var);
		void set(bool var);
		Types::DataType getDataType();
		template <class T> T get() {}
	};

	//LinkAttribute
	class LinkAttribute : public BaseAttribute
	{
		private:
			std::string path = "";
		public:
			LinkAttribute(ContainerAttribute* parent, std::string id, std::string path);
			void update(ContainerAttribute* root);
	};
	

	class ListAttribute : public ContainerAttribute
	{
	private:
		std::vector<std::unique_ptr<BaseAttribute>> dataList;
	public:
		ListAttribute(ContainerAttribute* parent, const std::string& id);
		ListAttribute(const std::string& id);
		virtual ~ListAttribute() {}
		unsigned int getSize();
		BaseAttribute* get(unsigned int index);
		void push(const std::string& element);
		void push(const int& element);
		void push(const bool& element);
		void push(const double& element);
		void insert(unsigned int index, const std::string& element);
		void insert(unsigned int index, const int& element);
		void insert(unsigned int index, const bool& element);
		void insert(unsigned int index, const double& element);
		void clear();
		void erase(unsigned int index);
		virtual Attribute* extractElement(Attribute* element);
	};

	class ComplexAttribute : public ContainerAttribute
	{
		private:
			ComplexAttribute* parent = nullptr;
		protected:
			std::map<std::string, std::unique_ptr<Attribute>> childAttributes;
			std::vector<std::string> childAttributesNames;
		public:
			ComplexAttribute(ComplexAttribute* parent, const std::string& id);
			ComplexAttribute(ComplexAttribute* parent, const std::string& id, ComplexAttribute* herit);
			ComplexAttribute(ComplexAttribute* parent, const std::string& id, std::vector<ComplexAttribute*>* multipleHerit);
			ComplexAttribute(const std::string& id);
			ComplexAttribute(const std::string& id, ComplexAttribute* herit);
			ComplexAttribute(const std::string& id, std::vector<ComplexAttribute*>* multipleHerit);
			virtual ~ComplexAttribute() {}
			virtual Attribute* extractElement(Attribute* element);
			void heritage(ComplexAttribute* heritTarget);
			ComplexAttribute* getPath(std::string attributePath);
			ComplexAttribute& operator[](const std::string& cPath);
			ComplexAttribute* at(std::string cPath);
			template<class ...Args>
			ComplexAttribute* at(const std::string& cPath, Args ...pathParts);
			BaseAttribute* getBaseAttribute(const std::string& attributeName);
			ComplexAttribute* getComplexAttribute(const std::string& id);
			ListAttribute* getListAttribute(const std::string& id);
			Types::AttributeType getAttributeType(const std::string& id);
			std::vector<std::string> getAllAttributes();
			std::vector<std::string> getAllComplexAttributes();
			std::vector<std::string> getAllBaseAttributes();
			std::vector<std::string> getAllListAttributes();
			bool containsAttribute(const std::string& attributeName);
			bool containsBaseAttribute(const std::string& attributeName);
			bool containsComplexAttribute(const std::string& attributeName);
			bool containsListAttribute(const std::string& attributeName);
			void createBaseAttribute(const std::string& name, const Types::DataType& type, const std::string& data);
			void createBaseAttribute(const std::string& name, const std::string& data);
			void createBaseAttribute(const std::string& name, bool data);
			void createBaseAttribute(const std::string& name, int data);
			void createBaseAttribute(const std::string& name, double data);
			void pushBaseAttribute(BaseAttribute* attr);
			void createListAttribute(const std::string& id);
			void pushListAttribute(ListAttribute* attr);
			void createComplexAttribute(const std::string& id);
			void pushComplexAttribute(ComplexAttribute* cmplx);
			void writeAttributes(std::ofstream* file, unsigned int depth = 0);
			void deleteBaseAttribute(const std::string& id, bool freeMemory = false);
			void deleteComplexAttribute(const std::string& id, bool freeMemory = false);
			void deleteListAttribute(const std::string& id, bool freeMemory = false);
	};

	class DataParser
	{
	private:
		std::string fileName;
		std::unique_ptr<ComplexAttribute> root = nullptr;
		std::vector<std::string> flagList;
		DataParserNavigator* dpNav = NULL;
		bool checkNavigator();
	public:
		DataParser();
		~DataParser();
		DataParserNavigator* hookNavigator(DataParserNavigator* dpNav);
		DataParserNavigator* accessNavigator();
		void createFlag(const std::string& flag);
		void createRootAttribute(const std::string& id);
		bool containsRootAttribute(const std::string& objectName);
		ComplexAttribute* extractRootAttribute(const std::string& rootAttributeName);
		ComplexAttribute* getRootAttribute(const std::string& id);
		ComplexAttribute* getPath(std::string path);
		ComplexAttribute& operator[](std::string cPath);
		ComplexAttribute* at(std::string cPath);
		template<class ...Args>
		ComplexAttribute* at(const std::string& cPath, Args ...pathParts);
		bool parseFile(const std::string& filename, bool verbose = false);
		void writeFile(const std::string& filename, bool verbose = false);
		bool hasFlag(const std::string& flagName);
		unsigned int getAmountOfFlags();
		std::string getFlagAtIndex(int index);
		std::vector<std::string> getAllRootAttributes();
		void deleteRootAttribute(const std::string& name, bool freeMemory = false);
	};
}

namespace vili
{
	//BaseAttribute
	template <> inline int BaseAttribute::get() {
		if (dtype == Types::Int)
			return std::stoi(data);
		else
			std::cout << "<Error:DataParser:BaseAttribute>[getData] : " \
			<< getNodePath() << " is not a <int> BaseAttribute (" << dtype << ")" << std::endl;
	}
	template <> inline double BaseAttribute::get() {
		if (dtype == Types::Float)
			return std::stod(data);
		else
			std::cout << "<Error:DataParser:BaseAttribute>[getData] : " \
			<< getNodePath() << " is not a <float> BaseAttribute (" << dtype << ")" << std::endl;
	}
	template <> inline bool BaseAttribute::get() {
		if (dtype == Types::Bool)
			return (data == "True") ? true : false;
		else
			std::cout << "<Error:DataParser:BaseAttribute>[getData] : " \
			<< getNodePath() << " is not a <bool> BaseAttribute (" << dtype << ")" << std::endl;
	}
	template <> inline std::string BaseAttribute::get() {
		if (dtype == Types::String)
			return data;
		else
			std::cout << "<Error:DataParser:BaseAttribute>[getData] : " \
			<< getNodePath() << " is not a <string> BaseAttribute (" << dtype << ")" << std::endl;
	}

	//ComplexAttribute
	template<class ...Args>
	inline ComplexAttribute* ComplexAttribute::at(const std::string& cPath, Args ...pathParts)
	{
		return getPath(cPath)->at(pathParts...);
	}
	template<class ...Args>
	inline ComplexAttribute* DataParser::at(const std::string& cPath, Args ...pathParts)
	{
		return getPath(cPath)->at(pathParts...);
	}
}