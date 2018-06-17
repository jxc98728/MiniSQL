#include "RecordManager.h"

void RecordManager::createTable(string tableName)
{
	string fileName = tableName + ".dat";
	fstream file(fileName);
	file.close();
	return;
}

void RecordManager::dropTable(string tableName)
{
	string fileName = tableName + ".dat";
	remove(fileName.c_str());
	return ;
}

void RecordManager::insertRecord(const Table & table, Row & row)
{

}
