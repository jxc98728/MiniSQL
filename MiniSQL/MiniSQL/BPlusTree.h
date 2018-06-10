#pragma once
#include "MiniSQL.h"
#include <vector>
using namespace std;

/* TreeNode:base class of nodes of BPlusTree */
template<typename T>
class TreeNode {
public:
	int size;
	int degree;
	int type_bytes;//当前数据类型的大小
	bool isLeaf; //type of a Node(internal or leaf)
	vector<T> keys;
	vector<TreeNode*> ptrs;
	
	TreeNode(); // calculate degree
	~TreeNode();
};