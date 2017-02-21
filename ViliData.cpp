//Author : Sygmei
//Key : 976938ef7d46c286a2027d73f3a99467bcfa8ff0c1e10bd0016139744ef5404f4eb4d069709f9831f6de74a094944bf0f1c5bf89109e9855290336a66420376f

#include "ViliData.hpp"

namespace mse
{
	namespace Data
	{
		std::vector<std::string> convertPath(std::string path)
		{
			std::vector<std::string> vecPath = {};
			if (Functions::String::occurencesInString(path, "/") >= 1)
				vecPath = Functions::String::split(path, "/");
			else
			{
				if (path != "")
					vecPath.push_back(path);
			}
			return vecPath;
		}

		std::string Types::getDefaultValueForType(Types::DataType type)
		{
			std::string value = "";
			if (type == Types::String) value = "";
			if (type == Types::Int) value = "0";
			if (type == Types::Float) value = "0.0";
			if (type == Types::Bool) value = "False";
			return value;
		}

		Types::DataType Types::getVarType(std::string var)
		{
			Types::DataType attributeType;
			std::vector<std::string> boolType = { "True", "False" };
			if (Functions::Vector::isInList(var, boolType))
				attributeType = Types::Bool;
			else if (Functions::String::occurencesInString(var, "..") == 1 
			&& Functions::String::isStringInt(Functions::String::split(var, "..")[0]) 
			&& Functions::String::isStringInt(Functions::String::split(var, "..")[1])) {
				attributeType = Types::Range;
			}
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

		Types::DataType Types::stringToDataType(std::string type)
		{
			if (type == "string" || type == "String") {
				return Types::String;
			}
			else if (type == "int" || type == "Int") {
				return Types::Int;
			}
			else if (type == "float" || type == "Float") {
				return Types::Float;
			}
			else if (type == "bool" || type == "Bool") {
				return Types::Bool;
			}
			else if (type == "range" || type == "Range") {
				return Types::Range;
			}
			else {
				return Types::Unknown;
			}
		}

		std::string Types::dataTypeToString(Types::DataType type)
		{
			if (type == Types::String) return "String";
			else if (type == Types::Int) return "Int";
			else if (type == Types::Float) return "Float";
			else if (type == Types::Bool) return "Bool";
			else if (type == Types::Range) return "Range";
			else if (type == Types::Unknown) return "Unknown";
		}

		std::string Types::attributeTypeToString(Types::AttributeType type)
		{
			if (type == Types::Attribute) return "Attribute";
			else if (type == Types::ContainerAttribute) return "ContainerAttribute";
			else if (type == Types::BaseAttribute) return "BaseAttribute";
			else if (type == Types::ListAttribute) return "ListAttribute";
			else if (type == Types::ComplexAttribute) return "ComplexAttribute";
			else if (type == Types::LinkAttribute) return "LinkAttribute";
		}

		//Path
		std::string Path()
		{
			return "";
		}
		std::string Path(std::string cPath)
		{
			std::cout << "Returns Path : " << cPath << std::endl;
			if (cPath.size() > 0 && Functions::String::extract(cPath, cPath.size() - 1, 0) == "/")
				cPath = Functions::String::extract(cPath, 0, 1);
			return cPath;
		}
		std::string Path(std::vector<std::string> path)
		{
			std::string strPath = "";
			for (unsigned int i = 0; i < path.size(); i++) {
				strPath += path[i];
				if (i != path.size() - 1)
					strPath += "/";
			}
			return strPath;
		}

		//DataParserNavigator
		std::string DataParserNavigator::getFullPath()
		{
			return currentRootAttribute + ((currentPath != "") ? "/" : "") + currentPath;
		}
		std::string DataParserNavigator::getCurrentRootAttribute() {
			return currentRootAttribute;
		}
		std::string DataParserNavigator::getCurrentPath() {
			return currentPath;
		}
		void DataParserNavigator::setCurrentRootAttribute(const std::string& object) {
			currentRootAttribute = object;
			goRoot();
		}
		void DataParserNavigator::setCurrentRootAttribute(const std::string& object, const std::string& path) {
			currentRootAttribute = object;
			currentPath = path;
		}
		void DataParserNavigator::setCurrentPath(const std::string& path) {
			currentPath = path;
		}
		void DataParserNavigator::goTo(const std::string& path) {
			currentPath += "/" + path;
		}
		void DataParserNavigator::goRoot() {
			currentPath = "";
		}
		void DataParserNavigator::goBack() {
			if (currentPath != "")
			{
				if (Functions::String::split(currentPath, "/").size() > 1)
				{
					std::vector<std::string> splittedPath = Functions::String::split(currentPath, "/");
					currentPath = Functions::Vector::join(splittedPath, "/", 0, splittedPath.size() - 2);
				}

			}
		}

		//Attribute
		Attribute::Attribute(ContainerAttribute* parent, const std::string& id, const Types::AttributeType& type)
		{
			this->id = id;
			this->type = type;
			this->parent = parent;
		}
		void Attribute::removeParent(ContainerAttribute* currentParent)
		{
			if (currentParent == parent) {
				parent = nullptr;
			}
		}
		void Attribute::setAnnotation(const std::string& annotation)
		{
			this->annotation = annotation;
		}
		std::string Attribute::getAnnotation()
		{
			return annotation;
		}
		std::string Attribute::getID()
		{
			return id;
		}
		Types::AttributeType Attribute::getType()
		{
			return type;
		}
		ContainerAttribute* Attribute::getParent()
		{
			return parent;
		}
		void Attribute::setParent(ContainerAttribute* parent)
		{
			if (this->parent != nullptr) {
				this->parent = parent;
			}
			else {
				std::cout << "<Error:DataParser:Attribute>[setParent] : Attribute " << getNodePath() << " already has a parent" << std::endl;
			}
		}

		std::string Attribute::getNodePath()
		{
			std::vector<std::string> parentChain;
			ContainerAttribute* currentParent = this->getParent();
			while (currentParent != nullptr) {
				parentChain.push_back(currentParent->getID() +
					((currentParent->getAnnotation() != "") ? "<" + currentParent->getAnnotation() + ">" : ""));
				currentParent = currentParent->getParent();
			}
			std::reverse(parentChain.begin(), parentChain.end());
			parentChain.push_back(this->getID() + ((this->getAnnotation() != "") ? "<" + this->getAnnotation() + ">" : ""));
			return Functions::Vector::join(parentChain, "/");
		}

		void Attribute::setID(const std::string& id)
		{
			if (parent != nullptr) {
				std::cout << "<Error:DataParser:Attribute>[setID] : Can't change ID of " << getNodePath() << " when it has a parent" << std::endl;
			}
			else {
				this->id = id;
			}
		}

		ContainerAttribute::ContainerAttribute(ContainerAttribute* parent, const std::string& id, const Types::AttributeType& type) : Attribute(parent, id, type)
		{
		}
		Attribute* ContainerAttribute::removeOwnership(Attribute* element)
		{
			element->removeParent(this);
			return element;
		}

		//BaseAttribute
		BaseAttribute::BaseAttribute(ContainerAttribute* parent, const std::string& id, const Types::DataType& btype, const std::string& bdata) : Attribute(parent, id, Types::BaseAttribute) {
			dtype = btype;
			data = bdata;
			this->parent = parent;
			if (dtype == Types::String)
				Functions::String::replaceStringInPlace(data, "\"", "");
		}
		BaseAttribute::BaseAttribute(const std::string& id, const Types::DataType& btype, const std::string& bdata) : Attribute(nullptr, id, Types::BaseAttribute)
		{
			dtype = btype;
			data = bdata;
			this->parent = parent;
			if (dtype == Types::String)
				Functions::String::replaceStringInPlace(data, "\"", "");
		}
		void BaseAttribute::set(int var) {
			if (dtype == Types::Int)
				data = std::to_string(var);
			else
				std::cout << "<Error:DataParser:BaseAttribute>[setData] : " \
				<< getNodePath() << " is not a <int> BaseAttribute (" << dtype << ")" << std::endl;
		}
		void BaseAttribute::set(double var) {
			if (dtype == Types::Float)
				data = std::to_string(var);
			else
				std::cout << "<Error:DataParser:BaseAttribute>[setData] : " \
				<< getNodePath() << " is not a <float> BaseAttribute (" << dtype << ")" << std::endl;
		}
		void BaseAttribute::set(const std::string& var) {
			if (dtype == Types::String)
				data = var;
			else
				std::cout << "<Error:DataParser:BaseAttribute>[setData] : " \
				<< getNodePath() << " is not a <string> BaseAttribute (" << dtype << ")" << std::endl;
		}
		void BaseAttribute::set(bool var) {
			if (dtype == Types::Bool) {
				if (var)
					data = "True";
				else
					data = "False";
			}
			else
				std::cout << "<Error:DataParser:BaseAttribute>[setData] : " \
				<< getNodePath() << " is not a <bool> BaseAttribute (" << dtype << ")" << std::endl;
		}
		std::string BaseAttribute::returnData() {
			if (dtype != Types::String)
				return data;
			else
				return "\"" + data + "\"";
		}
		Types::DataType BaseAttribute::getDataType() {
			return dtype;
		}

		//ListAttribute
		ListAttribute::ListAttribute(ContainerAttribute* parent, const std::string& id) : ContainerAttribute(parent, id, Types::ListAttribute) 
		{
		}
		ListAttribute::ListAttribute(const std::string& id) : ContainerAttribute(nullptr, id, Types::ListAttribute)
		{
		}
		unsigned int ListAttribute::getSize() {
			return dataList.size();
		}
		BaseAttribute* ListAttribute::getElement(unsigned int index) {
			if (index < dataList.size())
				return dataList[index].get();
			else
				std::cout << "<Error:DataParser:ListAttribute> : Can't access index " << index << " of " \
				<< getNodePath() << " (Size:" << dataList.size() << ")" << std::endl;
		}
		void ListAttribute::createElement(const std::string& element) 
		{
			dataList.push_back(std::make_unique<BaseAttribute>(this, "#" + std::to_string(dataList.size()), Types::String, element));
		}
		void ListAttribute::createElement(const int& element) 
		{
			dataList.push_back(std::make_unique<BaseAttribute>(this, "#" + std::to_string(dataList.size()), Types::Int, std::to_string(element)));
		}
		void ListAttribute::createElement(const bool& element) 
		{
			dataList.push_back(std::make_unique<BaseAttribute>(this, "#" + std::to_string(dataList.size()), Types::Bool, (element == true) ? "True" : "False"));
		}
		void ListAttribute::createElement(const double& element) 
		{
			dataList.push_back(std::make_unique<BaseAttribute>(this, "#" + std::to_string(dataList.size()), Types::Float, std::to_string(element)));
		}
		Attribute* ListAttribute::extractElement(Attribute* element)
		{
			if (Functions::Vector::isInList(std::unique_ptr<BaseAttribute>(dynamic_cast<BaseAttribute*>(element)), dataList)) {
				this->removeOwnership(element);
				int indexToRelease = Functions::Vector::indexOfElement(
					std::unique_ptr<BaseAttribute>(dynamic_cast<BaseAttribute*>(element)), dataList);
				dataList[indexToRelease].release();
				Functions::Vector::eraseAll(dataList, std::unique_ptr<BaseAttribute>(dynamic_cast<BaseAttribute*>(element)));
			}
			return element;
		}


		//ComplexAttribute
		ComplexAttribute::ComplexAttribute(ComplexAttribute* parent, const std::string& id) : ContainerAttribute(parent, id, Types::ComplexAttribute) {
			this->parent = parent;
		}
		ComplexAttribute::ComplexAttribute(ComplexAttribute* parent, const std::string& id, ComplexAttribute* herit)
			: ContainerAttribute(parent, id, Types::ComplexAttribute) {
			this->parent = parent;
			this->heritage(herit);
		}
		ComplexAttribute::ComplexAttribute(ComplexAttribute* parent, const std::string& id, std::vector<ComplexAttribute*>* multipleHerit)
			: ContainerAttribute(parent, id, Types::ComplexAttribute) {
			this->parent = parent;
			for (unsigned int i = 0; i < multipleHerit->size(); i++)
				this->heritage(multipleHerit->at(i));
		}
		ComplexAttribute::ComplexAttribute(const std::string& id) : ContainerAttribute(nullptr, id, Types::ComplexAttribute)
		{
		}
		ComplexAttribute::ComplexAttribute(const std::string& id, ComplexAttribute* herit) : ContainerAttribute(nullptr, id, Types::ComplexAttribute)
		{
			this->heritage(herit);
		}
		ComplexAttribute::ComplexAttribute(const std::string& id, std::vector<ComplexAttribute*>* multipleHerit)
			: ContainerAttribute(nullptr, id, Types::ComplexAttribute)
		{
			for (unsigned int i = 0; i < multipleHerit->size(); i++)
				this->heritage(multipleHerit->at(i));
		}
		Attribute* ComplexAttribute::extractElement(Attribute* element)
		{
			if (Functions::Vector::isInList(element->getID(), childAttributesNames)) {
				this->removeOwnership(element);
				childAttributes[element->getID()].release();
				childAttributes.erase(element->getID());
				Functions::Vector::eraseAll(childAttributesNames, element->getID());
			}
			else {
				std::cout << "<Error:DataParser:ComplexAttribute>[extractElement] : " << this->getID() << " doesn't contain Attribute " << element->getID() << std::endl;
			}
			return element;
		}
		void ComplexAttribute::heritage(ComplexAttribute* heritTarget) {
			for (unsigned int i = 0; i < heritTarget->getAllBaseAttributes().size(); i++)
			{
				BaseAttribute* currentBaseAttr = heritTarget->getBaseAttribute(heritTarget->getAllBaseAttributes()[i]);
				this->createBaseAttribute(currentBaseAttr->getID(), currentBaseAttr->getDataType(), currentBaseAttr->returnData());
			}
			for (unsigned int i = 0; i < heritTarget->getAllComplexAttributes().size(); i++)
			{
				ComplexAttribute* currentCmplxAttr = heritTarget->getComplexAttribute(heritTarget->getAllComplexAttributes()[i]);
				this->createComplexAttribute(currentCmplxAttr->getID());
				this->getComplexAttribute(heritTarget->getAllComplexAttributes()[i])->heritage(currentCmplxAttr);
			}
		}
		ComplexAttribute* ComplexAttribute::operator[](const std::string& cPath)
		{
			return getPath(cPath);
		}
		ComplexAttribute* ComplexAttribute::at(std::string cPath)
		{
			if (cPath.size() > 0 && Functions::String::extract(cPath, cPath.size() - 1, 0) == "/")
				cPath = Functions::String::extract(cPath, 0, 1);
			return getPath(cPath);
		}
		ComplexAttribute* ComplexAttribute::getPath(std::string attributePath)
		{
			if (attributePath.size() > 0 && Functions::String::extract(attributePath, attributePath.size() - 1, 0) == "/")
				attributePath = Functions::String::extract(attributePath, 0, 1);
			std::vector<std::string> sPath = convertPath(attributePath);
			if (sPath.size() > 0) {
				int pathIndex = 1;
				ComplexAttribute* getToPath;
				getToPath = this->getComplexAttribute(sPath[0]);
				while (pathIndex != sPath.size()) {
					getToPath = getToPath->getComplexAttribute(sPath[pathIndex]);
					pathIndex++;
				}
				return getToPath;
			}
			else {
				return this;
			}
		}
		BaseAttribute* ComplexAttribute::getBaseAttribute(const std::string& id) {
			if (childAttributes.find(id) != childAttributes.end() && childAttributes[id]->getType() == Types::BaseAttribute)
				return dynamic_cast<BaseAttribute*>(childAttributes[id].get());
			else {
				std::cout << "<Error:DataParser:ComplexAttribute>[getBaseAttribute] : Can't find BaseAttribute "
					<< id << " in " << getNodePath() << std::endl;
			}
		}
		ListAttribute* ComplexAttribute::getListAttribute(const std::string& id) {
			if (childAttributes.find(id) != childAttributes.end() && childAttributes[id]->getType() == Types::ListAttribute)
				return dynamic_cast<ListAttribute*>(childAttributes[id].get());
			else {
				std::cout << "<Error:DataParser:ComplexAttribute>[getListAttribute] : Can't find ListAttribute "
					<< id << " in " << getNodePath() << std::endl;
			}
		}
		ComplexAttribute* ComplexAttribute::getComplexAttribute(const std::string& id) {
			if (childAttributes.find(id) != childAttributes.end() && childAttributes[id]->getType() == Types::ComplexAttribute) {
				return dynamic_cast<ComplexAttribute*>(childAttributes[id].get());
			}
			else {
				std::cout << "<Error:DataParser:ComplexAttribute>[getComplexAttribute] : Can't find ComplexAttribute "
					<< id << " in " << getNodePath() << std::endl;
			}
		}
		Types::AttributeType ComplexAttribute::getAttributeType(const std::string& id)
		{
			if (childAttributes.find(id) != childAttributes.end())
				return childAttributes[id]->getType();
			else
				std::cout << "<Error:DataParser:ComplexAttribute>[getAttributeType] : Attribute " << id << " does not exists" << std::endl;
		}
		std::vector<std::string> ComplexAttribute::getAllAttributes()
		{
			return this->childAttributesNames;
		}
		std::vector<std::string> ComplexAttribute::getAllComplexAttributes() {
			std::vector<std::string> allComplexAttributes;
			for (std::string attributeName : this->childAttributesNames) {
				if (this->childAttributes[attributeName]->getType() == Types::ComplexAttribute) {
					allComplexAttributes.push_back(attributeName);
				}
			}
			return allComplexAttributes;
		}
		std::vector<std::string> ComplexAttribute::getAllBaseAttributes() {
			std::vector<std::string> allBaseAttributes;
			for (std::string attributeName : this->childAttributesNames) {
				if (this->childAttributes[attributeName]->getType() == Types::BaseAttribute) {
					allBaseAttributes.push_back(attributeName);
				}
			}
			return allBaseAttributes;
		}
		std::vector<std::string> ComplexAttribute::getAllListAttributes() {
			std::vector<std::string> allListAttributes;
			for (std::string attributeName : this->childAttributesNames) {
				if (this->childAttributes[attributeName]->getType() == Types::ListAttribute) {
					allListAttributes.push_back(attributeName);
				}
			}
			return allListAttributes;
		}
		bool ComplexAttribute::containsAttribute(const std::string& attributeName)
		{
			if (childAttributes.find(attributeName) != childAttributes.end())
				return true;
			else
				return false;
		}
		bool ComplexAttribute::containsBaseAttribute(const std::string& attributeName) {
			if (childAttributes.find(attributeName) != childAttributes.end() && childAttributes[attributeName]->getType() == Types::BaseAttribute)
				return true;
			else
				return false;
		}
		bool ComplexAttribute::containsComplexAttribute(const std::string& attributeName) {
			if (childAttributes.find(attributeName) != childAttributes.end() && childAttributes[attributeName]->getType() == Types::ComplexAttribute)
				return true;
			else
				return false;
		}
		bool ComplexAttribute::containsListAttribute(const std::string& attributeName) {
			if (childAttributes.find(attributeName) != childAttributes.end() && childAttributes[attributeName]->getType() == Types::ListAttribute)
				return true;
			else
				return false;
		}
		void ComplexAttribute::createBaseAttribute(const std::string& name, const Types::DataType& type, const std::string& data)
		{
			childAttributes[name] = std::make_unique<BaseAttribute>(this, name, type, data);
			childAttributesNames.push_back(name);
		}
		void ComplexAttribute::createBaseAttribute(const std::string& name, const std::string& data)
		{
			childAttributes[name] = std::make_unique<BaseAttribute>(this, name, Types::String, data);
			childAttributesNames.push_back(name);
		}
		void ComplexAttribute::createBaseAttribute(const std::string& name, bool data)
		{
			childAttributes[name] = std::make_unique<BaseAttribute>(this, name, Types::Bool, (data == true ? "True" : "False"));
			childAttributesNames.push_back(name);
		}
		void ComplexAttribute::createBaseAttribute(const std::string& name, int data)
		{
			childAttributes[name] = std::make_unique<BaseAttribute>(this, name, Types::Int, std::to_string(data));
			childAttributesNames.push_back(name);
		}
		void ComplexAttribute::createBaseAttribute(const std::string& name, double data)
		{
			childAttributes[name] = std::make_unique<BaseAttribute>(this, name, Types::Float, std::to_string(data));
			childAttributesNames.push_back(name);
		}
		void ComplexAttribute::pushBaseAttribute(BaseAttribute* attr) {
			childAttributes[attr->getID()] = std::unique_ptr<BaseAttribute>(attr);
			childAttributesNames.push_back(attr->getID());
		}
		void ComplexAttribute::createListAttribute(const std::string& id) {
			childAttributes[id] = std::make_unique<ListAttribute>(this, id);
			childAttributesNames.push_back(id);
		}
		void ComplexAttribute::pushListAttribute(ListAttribute* attr) {
			childAttributes[attr->getID()] = std::unique_ptr<ListAttribute>(attr);
			childAttributesNames.push_back(attr->getID());
		}
		void ComplexAttribute::createListItem(const std::string& listID, const std::string& value) {
			this->getListAttribute(listID)->createElement(value);
		}
		void ComplexAttribute::createComplexAttribute(const std::string& id) {
			childAttributes[id] = std::make_unique<ComplexAttribute>(this, id);
			childAttributesNames.push_back(id);
		}
		void ComplexAttribute::pushComplexAttribute(ComplexAttribute* cmplx) {
			childAttributes[cmplx->getID()] = std::unique_ptr<ComplexAttribute>(cmplx);
			childAttributesNames.push_back(cmplx->getID());
		}
		void ComplexAttribute::writeAttributes(std::ofstream* file, unsigned int depth) {
			for (unsigned int i = 0; i < depth; i++)
				(*file) << "    ";
			if (depth)
				(*file) << "@" << id << std::endl;
			else
				(*file) << id << ":" << std::endl;
			for (unsigned int i = 0; i < this->getAllBaseAttributes().size(); i++)
			{
				for (unsigned int j = 0; j < depth + 1; j++)
					(*file) << "    ";
				(*file) << this->getAllBaseAttributes()[i] << ":" << this->getBaseAttribute(getAllBaseAttributes()[i])->returnData() << std::endl;
			}
			for (unsigned int i = 0; i < this->getAllListAttributes().size(); i++)
			{
				for (unsigned int j = 0; j < depth + 1; j++)
					(*file) << "    ";
				(*file) << this->getAllListAttributes()[i] << ":[" << std::endl;
				for (unsigned int k = 0; k < this->getListAttribute(this->getAllListAttributes()[i])->getSize(); k++)
				{
					for (unsigned int l = 0; l < depth + 2; l++)
						(*file) << "    ";
					(*file) << this->getListAttribute(this->getAllListAttributes()[i])->getElement(k)->returnData() << std::endl;
				}
				(*file) << "]" << std::endl;
			}
			for (unsigned int i = 0; i < this->getAllComplexAttributes().size(); i++)
				this->getComplexAttribute(this->getAllComplexAttributes()[i])->writeAttributes(file, depth + 1);
			if (depth == 0)
				(*file) << std::endl;
		}
		void ComplexAttribute::deleteBaseAttribute(const std::string& id, bool freeMemory) {
			if (Functions::Vector::isInList(id, this->getAllBaseAttributes())) {
				childAttributesNames.erase(childAttributesNames.begin() + Functions::Vector::indexOfElement(id, this->getAllBaseAttributes()));
			}
			typedef std::map<std::string, std::unique_ptr<Attribute>>::iterator it_type;
			it_type itDel = childAttributes.find(id);
			if (itDel != childAttributes.end() && childAttributes[id]->getType() == Types::BaseAttribute) {
				if (!freeMemory)
					childAttributes[id].release();
				childAttributes.erase(itDel);
			}
		}
		void ComplexAttribute::deleteComplexAttribute(const std::string& id, bool freeMemory) {
			if (Functions::Vector::isInList(id, this->getAllComplexAttributes())) {
				childAttributesNames.erase(childAttributesNames.begin() + Functions::Vector::indexOfElement(id, this->getAllComplexAttributes()));
			}
			typedef std::map<std::string, std::unique_ptr<Attribute>>::iterator it_type;
			it_type itDel = childAttributes.find(id);
			if (itDel != childAttributes.end() && childAttributes[id]->getType() == Types::ComplexAttribute) {
				if (!freeMemory)
					childAttributes[id].release();
				childAttributes.erase(itDel);
			}
		}
		void ComplexAttribute::deleteListAttribute(const std::string& id, bool freeMemory) {
			if (Functions::Vector::isInList(id, this->getAllListAttributes())) {
				childAttributesNames.erase(childAttributesNames.begin() + Functions::Vector::indexOfElement(id, this->getAllListAttributes()));
			}
			typedef std::map<std::string, std::unique_ptr<Attribute>>::iterator it_type;
			it_type itDel = childAttributes.find(id);
			if (itDel != childAttributes.end() && childAttributes[id]->getType() == Types::ListAttribute) {
				if (!freeMemory)
					childAttributes[id].release();
				childAttributes.erase(itDel);
			}
		}

		//ModelObject
		/*std::map<std::string, ObjectModel*> ObjectModel::modelMap = std::map<std::string, ObjectModel*>();
		ObjectModel::ObjectModel(std::string modelName, std::string objectSignature) : signature("ModelSignature"), body("ModelBody")
		{
			name = modelName;
			Functions::String::StringExtractor signatureRes = Functions::String::extractAllStrings(objectSignature);
			std::string signatureWithoutStrings = Functions::Vector::join(std::get<1>(signatureRes), "&string");
			std::vector<std::string> allParameters = Functions::String::split(signatureWithoutStrings, ",");
			for (int i = 0; i < allParameters.size(); i++) {
				std::vector<std::string> splittedParameter = Functions::String::split(allParameters[i], ":");
				std::string parameterType = Functions::String::replaceString(splittedParameter[0], " ", "");
				std::vector<std::string> parameterRightPart = Functions::String::split(Functions::String::replaceString(splittedParameter[1], " ", ""), "=");
				std::string parameterName = parameterRightPart[0];
				std::string defaultValue = (parameterRightPart.size() == 2) ? parameterRightPart[1] : getDefaultValueForType(parameterType);
				signature.createBaseAttribute(parameterName, parameterType, defaultValue);
			}
		}
		ComplexAttribute ObjectModel::buildModel(std::string objectName, std::string buildSignature)
		{
			Functions::String::StringExtractor signatureRes = Functions::String::extractAllStrings(buildSignature);
			std::string signatureWithoutStrings = Functions::Vector::join(std::get<1>(signatureRes), "&string");
			std::vector<std::string> allParameters = Functions::String::split(signatureWithoutStrings, ",");
			std::vector<std::string> parameterList;
			int stringIndex = 0;
			for (std::string parameter : allParameters) {
				Functions::String::replaceStringInPlace(parameter, " ", "");
				if (parameter == "&string") {
					parameterList.push_back("\"" + std::get<0>(signatureRes)[stringIndex] + "\"");
					stringIndex++;
				}
				else {
					parameterList.push_back(parameter);
				}
			}
			ComplexAttribute objectParameters(objectName);
			std::vector<std::string> signatureAttributes = signature.getAllBaseAttributes();
			if (parameterList.size() > signatureAttributes.size()) {
				std::cout << "<Error:DataParser:ObjectModel>[buildModel] : Too many parameters provided for Model : "
					<< name << " (" << signatureAttributes.size() << " parameters maximum)" << std::endl;
			}
			for (int i = 0; i < signatureAttributes.size(); i++) {
				BaseAttribute* currentSignatureParameter = signature.getBaseAttribute(signatureAttributes[i]);
				if (i < parameterList.size()) {
					if (getVarType(parameterList[i]) == currentSignatureParameter->getDataType()) {
						objectParameters.createBaseAttribute(currentSignatureParameter->getID(),
							currentSignatureParameter->getDataType(), parameterList[i]);
					}
					else {
						std::cout << "<Error:DataParser:ObjectModel>[buildModel] : Type mismatch at parameter "
							<< currentSignatureParameter->getID() << " for Model " << name << std::endl;
					}
				}
				else {
					objectParameters.createBaseAttribute(currentSignatureParameter->getID(),
						currentSignatureParameter->getDataType(), currentSignatureParameter->returnData());
				}
			}
			ComplexAttribute linkedBody = body;
			//linkedBody.resolveLinking(&objectParameters);
		}
		void ObjectModel::CreateModel(std::string modelName, std::string objectSignature)
		{
			if (modelMap.find(modelName) != modelMap.end()) {
				delete modelMap[modelName];
			}
			modelMap[modelName] = new ObjectModel(modelName, objectSignature);
		}
		ComplexAttribute ObjectModel::BuildObjectFromModel(std::string modelName, std::string objectName, std::string buildSignature)
		{
			if (modelMap.find(modelName) != modelMap.end()) {
				return modelMap[modelName]->buildModel(objectName, buildSignature);
			}
			else {
				std::cout << "<Error:DataParser:ObjectModel>[BuildObjectFromModel] : Can't find ObjectModel : " << modelName << std::endl;
			}
		}*/

		//DataParser
		DataParser::DataParser()
		{
			root = std::make_unique<ComplexAttribute>("root");
			root->setAnnotation("NoFile");
		}
		DataParser::~DataParser()
		{
			std::cout << "Deleting DataParser !" << std::endl;
			//delete root.get();
		}
		DataParserNavigator* DataParser::hookNavigator(DataParserNavigator* dpNav) {
			return (this->dpNav = dpNav);
		}
		DataParserNavigator* DataParser::accessNavigator() {
			return this->dpNav;
		}
		bool DataParser::checkNavigator() {
			return dpNav != NULL;
		}
		void DataParser::createFlag(const std::string& flag) {
			flagList.push_back(flag);
		}
		bool DataParser::hasFlag(const std::string& flagName) {
			return std::find(flagList.begin(), flagList.end(), flagName) != flagList.end();
		}
		unsigned int DataParser::getAmountOfFlags() {
			return flagList.size();
		}
		std::string DataParser::getFlagAtIndex(int index) {
			return flagList[index];
		}
		void DataParser::createRootAttribute(const std::string& id)
		{
			root->createComplexAttribute(id);
			if (checkNavigator()) {
				dpNav->setCurrentRootAttribute(id);
			}
		}
		bool DataParser::containsRootAttribute(const std::string& objectName)
		{
			return (root->containsComplexAttribute(objectName));
		}
		ComplexAttribute* DataParser::extractRootAttribute(const std::string& rootAttributeName)
		{
			ComplexAttribute* returnAttribute = this->getRootAttribute(rootAttributeName);
			root->extractElement(this->getRootAttribute(rootAttributeName));
			return returnAttribute;
		}
		ComplexAttribute* DataParser::getRootAttribute(const std::string& id)
		{
			if (containsRootAttribute(id)) {
				return root->getComplexAttribute(id);
			}
			else {
				std::cout << "<Error:DataParser:DataParser>[getRootAttribute] : Can't find RootAttribute : "
					<< id << " on " << root->getAnnotation() << std::endl;
			}
		}
		ComplexAttribute* DataParser::getPath(std::string path)
		{
			if (path.size() > 0 && Functions::String::extract(path, path.size() - 1, 0) == "/")
				path = Functions::String::extract(path, 0, 1);
			if (Functions::String::occurencesInString(path, "/") > 0) {
				std::vector<std::string> splittedPath = Functions::String::split(path, "/");
				std::string subPath = Functions::Vector::join(splittedPath, "/", 1);
				std::cout << "Go to : " << Functions::String::split(path, "/")[0] << " then " << subPath << std::endl;
				return getRootAttribute(Functions::String::split(path, "/")[0])->at(subPath);
			}
			else {
				return getRootAttribute(path);
			}
		}
		ComplexAttribute* DataParser::operator[](std::string cPath)
		{
			return getPath(cPath);
		}
		ComplexAttribute* DataParser::at(std::string cPath)
		{
			if (cPath.size() > 0 && Functions::String::extract(cPath, cPath.size() - 1, 0) == "/")
				cPath = Functions::String::extract(cPath, 0, 1);
			return getPath(cPath);
		}
		bool DataParser::parseFile(const std::string& filename, bool verbose) {
			std::ifstream useFile;
			useFile.open(filename);
			std::string currentLine;
			std::vector<std::string> addPath = {};
			unsigned int spacing = 4;
			std::string curCat = "None";
			std::string curList = "None";
			int curListIndent = 0;
			if (useFile.is_open())
			{
				if (verbose) std::cout << "Start Parsing File : " << filename << std::endl;
				while (getline(useFile, currentLine)) 
				{
					Functions::String::StringExtractor stringsInLine = Functions::String::extractAllStrings(currentLine);
					std::string rawLine = Functions::Vector::join(std::get<1>(stringsInLine), "");
					Functions::String::replaceStringInPlace(rawLine, "	", std::string(spacing, ' '));
					unsigned int currentIndent = 0;
					unsigned int spacingAmount = 0;
					for (int i = 0; i < rawLine.size(); i++) {
						if (rawLine[i] == ' ') {
							spacingAmount += 1;
							if (spacingAmount == spacing) {
								spacingAmount = 0;
								currentIndent += 1;
							}
						}
						else
							break;
					}
					Functions::String::replaceStringInPlace(rawLine, std::string(spacing, ' '), "");
					std::vector<std::string> parsedLines;
					std::string addParsedLine = "";
					for (int i = 0; i < std::get<2>(stringsInLine).size(); i++) {
						if (std::get<2>(stringsInLine)[i].first == 0) {
							addParsedLine += "\"" + std::get<0>(stringsInLine)[std::get<2>(stringsInLine)[i].second] + "\"";
						}
						else {
							std::string removeRawSpacing = Functions::String::replaceString(std::get<1>(stringsInLine)[std::get<2>(stringsInLine)[i].second], "	", "");
							Functions::String::replaceStringInPlace(removeRawSpacing, "	", ""); //Tabs
							Functions::String::replaceStringInPlace(removeRawSpacing, " ", ""); //Spaces
							for (int j = 0; j < removeRawSpacing.size(); j++) {
								std::string currentRawChar = removeRawSpacing.substr(j, 1);
								if (currentRawChar == ",") {
									parsedLines.push_back(addParsedLine);
									addParsedLine = "";
								}
								else if (currentRawChar == "[") {
									parsedLines.push_back(addParsedLine + "[");
									addParsedLine = "";
								}
								else if (currentRawChar == "]") {
									parsedLines.push_back(addParsedLine);
									parsedLines.push_back("]");
									addParsedLine = "";
								}
								else {
									addParsedLine += currentRawChar;
								}
							}
						}
					}
					parsedLines.push_back(addParsedLine);
					for (std::string parsedLine : parsedLines) {
						while (currentIndent < addPath.size() + 1 && addPath.size() > 0)
							addPath.pop_back();
						std::function<std::string()> indenter = [curList, currentIndent, curListIndent](){
							std::string strIndent = "";
							if (curList == "None") {
								for (unsigned int p = 0; p <= currentIndent; p++)
									strIndent += "    ";
							}
							else {
								for (unsigned int p = 0; p <= curListIndent + 1; p++)
									strIndent += "    ";
							}
							return strIndent; 
						};
						if (curList != "None" && parsedLine == "]")
							curList = "None";
						if (parsedLine != "" && currentIndent == 0) 
						{
							if (parsedLine.substr(parsedLine.size() - 1, 1) == ":") 
							{
								if (verbose) std::cout << indenter() << "Create new RootAttribute : " << parsedLine.substr(0, parsedLine.size() - 1) << std::endl;
								curCat = parsedLine.substr(0, parsedLine.size() - 1);
								this->createRootAttribute(curCat);
							}
							else if (parsedLine.substr(parsedLine.size() - 1, 1) == ";")
							{
								std::string instructionType = Functions::String::split(parsedLine, "(")[0];
								std::string instructionValue = Functions::String::split(parsedLine, "(")[1];
								instructionValue = Functions::String::split(instructionValue, ")")[0];
								if (instructionType == "Flag")
								{
									flagList.push_back(instructionValue);
									if (verbose) std::cout << indenter() << "Define New Flag : " << instructionValue << std::endl;
								}
								else if (instructionType == "Spacing")
								{
									spacing = std::stoi(instructionValue);
									if (verbose) std::cout << indenter() << "Define New Spacing : " << spacing << std::endl;
								}
								else if (instructionType == "Include")
								{
									if (verbose) std::cout << indenter() << "Include New File : " << instructionValue << std::endl;
									this->parseFile(instructionValue, verbose);
								}
							}
						}
						else if (curCat != "None" && parsedLine != "" && currentIndent > 0)
						{
							std::vector<std::string> symbolExclusion = { "@" , "\"", "#"};
							if (!Functions::Vector::isInList(parsedLine.substr(0, 1), symbolExclusion) && Functions::String::occurencesInString(parsedLine, ":[") == 1)
							{
								std::string listElementID = Functions::String::split(parsedLine, ":")[0];
								getRootAttribute(curCat)->getPath(Path(addPath))->createListAttribute(listElementID);
								curList = listElementID;
								curListIndent = currentIndent;
								if (verbose) {
									std::cout << indenter() << "Create ListAttribute " << curList << " inside "
										<< curCat + ":" + getRootAttribute(curCat)->getPath(Path(addPath))->getID() << std::endl;
								}
							}
							else if (!Functions::Vector::isInList(parsedLine.substr(0, 1), symbolExclusion) && Functions::String::occurencesInString(parsedLine, ":"))
							{
								std::string attributeName = Functions::String::split(parsedLine, ":")[0];
								std::vector<std::string> splittedLine = Functions::String::split(parsedLine, ":");
								std::string attributeValue = Functions::Vector::join(splittedLine, ":", 1, 0);
								Types::DataType attributeType = Types::getVarType(attributeValue);
								getRootAttribute(curCat)->getPath(Path(addPath))->createBaseAttribute(attributeName, attributeType, attributeValue);
								std::string pushIndicator;
								if (addPath.size() == 0) { pushIndicator = curCat + ":Root"; }
								else { pushIndicator = addPath[addPath.size() - 1]; }
								if (verbose) {
									std::cout << indenter() << "Create BaseAttribute " << attributeName << "(" << attributeValue;
									std::cout << ") inside " << pushIndicator << " (Type:" << attributeType << ")" << std::endl;
								}
									
							}
							else if (parsedLine.substr(0, 1) == "@")
							{
								std::string complexElementID = parsedLine.substr(1);
								std::string complexToHeritID;
								std::vector<std::string> complexToHeritIDList;
								std::vector<ComplexAttribute*> complexToHerit;
								if (Functions::String::occurencesInString(complexElementID, ":") == 1 && Functions::String::split(complexElementID, ":").size() == 2)
								{
									complexToHeritID = Functions::String::replaceString(Functions::String::split(complexElementID, ":")[1], " ", "");
									complexElementID = Functions::String::split(complexElementID, ":")[0];
									complexToHeritIDList = Functions::String::split(complexToHeritID, ",");

									for (unsigned int i = 0; i < complexToHeritIDList.size(); i++) {
										complexToHerit.push_back(getRootAttribute(curCat)->getComplexAttribute(complexToHeritIDList[i]));
									}
								}
								getRootAttribute(curCat)->getPath(Path(addPath))->createComplexAttribute(complexElementID);
								if (verbose) std::cout << indenter() << "Create ComplexAttribute @" << complexElementID << " inside " << curCat + ":" \
									+ getRootAttribute(curCat)->getPath(Path(addPath))->getID() << std::endl;
								for (unsigned int i = 0; i < complexToHerit.size(); i++) {
									if (verbose) std::cout << indenter() << "    {Herit from : " << complexToHeritIDList[i] << "}" << std::endl;
									getRootAttribute(curCat)->getPath(Path(addPath))->getComplexAttribute(complexElementID)->heritage(complexToHerit[i]);
								}

								addPath.push_back(complexElementID);
							}
							else if (curList != "None")
							{
								std::string attributeValue = parsedLine;
								Types::DataType attributeType = Types::getVarType(attributeValue);
								std::cout << indenter() << "Create Element #" << getRootAttribute(curCat)->getPath(Path(addPath))->getListAttribute(curList)->getSize() 
								<< "(" << attributeValue << ") of ListAttribute " << curList << std::endl;
								if (attributeType == Types::String) {
									getRootAttribute(curCat)->getPath(Path(addPath))->getListAttribute(curList)->createElement(attributeValue);
								}
								else if (attributeType == Types::Int) {
									getRootAttribute(curCat)->getPath(Path(addPath))->getListAttribute(curList)->createElement(std::stoi(attributeValue));
								}
								else if (attributeType == Types::Float) {
									getRootAttribute(curCat)->getPath(Path(addPath))->getListAttribute(curList)->createElement(std::stod(attributeValue));
								}
								else if (attributeType == Types::Bool) {
									getRootAttribute(curCat)->getPath(Path(addPath))->getListAttribute(curList)->createElement(((attributeValue == "True") ? true : false));
								}
								else if (attributeType == Types::Range) {
									int rStart = std::stoi(Functions::String::split(attributeValue, "..")[0]);
									int rEnd = std::stoi(Functions::String::split(attributeValue, "..")[1]);
									int step = 1;
									if (rStart > rEnd)
										step = -1;
									rEnd += step;
									for (int i = rStart; i != rEnd; i+= step) {
										getRootAttribute(curCat)->getPath(Path(addPath))->getListAttribute(curList)->createElement(i);
									}
								}
								else {
									std::cout << "<Error:ViliData:DataParser>[parseFile] : Can't append unknown type to list (" << attributeValue << ")" << std::endl;
								}
							}
						}
					}
				}
				useFile.close();
				root->setAnnotation(filename);
				if (verbose) std::cout << "Parsed over.." << std::endl;
				return true;
			}
			else
			{
				std::cout << "<Error:DataParser:DataParser>[parseFile] : Can't open " << filename << ". File does not exists" << std::endl;
				return false;
			}
		}
		void DataParser::writeFile(const std::string& filename, bool verbose) {
			std::ofstream outFile;
			outFile.open(filename);
			if (verbose) std::cout << "Writing DataParser's content on file : " << filename << std::endl;
			unsigned int currentDeepness = 1;
			if (verbose && this->getAmountOfFlags() > 0) std::cout << "    Writing Flags..." << std::endl;
			for (unsigned int i = 0; i < this->getAmountOfFlags(); i++) {
				outFile << "Flag (" << this->getFlagAtIndex(i) << ");" << std::endl;
				if (verbose) std::cout << "        Write New Flag : " << this->getFlagAtIndex(i) << std::endl;
			}
			if (this->getAmountOfFlags() > 0) outFile << std::endl;
			if (verbose && root->getAllComplexAttributes().size() > 0) std::cout << "    Writing RootAttributes..." << std::endl;
			for (std::string& currentRootAttribute : root->getAllComplexAttributes()) {
				if (verbose) std::cout << "        Write New RootAttribute : " << currentRootAttribute << std::endl;
				getRootAttribute(currentRootAttribute)->writeAttributes(&outFile);
			}
			outFile.close();
		}
		std::vector<std::string> DataParser::getAllRootAttributes() {
			return root->getAllComplexAttributes();
		}
		void DataParser::deleteRootAttribute(const std::string& name, bool freeMemory)
		{
			root->deleteComplexAttribute(name, freeMemory);
		}
	}
}