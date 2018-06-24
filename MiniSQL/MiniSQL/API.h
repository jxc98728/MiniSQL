#pragma once
#include <string>
#include "MiniSQL.h"
#include "IndexManager.h"
#include "CatalogManager.h"
#include "RecordManager.h"
/*
	APIģ��������ϵͳ�ĺ��ģ�����Ҫ����Ϊ�ṩִ��SQL���Ľӿڣ���Interpreter�����
	�ýӿ���Interpreter��������ɵ������ڲ���ʾΪ���룬����Catalog Manager�ṩ����Ϣȷ��ִ�й���
	������Record Manager��Index Manager��Catalog Manager�ṩ����Ӧ�ӿڽ���ִ�У���󷵻�ִ�н����Interpreterģ�顣

	API�����Ѿ����������static�ı���api���ϲ����ֱ��API::function(...)����
	����Ҫ����manager�����жϵĴ�����Ϣ�ķ��ؽ���API�����������﷨�ϾͿ����жϵĴ�����Ϣ��Interpreter����
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