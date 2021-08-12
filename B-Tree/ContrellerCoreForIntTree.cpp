#include "ControllerCoreForIntTree.h"
#include <boost/algorithm/string/trim.hpp>

bool ControllerCoreForIntTree::randomiseTree(int size)
{
	for (int i = 0; i < size; i++) {
		_tree->add(rand() % 100);
	}

	return true;
}

int ControllerCoreForIntTree::processGetValueFromStr(std::string str)
{	
	boost::trim(str);
	return stoi(str, NULL, 0);
}
