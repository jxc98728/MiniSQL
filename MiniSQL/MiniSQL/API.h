#pragma once
#include <string>
#include "MiniSQL.h"
#include "IndexManager.h"
#include "CatalogManager.h"
#include "RecordManager.h"
/*
	API模块是整个系统的核心，其主要功能为提供执行SQL语句的接口，供Interpreter层调用
	该接口以Interpreter层解释生成的命令内部表示为输入，根据Catalog Manager提供的信息确定执行规则
	并调用Record Manager、Index Manager和Catalog Manager提供的相应接口进行执行，最后返回执行结果给Interpreter模块。

	API类中已经创建了类的static的变量api，上层调用直接API::function(...)即可
	！需要调用manager才能判断的错误信息的返回交给API负责，其他从语法上就可以判断的错误信息由Interpreter负责
*/

//SqlCommand.opName
enum { CreateTableCmd, DropTableCmd, CreateIndexCmd, DropIndexCmd, SelectCmd, InsertCmd, DeleteCmd };

class API
{
public:
	static API *api;
	RecordManager *rm;
	CatalogManager *cm;
	API();
	~API();

public:
	static bool Execute(SQLCommand sqlCommand);

	static bool CreateTable(Table table);

	static bool CreateIndex(Index index);

	static bool DropTable(string tableName);

	static bool DropIndex(string indexName);

	static bool Delete(string tableName, vector<Condition> condList);

	static bool Select(string tableName,vector<Condition> condList ,vector<string> attrList);

	static bool Insert(string tableName, vector<string> valuesList);

	bool Exit();
};