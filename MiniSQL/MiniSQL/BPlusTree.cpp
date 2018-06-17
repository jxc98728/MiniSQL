#include "BPlusTree.h"
using namespace std;

template<typename T>
inline TreeNode<T>::TreeNode(int d, bool leaf)
	:degree(d),isLeaf(leaf)
{

}

template<typename T>
int TreeNode<T>::insert(T & key, int offset)
{
	return 0;
}

template<typename T>
TreeNode<T> * TreeNode<T>::split(T & key)
{
	return nullptr;
}

template<typename T>
void TreeNode<T>::remove(int index)
{
}

template<typename T>
bool TreeNode<T>::search(const T & key, int index)
{
	return false;
}

