#include "MiniSQL.h"
#include "CatalogManager.h"
#include <string>
#include <iostream>
using namespace std;

int main() {
	Attribute a1("bno",2, 10, true, true);
	Attribute a2("name", 2, 20, false, true);
	vector<Attribute> attrs = { a1, a2 };

	Table table("book",2,attrs);
	table.show();
	
	CatalogManager log;
	log.createTable(table);
	log.writeTables();
	log.listTables();

	system("pause");
	return 0;

}