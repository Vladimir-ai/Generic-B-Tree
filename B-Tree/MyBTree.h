#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <functional>
#include <ostream>
#include <algorithm>


template<typename T>
class MyBTree
{
private:
	
	template <typename T = T>
	struct BTreeNode
	{
		BTreeNode<T>* _parent;
		std::vector<T> _keys;
		std::vector<BTreeNode<T>*> _children;

		BTreeNode<T>(unsigned int order, BTreeNode<T>* parent = nullptr) {
			if (order < 3)
				throw std::invalid_argument("Order must be greater or equal than 2");

			_parent = parent;
			_keys.reserve(order);
			_children.reserve(order);
		};

		BTreeNode<T>* find(T key) {
			//assume that lists are if correct order;
			
			if (std::find(_keys.begin(), _keys.end(), key) != _keys.end())
				return this;

			int idx = findMinIndGreaterThanKey(key);

			if (_children.size() && idx <= _keys.size())
				return _children[idx]->find(key);

			return nullptr;
		};

		int findMinIndGreaterThanKey(T key)
		{
			auto res = upper_bound(_keys.begin(), _keys.end(), key, MyBTree<T>::_comparisonFunc);
			int ret = res - _keys.begin(); //for debug
			return ret;
		};

		int findChild(BTreeNode<T>* child) 
		{
			int res = std::find(_children.begin(), _children.end(), child) - _children.begin();
			if (res == _children.size())
				return -1;
			return res;
		};

		void clearChildList()
		{
			_children.clear();
		};

		T getSuccesor()
		{
			if (_children.size())
				return _children[0]->getSuccesor();

			return _keys[0];
		};

		T getPredecessor()
		{
			if (_children.size())
				return _children.back();

			return _keys.back();
		};

		void insert(T value)
		{
			auto index = findMinIndGreaterThanKey(value);
			_keys.insert(index + _keys.begin(), value);
		};

		~BTreeNode() {
			for (auto chld : _children) {
				delete chld;
			}
			_children.clear();

			_keys.clear();
		};
		
	};

	const static inline std::function<bool(const T&, const T&)> _comparisonFunc =
		[](const T& left, const T& right) {return left < right; };

	unsigned int _order;
	unsigned int _min;
	unsigned int _size;
	BTreeNode<T> *_rootNode;

	void splitChild(BTreeNode<T>* childToSplit, BTreeNode<T>* parent = nullptr);
	
	bool removeFromInner(BTreeNode<T>* node, T keyToRemove);
	bool removeFromLeaf(BTreeNode<T>* node, T keyToRemove);
	
	void fixChildSize(BTreeNode<T>* childToFix, BTreeNode<T>* parent = nullptr);
	bool mergeChilden(BTreeNode<T>* parent, unsigned int leftChildIdx);
	void mergeLeaf(BTreeNode<T>* parent, unsigned int leftChildIdx);

	bool isCorrectKeysAmount(BTreeNode<T>* nodeToCheck);

	bool removeFromPredecessorNode(BTreeNode<T>* nodeToInsert, unsigned int pos, BTreeNode<T>* const node);
	bool removeFromSuccesorNode(BTreeNode<T>* nodeToInsert, unsigned int pos, BTreeNode<T>* const node);

	bool leftRotate(BTreeNode<T>* parent, BTreeNode<T>* leftNode, BTreeNode<T>* rightNode);
	bool rightRotate(BTreeNode<T>* parent, BTreeNode<T>* leftNode, BTreeNode<T>* rightNode);

	void printTree(std::ostream& strm, BTreeNode<T>* node, int nChild = 0, std::string prevString = "");
	std::string printNode(std::ostream& strm, BTreeNode<T>* node, int nChild, std::string prevString);

	friend std::ostream& operator<<(std::ostream& strm, MyBTree<T>& tree) {
		tree.printTree(strm, tree._rootNode);
		return strm;
	}
	
	
public:
	MyBTree(unsigned int order);
	~MyBTree();

	void add(std::vector<T> elements);
	void add(T elem);
	
	bool remove(T elem);
	bool find(T elem);
	
	int size();

	
};



