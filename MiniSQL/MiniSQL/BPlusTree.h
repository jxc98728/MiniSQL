#pragma once
#include <vector>

using namespace std;

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