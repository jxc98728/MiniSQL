#pragma once
#include <vector>
using namespace std;

/* TreeNode:base class of nodes of BPlusTree */
template<typename T>
class TreeNode {
public:
	int size;
	int degree;
	int type_bytes;
	
	vector<T> keys;
	vector<TreeNode*> ptrs;
	
	TreeNode(); // calculate degree
	~TreeNode();
};