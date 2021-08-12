#pragma once
#include "MyBTree.h"
#include "MyBTree.cpp"
#include "ControllerCore.h"
#include <sstream>

template <typename T>
class BaseTemplateControllerCore : public ControllerCore {
protected:
	
	MyBTree<T>* _tree;
	virtual T processGetValueFromStr(std::string str) = 0;

public:
	BaseTemplateControllerCore(unsigned int order) {
		_tree = new MyBTree<T>(order);
	};

	bool addToTree(std::string key) {
		try {
			_tree->add(processGetValueFromStr(key));
			return true;
		}catch (const std::invalid_argument &exp) {
			return false;
		}
	};

	bool findInTree(std::string key, std::string &error) {
		try {
			error.append("Value didn't found");
			return _tree->find(processGetValueFromStr(key));
		}
		catch (const std::invalid_argument& exp) {
			error.append("Incorrect value");
			return false;
		}
	};

	bool removeFromTree(std::string key, std::string& error) {
		try {
			error.append("Value didn't found");
			return _tree->remove(processGetValueFromStr(key));
		}
		catch (const std::invalid_argument& exp) {
			error.append("Incorrect value");
			return false;
		}
	};
	
	std::string printTree() {
		std::stringstream stream;
		stream << *_tree;
		return stream.str();
	};

	virtual ~BaseTemplateControllerCore() override{
		delete _tree;
	};

};
