#pragma once
/*
BPTree����IndexManager���������ݽṹ�������ڴ��У�
�رճ�����ٴ���������index��information�ؽ�BPTree
*/
#include <iostream>
#include <vector>
#include <string>
#include "MiniSQL.h"

using namespace std;

//Internal��Leaf����һ�������isLeaf����
/* TreeNode:base class of nodes of BPlusTree */
template<typename T>
class TreeNode {
public:
	int size;//size <= 4K
	int degree;
	//int type_bytes;//��ǰ�������͵Ĵ�С
	bool isLeaf; //type of a Node(internal or leaf)
	vector<T> keys;//����ֵ
	vector<int> offsets;//offset of the record in its file
	TreeNode* parent;
	TreeNode* sibling;
	vector<TreeNode<T> *> children;//ָ���ӽ�����ĿΪkeys+1
	bool isRoot;
public:
	TreeNode() = default;
	TreeNode(int d, bool leaf);
	int insert(T& key, int offset);
	TreeNode* split(T& key);//insert������splitһ��Node
	void remove(int index);//remove node[index]
	bool search(const T& key, int index);
	~TreeNode() {};
};




