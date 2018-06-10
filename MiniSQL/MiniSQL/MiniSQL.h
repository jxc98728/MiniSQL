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
#define TABLE_NAME_LENGTH 32 
#define BLOCK_HEAD 64

/* Attribute consisting the info of type/length/primary key/unique */
class Attribute {
public:
	string name;
	DataType type;
	int length;
	bool PK; //primary key
	bool unique;
	Attribute(string n, DataType t, int l, bool p, bool u)
		:name(n), type(t), length(l), PK(p), unique(u)
	{ };
};

/* Table class in CatalogManager, only table info without records */
class Table {
public:
	string dbName; //name of its database
	string name; //name of the table
	int attriNum; //number of the attributes
	vector<Attribute> attributes; //list of the attributes
	int recNum;
	int recLength; //length of a record
	int blockNum;
	Table(string db, string n, int attrNum, std::vector<Attribute> attrs)
		: dbName(db), name(n), attriNum(attrNum), attributes(attrs), recNum(0), blockNum(0)
	{
		int length = 0;
		for (auto elem : attrs)
			length += elem.length;
		recLength = length;
	}
};

/* index class in IndexManager, information of a index created */
class Index {
public:
	string tableName;
	string indexName;
	int columnIndex; //which column in the sequential attributes
	int attrLength; //attribute's length of the index
	int blockNum; //number of blocks in the B+Tree
	Index(string tbName, string idName, int column)
		:tableName(tbName), indexName(idName), columnIndex(column)
	{ /*attrLength and blockNum assigned from the data*/ }
};


/* Block������buffermanager�л����д�����ȡ,��СΪ4K bytes */
class Block {
public:
	char tableName[TABLE_NAME_LENGTH]; //block��Ӧ�ı�����
	int size; //��ǰblock���ÿռ�Ĵ�С
	bool isDirty; //isDirty = 1��ʾ�д�д���ļ�
	//bool isDelete; //isDelete = 1��ʾ���Ѿ���ɾ��
	int offset; //��ǰblock���ļ��е�ƫ����
	int nextOffset; //��һ��block���ļ��е�ƫ����
	char content[BLOCK_SIZE]; //block�е����� char[4096]
	Block(string table);
	Block(string table, int loc);
	int setContent(uint8_t* data, int length); //�ڵ�ǰcontent��������������,����0����ǰ������
	void setDirty(); //isDirty = 1
	char* getContent();//��ȡ��������
	~Block();//��д���ļ�������Block
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


