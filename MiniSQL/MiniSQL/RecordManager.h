#pragma once
/*
	RecordManager负责管理记录表中的数据文件，主要功能为实现数据文件的创建与删除、
	记录的插入、删除和查找操作。其中记录的查找操作要求能支持不带条件的查找和带一
	个条件的查找（包括等值查找、不等值查找和区间查找）
	TODO: 在minisql.h中定义condition类
*/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "MiniSQL.h"
#include "BufferManager.h"
#include "IndexManager.h"s
using namespace std;

class RecordManager
{
public:
	BufferManager *bufferM;
	IndexManager *indexM;
public:
	RecordManager() = default;
	RecordManager(BufferManager *b, IndexManager* i)
		:bufferM(b), indexM(i)
	{};
	//create和drop仅管理.dat文件的建立与删除,非常simple
	void createTable(string tableName);
	void dropTable(string tableName);

	//insert/delete:API调用的接口，插入/删除数据到buffer中
	//delete和select是RecordManager中的难点
	void insertRecord(const Table& table, Row& row);
	//void deleteRecord(const Table& table, const vector<Condition> cond);
	//void selectRecord(const Table& table, );
	~RecordManager() = default;
};

