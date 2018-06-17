/*
	CatalogManager����������ݿ����е�ģʽ��Ϣ��������
	����Ϣ��������Ϣ��������Ϣ�����ṩ���ʺͲ�����Щ��Ϣ�Ľӿڣ�
	��Interpreter��APIģ��ʹ��
	�ļ��涨��"Tables.dat"�洢����Ϣ��"Indexes.dat"�洢������Ϣ
	TODO:�����µ����г� metadata
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
	CatalogManager();//��ȡlog�ļ�
	void readTables(); //�����е�log�ļ��дӹ���tables
	void readIndexes();
	void writeTables();
	void writeIndexes();
	void createTable(const Table& table); //���create��drop��log����
	void createIndex(Index index);
	void dropTable(string tableName);
	void dropIndex(string indexName);
	bool tableExist(string tableName);//�����Ƿ�Ϸ���exist�����ж�
	bool indexExist(string indexName);
	Table getTable(string tableName); //��������Table����Ϣ
	Index getIndex(string indexName);
	void listTables();
	void listIndexes();
	int attriIndex(Table& table, string attriName);//���������ڱ��е���ţ�-1��������
	~CatalogManager();//����ʱ��log�ļ��е���Ϣ����
};
