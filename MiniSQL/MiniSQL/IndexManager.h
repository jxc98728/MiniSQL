#pragma once
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <iomanip>
#include "BPlusTree.h"

using namespace std;


class TreeInfo {

public:
	string tableName;
	string indexName;

	/*
	list[id] -> B+ Tree
	*/
	int		id;

	TreeInfo(const string & tableName, const string & indexName, int type, int id)
		:tableName(tableName), indexName(indexName), id(id) {}
};

class IndexManager {
public:

	//BufferManager * bufferm;
	//CatalogManager * catalogm;

	// ������B+��
	map<string, int>	intTreeList;
	map<string, int>	fltTreeList;
	map<string, int>	strTreeList;

	vector<BPlusTree<int>>		int_treelist;
	vector<BPlusTree<float>>	flt_treelist;
	vector<BPlusTree<string>>	str_treelist;

	set<int> SearchResult;

	// Constructor
	IndexManager();

	// Destroyer
	~IndexManager();

	// 
	void writeBackToDisk(void);


	// ��tableName�ı��м���һ����indexNameΪkey��index, ��ʧ��return false.
	bool createIndex(const string & tableName, const string & indexName, int type);

	// ��tableName�ı���ɾ����indexNameΪkey��index, ��ʧ�ܻ�index��������return false.
	bool dropIndex(const string tableName, const string & indexName, int type);

	// �ڽ���insert����ʱ�Զ�Ӧ��B+�����в�����£�Ϊ�˷��㣬keyֵͳһ��string���ݣ�����catalog�е�type������Ӧ��ת����offset�ǰ�����key��record���ļ��е�ƫ������
	bool insert(const string tableName, const string & indexName, const string & key, int type, int offset);

	// �ڽ���delete����ʱ�Զ�Ӧ��B+�����и��£�Ϊ�˷��㣬keyֵͳһ��string���ݣ�����catalog��type������Ӧ��ת��
	// ע�⣺��ɾ�����м�ֵΪkey���
	bool remove(const string tableName, const string & indexName, const string & key, int type);


	/* ��ֵ���ң�����ִ�� ��=�� �����
		1. ��tableName�ı���indexNameΪ����ֵ��B+���в��Ҽ�ֵΪkey��������, ����ֵΪ��ѯ����ĸ�����
		2. ��ѯ�����ƥ���ֵ����record���ļ��е�ƫ����������ȫ��set����SearchResult�У�������MiniSQL.cpp�У���
		   ִ��ʱ�������SearchResult���飨�����������ѯ������Ҫ�������ڲ���ǰ������ա�
		3. type��key����������
			#0: int
			#-1: float
			#n: char(n)
	*/
	int selectEqual(const string tableName, const string & indexName, const string & key, int type);

	/* ����ֵ���ң�����ִ�� ��<>�� �����
		1. ��tableName�ı���indexNameΪ����ֵ��B+���в��Ҽ�ֵ��Ϊkey��������, ����ֵΪ��ѯ����ĸ�����
		2. ��ѯ�����ƥ���ֵ����record���ļ��е�ƫ����������ȫ��set����SearchResult�У�������MiniSQL.cpp�У�
		   ִ��ʱ�������SearchResult���飨�����������ѯ������Ҫ�������ڲ���ǰ������ա�
		3. type��key����������
			#0: int
			#-1: float
			#n: char(n)
	*/
	int selectInequal(const string tableName, const string & indexName, const string & key, int type);

	/* ��������(Range Query)������ִ�� ��>, < <=, >=�� �����
		1. ��tableName�ı���indexNameΪ����ֵ��B+���в��ҷ��ϼ�ֵ����������ϵ��������, ���硰 key >= 1234 ����
		   ����ֵΪ��ѯ����ĸ�����
		2. ��ѯ�����ƥ���ֵ����record���ļ��е�ƫ����������ȫ��set����SearchResult�У�������MiniSQL.cpp�У�
		   ִ��ʱ�������SearchResult���飨�����������ѯ������Ҫ�������ڲ���ǰ������ա�
		3. type��key���������ͣ�
			#0: int
			#-1: float
			#n: char(n)
		4. opcode�ǱȽϷ����ͣ�
			#0: >
			#1: <
			#2: >=
			#3. <=
			#4: =
			#5: <>
		5. cond������ֵ
	*/
	int selectRange(const string tableName, const string & indexName, const string & cond, int opcode, int type);

};
