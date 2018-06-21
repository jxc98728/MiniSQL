#include "CatalogManager.h"
#include <fstream>
using namespace std;

CatalogManager::CatalogManager()
{
	readTables();
	readIndexes();
}

void CatalogManager::readTables()
{
	string fileName = "Tables.log";
	fstream file(fileName);
	file >> tableNum;
	//读入Tables.dat中的信息到vector<Table>中
	Table table;
	Attribute attribute;
	int type;
	for (int i = 0; i < tableNum; i++) {
		file >> table.name;
		file >> table.attriNum;
		file >> table.fileTail;
		file >> table.blockNum;
		//读入表中记录的attributes及信息
		for (int j = 0; j < table.attriNum; j++) {
			file >> attribute.name;
			file >> attribute.type;
			file >> attribute.charlength;
			file >> attribute.PK;
			file >> attribute.unique;
			table.attributes.push_back(attribute);
		}
		tables.push_back(table);
	}
	file.close();
}

void CatalogManager::readIndexes()
{
	string fileName = "Indexes.log";
	fstream file(fileName);
	file >> indexNum;
	//读入.index中的信息到vector<Index>中
	Index index;
	for (int i = 0; i < indexNum; i++)
	{
		file >> index.indexName;
		file >> index.tableName;
		file >> index.columnIndex;
		file >> index.attrLength;
		file >> index.blockNum;
		indexes.push_back(index);
	}
	file.close();
}

void CatalogManager::writeTables()
{
	string fileName = "Tables.log";
	fstream  file(fileName);
	file << tableNum << endl;
	//在.table中写入当前Manager中的表信息
	//(如果表信息更新了相当于重新写一遍整个文件)
	for (int i = 0; i < tableNum; i++)
	{
		file << tables[i].name << " ";
		file << tables[i].attriNum << " ";
		file << tables[i].fileTail << " ";
		file << tables[i].blockNum << endl;
		//写入单个表的所有属性信息
		//一个attribute一行
		for (int j = 0; j < tables[i].attriNum; j++)
		{
			file << tables[i].attributes[j].name << " ";
			file << tables[i].attributes[j].type << " ";
			file << tables[i].attributes[j].charlength << " ";
			file << tables[i].attributes[j].PK << " ";
			file << tables[i].attributes[j].unique << endl;
		}
	}
	file.close();
}


void CatalogManager::writeIndexes()
{
	string fileName = "Indexes.log";
	fstream file(fileName);
	file << indexNum << endl;
	//在.index中写入当前Manager中的索引信息
	for (int i = 0; i < indexNum; i++)
	{
		file << indexes[i].indexName << " ";
		file << indexes[i].tableName << " ";
		file << indexes[i].columnIndex << " ";
		file << indexes[i].attrLength << " ";
		file << indexes[i].blockNum << endl;
	}
	file.close();
}

void CatalogManager::createTable(const Table & table)
{
	tableNum++;
	tables.push_back(table);
}

void CatalogManager::createIndex(Index index)
{
	indexNum++;
	indexes.push_back(index);
}

//在上层调用时先通过exist判断指令是否合法
//只有在确认有这个table信息的时候才会调用dropTabel
void CatalogManager::dropTable(string tableName)
{
	//erase the table from tables
	for (int i = 0; i < tableNum; i++) {
		if (tables[i].name == tableName) {
			tables.erase(tables.begin()+ i);
			tableNum--;
		}
	}
	//earse all the indexes of the table
	for (int i = 0; i < indexNum; i++) {
		if (indexes[i].tableName == tableName) {
			indexes.erase(indexes.begin() + i);
			indexNum--;
		}
	}
}

//在上层调用时先通过exist判断指令是否合法
//只有在确认有这个index信息的时候才会调用dropIndex
void CatalogManager::dropIndex(string indexName)
{
	for (int i = 0; i < indexNum; i++) {
		if (indexes[i].indexName == indexName) {
			indexes.erase(indexes.begin()+i);
			indexNum--;
		}
	}
}

bool CatalogManager::tableExist(string tableName)
{
	for (int i = 0; i < tableNum; i++) {
		if (tables[i].name == tableName) {
			return true;
		}
	}
	return false;
}

bool CatalogManager::indexExist(string indexName)
{
	for (int i = 0; i < indexNum; i++) {
		if (indexes[i].indexName == indexName) {
			return true;
		}
	}
	return false;
}

Table CatalogManager::getTable(string tableName)
{
	Table result;
	for (int i = 0; i < tableNum; i++) {
		if (tables[i].name == tableName) {
			result = tables[i];
			break;
		}
	}
	return result;
}

Index CatalogManager::getIndex(string indexName)
{
	Index result;
	for (int i = 0; i < indexNum; i++) {
		if (indexes[i].indexName == indexName) {
			result = indexes[i];
			break;
		}
	}
	return result;
}
//列出所有表信息：用于调试
void CatalogManager::listTables()
{
	cout << "All the tables are listed as: " << endl;
	for (int i = 0; i < tableNum; i++) {
		cout << "Table " << i << ":" << tables[i].name << endl;
		cout << tables[i].blockNum << " blocks" << endl;
		cout << tables[i].attriNum << " attributes" << endl;
		for (int j = 0; j < tables[i].attriNum; j++) {
			cout << tables[i].attributes[j].name << endl;
		}
	}
}
//列出所有index信息：用于调试
void CatalogManager::listIndexes()
{
	cout << "All the indexes are listed as:" << endl;
	for (int i = 0; i < indexNum; i++) {
		cout << "Index " << i << ":" << indexes[i].indexName << endl;
	}
}

int CatalogManager::attriIndex(Table & table, string attriName)
{
	for (int i = 0; i < table.attriNum; i++) {
		if (table.attributes[i].name == attriName)
			return i;
	}
	return -1;//-1代表不存在该属性
}

//析构时把Manager信息写入log文件
CatalogManager::~CatalogManager()
{
	writeIndexes();
	writeTables();
}