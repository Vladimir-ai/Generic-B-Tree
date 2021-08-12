#include "MyBTree.h"

using namespace std;

template<typename T>
MyBTree<T>::MyBTree(unsigned int order)
{
	if (order < 2)
		throw invalid_argument("Order must be greater than 2");

	_order = order;
	_min = ((order - 1) / 2);
	_size = 0;
	_rootNode = new BTreeNode<T>(order);
}


template<typename T>
void MyBTree<T>::add(std::vector<T> elements)
{
	_size += elements.size();
	_rootNode->insert(elements);
}


template<typename T>
void MyBTree<T>::add(T elem)
{
	_size++;

	BTreeNode<T>* currNode = _rootNode;

	while (currNode->_children.size() != 0) {
		int ind = currNode->findMinIndGreaterThanKey(elem);
		currNode = currNode->_children[ind];
	}

	currNode->insert(elem);

	if (currNode->_keys.size() == _order) {
		BTreeNode<T>* parent = currNode->_parent;

		if (parent) {
			splitChild(currNode, parent);
		}
		else { //split root
			BTreeNode<T>* newRoot = new BTreeNode<T>(_order);
			_rootNode->_parent = newRoot;

			newRoot->_children.push_back(_rootNode);
			splitChild(_rootNode, newRoot);

			_rootNode = newRoot;
		}
	}
}

template<typename T>
bool MyBTree<T>::isCorrectKeysAmount(BTreeNode<T>* nodeToCheck)
{
	return nodeToCheck->_keys.size() < _order && nodeToCheck->_keys.size() >= _min;
}


template<typename T>
void MyBTree<T>::splitChild(BTreeNode<T>* childToSplit, BTreeNode<T>* parent)
{
	if (parent && std::find(parent->_children.begin(), parent->_children.end(), childToSplit) == parent->_children.end())
		return; //except?

	//ChildToSplit is left node, new child - right
	BTreeNode<T>* newChild = new BTreeNode<T>(_order, parent);

	int anotherChildIndices = _order / 2;

	newChild->_keys = vector<T>(childToSplit->_keys.end() - anotherChildIndices,
		childToSplit->_keys.end());
	childToSplit->_keys.erase(childToSplit->_keys.end() - anotherChildIndices,
		childToSplit->_keys.end());

	T keyToParentNode = childToSplit->_keys.back();
	childToSplit->_keys.pop_back();

	if (childToSplit->_children.size()) {
		newChild->_children = vector<BTreeNode<T>*>(childToSplit->_children.end() - anotherChildIndices - 1,
			childToSplit->_children.end());
		childToSplit->_children.erase(childToSplit->_children.end() - anotherChildIndices - 1,
			childToSplit->_children.end());

		for (auto child : childToSplit->_children) {
			child->_parent = childToSplit;
		}

		for (auto child : newChild->_children) {
			child->_parent = newChild;
		}
	}

	int indexInCurrNode = parent->findMinIndGreaterThanKey(keyToParentNode);

	if (indexInCurrNode != parent->_keys.size()) {
		parent->_keys.insert(parent->_keys.begin() + indexInCurrNode, keyToParentNode);
		parent->_children.insert(parent->_children.begin() + indexInCurrNode + 1, newChild);
	}
	else
	{
		parent->_keys.push_back(keyToParentNode);
		parent->_children.push_back(newChild);
	}

	if (parent->_keys.size() == _order) {
		if (parent->_parent)
			splitChild(parent, parent->_parent);
		else {
			BTreeNode<T>* newRoot = new BTreeNode<T>(_order);
			_rootNode->_parent = newRoot;

			newRoot->_children.push_back(_rootNode);
			_rootNode = newRoot;
			splitChild(_rootNode->_children[0], newRoot);

			
		}
	}
}

template<typename T>
bool MyBTree<T>::removeFromInner(BTreeNode<T>* node, T key)
{
	// this method calls if and only if node contains key
	auto keyPosIter = std::find(node->_keys.begin(), node->_keys.end(), key);

	
	if (keyPosIter == node->_keys.end())
		return false;

	int keyPos = keyPosIter - node->_keys.begin();

	//if last inner node try to get key from leaf
	if (!node->_children[keyPos]->_children.size() && node->_children[keyPos]->_keys.size() > _min) {
		node->_keys[keyPos] = node->_children[keyPos]->_keys.back();
		node->_children[keyPos]->_keys.pop_back();

		return true;
	}

	if (!node->_children[keyPos + 1]->_children.size() && node->_children[keyPos + 1]->_keys.size() > _min) {
		node->_keys[keyPos] = node->_children[keyPos + 1]->_keys.front();
		node->_children[keyPos + 1]->_keys.erase(node->_children[keyPos + 1]->_keys.begin());

		return true;
	}	
	
	return removeFromPredecessorNode(node, keyPos, node->_children[keyPos]);

	//if can't merge childs get full subtree with its key
			

	//what to do if can't do this???


	return true;
		
}




