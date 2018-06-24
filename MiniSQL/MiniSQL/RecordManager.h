#pragma once
/*
	recordmanager��������¼���е������ļ�����Ҫ����Ϊʵ�������ļ��Ĵ�����ɾ����
	��¼�Ĳ��롢ɾ���Ͳ��Ҳ��������м�¼�Ĳ��Ҳ���Ҫ����֧�ֲ��������Ĳ��Һʹ�һ
	�������Ĳ��ң�������ֵ���ҡ�����ֵ���Һ�������ң�

	add:���Խ�buffermanager��indexmanager����Ϊȫ�֣���recordmanager����extern����
	���Һ�indexmanager��дȫ�ֵ�set�����еķ��ؽ��

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

//set<int> SearchResult; ʵ�ʶ�����Minisql.h�У�ΪIndexManager��ѯ���ص�ȫ����������
using namespace std;

class RecordManager
{
public:
	BufferManager *bufferm;
	IndexManager *indexm;
public:
	Row byte2row(Table table, char* src); //�����ֽڱ�ʾ������ת��Ϊrow��string������ʽ
	char* row2byte(Table table, Row row); //�����ַ�����ʾ�����ݣ�һ��row��ת��Ϊchar�������ʽ
	bool Comparator(Table table, Row row, vector<Condition> conditions); //�ڱ����ļ�������ʱ�ж��Ƿ�����conditions
public:
	RecordManager() = delete;
	RecordManager(BufferManager *b, IndexManager* i)
		:bufferm(b), indexm(i)
	{};
	//create��drop������.dat�ļ��Ľ�����ɾ��,�ǳ�simple
	void createTable(string tablename);
	void dropTable(string tablename);

	//insert/delete:api���õĽӿڣ�����/ɾ�����ݵ�buffer��
	//�����ע����д��buffer�еĿ飡��
	void insertRecord(Table& table, Row& row); //done
	void deleterecord(const Table& table, const vector<Condition> cond);
	Records& selectRecord(const Table& table, const vector<Condition> cond);
	~RecordManager() = default;
};

