#pragma once
/*
BPTree用作IndexManager的索引数据结构，存在内存中，
关闭程序后再次启动根据index的information重建BPTree
*/
#include <iostream>
#include <vector>
#include <string>
#include "MiniSQL.h"

using namespace std;

//Internal和Leaf放在一个类里，用isLeaf区分
/* TreeNode:base class of nodes of BPlusTree */
template<typename T>
class TreeNode {
public:
	int size;//size <= 4K
	int degree;
	//int type_bytes;//当前数据类型的大小
	bool isLeaf; //type of a Node(internal or leaf)
	vector<T> keys;//索引值
	vector<int> offsets;//offset of the record in its file
	TreeNode* parent;
	TreeNode* sibling;
	vector<TreeNode<T> *> children;//指向孩子结点的数目为keys+1
	bool isRoot;
public:
	TreeNode() = default;
	TreeNode(int d, bool leaf);
	int insert(T& key, int offset);
	TreeNode* split(T& key);//insert后过多而split一个Node
	void remove(int index);//remove node[index]
	bool search(const T& key, int index);
	~TreeNode() {};
};




