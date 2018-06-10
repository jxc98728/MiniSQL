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


/* Block类用于buffermanager中缓存的写入与读取,大小为4K bytes */
class Block {
public:
	char tableName[TABLE_NAME_LENGTH]; //block对应的表名称
	int size; //当前block已用空间的大小
	bool isDirty; //isDirty = 1表示有待写回文件
	//bool isDelete; //isDelete = 1表示块已经被删除
	int offset; //当前block在文件中的偏移量
	int nextOffset; //下一块block在文件中的偏移量
	char content[BLOCK_SIZE]; //block中的数据 char[4096]
	Block(string table);
	Block(string table, int loc);
	int setContent(uint8_t* data, int length); //在当前content基础上增加数据,返回0代表当前块已满
	void setDirty(); //isDirty = 1
	char* getContent();//读取块内数据
	~Block();//在写回文件后析构Block
};

//records信息
class row {
public:
	vector<string> columns;//一行record由各列上的值组成
};
class records { //查询返回的records是row的集合
public:
	vector<row> rows;
};