template<typename T>
bool MyBTree<T>::mergeChilden(BTreeNode<T>* parent, unsigned int leftChildIdx)
{
	//assume that leftChildInd < parent->_children.size() - 1
	if (leftChildIdx == parent->_children.size() - 1)
		leftChildIdx -= 1;

	BTreeNode<T>* leftChld = parent->_children[leftChildIdx];
	BTreeNode<T>* rightChld = parent->_children[leftChildIdx + 1];

	//try to rotate subtree
	if (leftChld->_keys.size() < _min && rightChld->_keys.size() > _min)
		return leftRotate(parent, leftChld, rightChld);

	if (rightChld->_keys.size() < _min && leftChld->_keys.size() > _min)
		return rightRotate(parent, leftChld, rightChld);

	//if cant merge children (overflow)
	if (leftChld->_children.size() + rightChld->_children.size() + 2 == _order)
		return false;

	leftChld->_keys.push_back(parent->_keys[leftChildIdx]);
	
	parent->_keys.erase(parent->_keys.begin() + leftChildIdx);
	parent->_children.erase(parent->_children.begin() + leftChildIdx + 1);
	

	leftChld->_keys.insert(leftChld->_keys.end(), rightChld->_keys.begin(), rightChld->_keys.end());
	leftChld->_children.insert(leftChld->_children.end(), rightChld->_children.begin(), rightChld->_children.end());

	for (auto child : leftChld->_children)
		child->_parent = leftChld;

	rightChld->clearChildList();
	delete rightChld;

	if (parent->_parent && !isCorrectKeysAmount(parent)) {
		return mergeChilden(parent->_parent, parent->_parent->findChild(parent));
	}

	if (!parent->_parent && parent->_keys.size() == 0) {
		_rootNode = leftChld;
		leftChld->_parent->clearChildList();

		delete leftChld->_parent;
		
		_rootNode->_parent = nullptr;
		return true;
	}

	return true;
}


template<typename T>
bool MyBTree<T>::remove(T elem)
{
	BTreeNode<T>* currNode = _rootNode;

	while (currNode->_children.size() != 0 && std::find(currNode->_keys.begin(), currNode->_keys.end(), elem) == currNode->_keys.end())
		currNode = currNode->_children[currNode->findMinIndGreaterThanKey(elem)];

	auto posIter = std::find(currNode->_keys.begin(), currNode->_keys.end(), elem);


	if (currNode->_children.size() == 0 && posIter == currNode->_keys.end())
		return false;

	//leaf, not leaf
	if (currNode->_children.size() != 0) {
		return removeFromInner(currNode, elem);
	}


	if (currNode->_children.size() == 0) {
		return removeFromLeaf(currNode, elem);
	}

	return true;
}


template<typename T>
bool MyBTree<T>::find(T elem)
{
	return _rootNode->find(elem);
}


template<typename T>
bool MyBTree<T>::removeFromLeaf(BTreeNode<T>* node, T keyToRemove)
{

	if (node->_children.size() != 0)
		return false;

	auto posIter = std::find(node->_keys.begin(), node->_keys.end(), keyToRemove);

	if (posIter == node->_keys.end())
		return false;

	node->_keys.erase(posIter, posIter + 1);

	if (!node->_parent) //because this node is root
		return true;

	if (node->_keys.size() >= _min) {
		return true;
	}

	BTreeNode<T>* parentNode = node->_parent;

	int indInParentNode = parentNode->findChild(node); //our node is left
	
	if (indInParentNode == -1)
		return false; //except?

	// node is left check for right if it exists
	if (indInParentNode < parentNode->_children.size() - 1 && parentNode->_children[indInParentNode]->_keys.size() > _min) {
		BTreeNode<T>* rightNode = parentNode->_children[indInParentNode + 1];
		leftRotate(node->_parent, node, rightNode);

		return true;
	}

	// node is right check for left if it exists
	if (indInParentNode != 0 && parentNode->_children[indInParentNode - 1]->_keys.size() > _min) {
		BTreeNode<T>* leftNode = parentNode->_children[indInParentNode - 1];
		rightRotate(node->_parent, leftNode, node);

		return true;
	}

	//rotates

	//if (!node->_keys.size())
	//	delete node;
	//we need to mergeChildren
	if (indInParentNode == parentNode->_children.size() - 1)
		return mergeChilden(parentNode, indInParentNode - 1);
	else
		return mergeChilden(parentNode, indInParentNode);
}


