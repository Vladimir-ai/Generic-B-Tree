#pragma once
#define DEFAULT_ORDER 3

#include "ControllerCoreForIntTree.h"

class Controller
{
public:
	
	std::string processInput(std::string input);
	std::string printHelp();


private:
	ControllerCore* _core = new ControllerCoreForIntTree(DEFAULT_ORDER);
};

