#pragma once
/*
	ע���ڳ����о�����string���������ļ��е������ö����ƽ��ж�ȡ��д��
	Minisql.h����������Manager����Ҫ�õ��Ļ�����
	
*/
#include <iostream>
#include <vector>
#include <string>
using namespace std;

#define INT 0;
#define FLOAT 1;
#define CHAR 2;
#define BLOCK_SIZE 4096 /* block of size 4K bytes */

/* Attribute consisting the info of type/length/primary key/unique */
class Attribute {
public:
	string name;
	int type;
	int charlength;
	bool PK; //primary key
	bool unique;
	Attribute() = default;
	Attribute(string n, int t, int l, bool p, bool u)
		:name(n), type(t), charlength(l), PK(p), unique(u)
	{ };
	//method .size() ������һ���Ե�element��ռ�ռ�
	inline int size() {
		switch (type) {
		case 0:
			return sizeof(int);
		case 1:
			return sizeof(float);
		case 2:
			return  charlength + 1;
		}
	}
	void show();
};

/* Table class in CatalogManager, only table info without records */
class Table {
public:
	string name; //name of the table
	int attriNum; //number of the attributes
	vector<Attribute> attributes; //list of the attributes
	int recNum;
	int recLength; //length of a record
	int blockNum;
	int fileTail; //�ļ�β��offset��ΪBlock��ȡ�Ĳο�
	Table() = default;
	Table(string n)
		: name(n)
	{ };
	Table(string n, int attrNum, std::vector<Attribute> attrs)
		: name(n), attriNum(attrNum), attributes(attrs), recNum(0), blockNum(0),fileTail(0)
	{
		int length = 0;
		for (auto elem : attrs)
			length += elem.size();
		recLength = length;
	}
	void show();
};

/* index class in IndexManager, information of a index created */
class Index {
public:
	string tableName;
	string indexName;
	int columnIndex; //which column in the sequential attributes
	int attrLength; //attribute's length of the index
	int blockNum; //number of blocks in the B+Tree
	Index() = default;
	Index(string tbName, string idName, int column)
		:tableName(tbName), indexName(idName), columnIndex(column)
	{ /*attrLength and blockNum assigned from the data*/ }
};


/* Block������buffermanager�л����д�����ȡ,��СΪ4K bytes
   4K byteΪ��ʵ��С��ʵ���ļ��ö������ļ���д������ȷ��λ��
   �ļ���ֻ����ʵ���ݣ�һ��Record�����ΪValid Byte + Record */

class Block {
public:
	string tableName; //block��Ӧ�ı���
	int blockid ; //��ǰblock�ڶ�Ӧ���ļ��еı��
	int size; //��ǰblock���ÿռ�Ĵ�С
	bool isDirty; //isDirty = 1��ʾ�д�д���ļ�
	char content[BLOCK_SIZE]; //block�е����� char[4096]

public:
	Block();
	Block(string table);
	Block(string table, int blockid);
	bool insertRecord(char* src, int length);
	void setDirty(); //isDirty = 1
	void show();
	char* getContent(int start, int bytes);//��ȡ��������
	~Block() = default;//��д���ļ�������Block
};

//records��Ϣ
class Row {
public:
	vector<string> columns;//һ��record�ɸ����ϵ�ֵ���
};
class Records { //��ѯ���ص�records��row�ļ���
public:
	vector<Row> rows;
};

