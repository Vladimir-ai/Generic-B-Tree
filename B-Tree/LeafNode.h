#pragma once
#include <map>
#include <memory>
#include "BTreeNode.h"

template<typename T>
class LeafNode : BTreeNode<T>
{
protected:
	std::map<T, std::shared_ptr<T>> _values{};
	
public:
	int size();

};

