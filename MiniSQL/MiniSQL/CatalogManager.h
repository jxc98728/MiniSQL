/*
	CatalogManager负责管理数据库所有的模式信息，包括：
	表信息、属性信息、索引信息，并提供访问和操作这些信息的接口，
	供Interpreter和API模块使用
	文件规定："Tables.dat"存储表信息，"Indexes.dat"存储索引信息
	TODO:考虑下单独列出 metadata
*/
#pragma once
#include "MiniSQL.h"
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;

class CatalogManager
{
public:
	vector<Table> tables;
	vector<Index> indexes;
	int tableNum;
	int indexNum;
public:
	CatalogManager();//读取log文件
	void readTables(); //从已有的log文件中从构造tables
	void readIndexes();
	void writeTables();
	void writeIndexes();
	void createTable(const Table& table); //语句create和drop的log管理
	void createIndex(Index index);
	void dropTable(string tableName);
	void dropIndex(string indexName);
	bool tableExist(string tableName);//操作是否合法用exist函数判断
	bool indexExist(string indexName);
	Table getTable(string tableName); //返回整个Table的信息
	Index getIndex(string indexName);
	void listTables();
	void listIndexes();
	int attriIndex(Table& table, string attriName);//返回属性在表中的序号，-1代表不存在
	~CatalogManager();//析构时将log文件中的信息更新
};
