#pragma once
/*
	RecordManager��������¼���е������ļ�����Ҫ����Ϊʵ�������ļ��Ĵ�����ɾ����
	��¼�Ĳ��롢ɾ���Ͳ��Ҳ��������м�¼�Ĳ��Ҳ���Ҫ����֧�ֲ��������Ĳ��Һʹ�һ
	�������Ĳ��ң�������ֵ���ҡ�����ֵ���Һ�������ң�
	TODO: ��minisql.h�ж���condition��
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
	//create��drop������.dat�ļ��Ľ�����ɾ��,�ǳ�simple
	void createTable(string tableName);
	void dropTable(string tableName);

	//insert/delete:API���õĽӿڣ�����/ɾ�����ݵ�buffer��
	//delete��select��RecordManager�е��ѵ�
	void insertRecord(const Table& table, Row& row);
	//void deleteRecord(const Table& table, const vector<Condition> cond);
	//void selectRecord(const Table& table, );
	~RecordManager() = default;
};

