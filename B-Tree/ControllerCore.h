#pragma once
#include <string>

class ControllerCore {
public:
	virtual bool addToTree(std::string key) = 0;
	virtual bool findInTree(std::string key, std::string& error) = 0;
	virtual bool removeFromTree(std::string key, std::string& error) = 0;
	virtual bool randomiseTree(int size) = 0;
	virtual std::string printTree() = 0;
	virtual ~ControllerCore() {};
};