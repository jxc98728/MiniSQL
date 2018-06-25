#include "MiniSQL.h"
#include "BufferManager.h"
#include <iostream>

using namespace std;

//int main() {
//	BufferManager a;
//	Attribute a1("sid", 10, 1, 1);
//	Attribute a2("name", 8, 0, 0);
//	Attribute a3("age", 0, 0, 0);
//	vector<Attribute> as = { a1, a2,a3 };
//	Table t1("student", 3, as);
//
//	char s[10] = { 'a','b','c','d','e' };
//	Block block("student", 0);
//	block.isDirty = true;
//	block.insertRecord(s,5);
//	//a.writeBlock(block);
//
//	Block block2("student", 1);
//	block2.isDirty = true;
//	block2.insertRecord(s, 5);
//	//a.writeBlock(block2);
//	a.block2buf(block);
//	a.block2buf(block2);
//	a.allWrite();
//
//
//	//system("pause");
//	return 0;
//}
