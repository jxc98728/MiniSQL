#include <iostream>
#include "BPlusTree.h"

using namespace std;

int main(int argc, char* argv[])
{
	TreeNode<char[8]> list;

	char t[8] = "1234567";

	list.keys.push_back(t);

	cout << list.keys[0] << endl;

}