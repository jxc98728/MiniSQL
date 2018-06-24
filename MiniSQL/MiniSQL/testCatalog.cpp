#include "MiniSQL.h"
#include "CatalogManager.h"
#include <string>
#include <iostream>
using namespace std;
//
//int main() {
//	//测试属性创建
//	Attribute a1("bno",2, 10, true, true);
//	Attribute a2("name", 2, 20, false, true);
//	vector<Attribute> attrs = { a1, a2 };
//
//	//测试建表
//	Table table("book",2,attrs);
//	table.show();
//	
//	//测试log文件的读写
//	CatalogManager log;
//	log.createTable(table);
//	log.writeTables();
//	log.listTables();
//
//	system("pause");
//	return 0;
//
//}