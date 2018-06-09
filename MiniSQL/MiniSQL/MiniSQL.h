#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;
typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;
enum DataType {CHAR,INT,FLOAT};
#define BLOCK_SIZE 4096 /* block of size 4K bytes */

/* attribute consisting the info of type/length/primary key/unique */
class attribute {
public:
	string name;
	DataType type;
	int length;
	bool PK; //primary key
	bool unique;
	attribute(string n, DataType t, int l, bool p, bool u)
		:name(n), type(t), length(l), PK(p), unique(u)
	{ };
};

/* table class in CatalogManager, only table info without records */
class table {
public:
	string dbName; //name of its database
	string name; //name of the table
	int attriNum; //number of the attributes
	vector<attribute> attributes; //list of the attributes
	int recNum;
	int recLength; //length of a record
	int blockNum;
	table(string db, string n, int attrNum, std::vector<attribute> attrs)
		: dbName(db), name(n), attriNum(attrNum), attributes(attrs), recNum(0), blockNum(0)
	{
		int length = 0;
		for (auto elem : attrs)
			length += elem.length;
		recLength = length;
	}
};

/* index class in IndexManager, information of a index created */
class index {
public:
	string tableName;
	string indexName;
	int columnIndex; //which column in the sequential attributes
	int attrLength; //attribute's length of the index
	int blockNum; //number of blocks in the B+Tree
	index(string tbName, string idName, int column)
		:tableName(tbName), indexName(idName), columnIndex(column)
	{ /*attrLength and blockNum assigned from the data*/ }
};


/* block������buffermanager�л����д�����ȡ,��СΪ4K bytes */
class block {
public:
	char table[32]; //block��Ӧ�ı�����
	int size; //��ǰblock���ÿռ�Ĵ�С
	uint8_t content[BLOCK_SIZE]; //block�е�����
	bool isDirty; //isDirty = 1��ʾ�д�д���ļ�
	int offset; //��ǰblock���ļ��е�ƫ����
	int next; //��һ��block���ļ��е�ƫ����
	block();
	void setContent(uint8_t* data); //�ڵ�ǰcontent��������������
	uint8_t* getContent();
	~block();
};

//records��Ϣ
class row {
public:
	vector<string> columns;//һ��record�ɸ����ϵ�ֵ���
};
class records { //��ѯ���ص�records��row�ļ���
public:
	vector<row> rows;
};


