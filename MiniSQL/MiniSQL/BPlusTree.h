#pragma once
#include <vector>

using namespace std;

template<typename KeyType>
class TreeNode {
public:
	int size;
	int degree;
	
	vector<KeyType> keys;
	vector<TreeNode*> ptrs;

	void insert();
	void findnode();
	
	TreeNode() {}
	~TreeNode() {}
};
