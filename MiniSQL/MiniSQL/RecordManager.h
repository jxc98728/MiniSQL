#pragma once
/*
	recordmanager负责管理记录表中的数据文件，主要功能为实现数据文件的创建与删除、
	记录的插入、删除和查找操作。其中记录的查找操作要求能支持不带条件的查找和带一
	个条件的查找（包括等值查找、不等值查找和区间查找）

	add:可以将buffermanager和indexmanager设置为全局，在recordmanager中用extern调用
	查找后indexmanager改写全局的set容器中的返回结果

*/
#include <iostream>
#include <string>
#include <set>
#include <algorithm>
#include <vector>
#include <fstream>
#include <iterator>
#include "minisql.h"
#include "indexmanager.h"
#include "buffermanager.h"

//set<int> SearchResult; 实际定义在Minisql.h中，为IndexManager查询返回的全局容器变量
using namespace std;

class RecordManager
{
public:
	BufferManager *bufferm;
	IndexManager *indexm;
public:
	Row byte2row(Table table, char* src); //将以字节表示的数据转换为row（string）的形式
	char* row2byte(Table table, Row row); //将以字符串表示的数据（一行row）转换为char数组的形式
	bool Comparator(Table table, Row row, vector<Condition> conditions); //在遍历文件的数据时判断是否满足conditions
public:
	RecordManager() = delete;
	RecordManager(BufferManager *b, IndexManager* i)
		:bufferm(b), indexm(i)
	{};
	//create和drop仅管理.dat文件的建立与删除,非常simple
	void createTable(string tablename);
	void dropTable(string tablename);

	//insert/delete:api调用的接口，插入/删除数据到buffer中
	//（务必注意是写到buffer中的块！）
	void insertRecord(Table& table, Row& row); //done
	void deleterecord(const Table& table, const vector<Condition> cond);
	Records& selectRecord(const Table& table, const vector<Condition> cond);
	~RecordManager() = default;
};