template<typename T>
int MyBTree<T>::size()
{
	return _size;
}

template<typename T>
bool MyBTree<T>::leftRotate(BTreeNode<T>* parent, BTreeNode<T>* leftNode, BTreeNode<T>* rightNode)
{
	int leftNodeIdx = parent->findChild(leftNode);

	if (leftNodeIdx == -1)
		return false;

	if (rightNode->_keys.size() == _min)
		return false;//try to mergeParents?

	leftNode->_keys.push_back(parent->_keys[leftNodeIdx]);
	parent->_keys[leftNodeIdx] = rightNode->_keys.front();
	rightNode->_keys.erase(rightNode->_keys.begin());

	if (leftNode->_children.size() == 0 && rightNode->_children.size() == 0)
		return true;

	leftNode->_children.push_back(rightNode->_children.front());
	rightNode->_children.erase(rightNode->_children.begin());

	leftNode->_children.back()->_parent = leftNode;

	return true;
}

template<typename T>
MyBTree<T>::~MyBTree()
{
	delete _rootNode;
}

template<typename T>
bool MyBTree<T>::rightRotate(BTreeNode<T>* parent, BTreeNode<T>* leftNode, BTreeNode<T>* rightNode)
{
	int leftNodeIdx = parent->findChild(leftNode);

	if (leftNodeIdx == -1 || parent->_children.size() == leftNodeIdx + 1)
		return false;

	if (leftNode->_keys.size() == _min)
		return false;//try to mergeParents?

	rightNode->_keys.insert(rightNode->_keys.begin(), parent->_keys[leftNodeIdx]);
	parent->_keys[leftNodeIdx] = leftNode->_keys.back();
	leftNode->_keys.pop_back();

	if (leftNode->_children.size() == 0 && rightNode->_children.size() == 0)
		return true;

	rightNode->_children.insert(rightNode->_children.begin(), leftNode->_children.back());
	leftNode->_children.pop_back();

	rightNode->_children.front()->_parent = rightNode;

	return true;
}


template<typename T>
bool MyBTree<T>::removeFromPredecessorNode(BTreeNode<T>* nodeToInsert, unsigned int pos, BTreeNode<T>* const node)
{
	BTreeNode<T>* currNode = node;
	while (currNode->_children.size())
		currNode = currNode->_children.back();

	nodeToInsert->_keys[pos] = currNode->_keys.back();

	return removeFromLeaf(currNode, currNode->_keys.back());
}


template<typename T>
bool MyBTree<T>::removeFromSuccesorNode(BTreeNode<T>* nodeToInsert, unsigned int pos, BTreeNode<T>* const node)
{
	BTreeNode<T>* currNode = node;
	while (currNode->_children.size())
		currNode = currNode->_children[0];

	nodeToInsert->_keys[pos] = currNode->_keys[0];
	
	return removeFromLeaf(currNode, currNode->_keys[0]);
}


template<typename T>
void MyBTree<T>::printTree(ostream& strm, BTreeNode<T>* node, int nChild, string prevString)
{
	if (node) {
		string aux = prevString;
		prevString = printNode(strm, node, nChild, aux);
		for (int i = node->_children.size() - 1; i >= 0 ; i--) {
			printTree(strm, node->_children[i], i, prevString);
		}
	}
}


template<typename T>
string MyBTree<T>::printNode(ostream& strm, BTreeNode<T>* node, int nChild, string prevString)
{
	prevString += "     ";
	for (int j = 1; j <= nChild; ++j)  prevString += "|  ";
	strm << prevString;

	strm << string(" `---.");
	for (unsigned int i = 0; i < node->_keys.size(); i++) {
		strm << node->_keys[i] << ".";
	}
	strm << string("\n");

	return prevString;
}