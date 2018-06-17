#pragma once
/*
	注意在程序中尽量用string操作，对文件中的数据用二进制进行读取和写入
	Minisql.h定义了其他Manager中需要用到的基本类，当前.h为初稿，按需改动
*/
#include <iostream>
#include <vector>
#include <string>
using namespace std;

#define INT 0;
#define FLOAT 1;
#define CHAR 2;
#define NULL 3;
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
	//method .size() 返回这一属性的element所占空间
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
	Table() = default;
	Table(string n, int attrNum, std::vector<Attribute> attrs)
		: name(n), attriNum(attrNum), attributes(attrs), recNum(0), blockNum(0)
	{
		int length = 0;
		for (auto elem : attrs)
			length += elem.size();
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
	Index() = default;
	Index(string tbName, string idName, int column)
		:tableName(tbName), indexName(idName), columnIndex(column)
	{ /*attrLength and blockNum assigned from the data*/ }
};


/* Block类用于buffermanager中缓存的写入与读取,大小为4K bytes */
class Block {
public:
	string tableName; //block对应的表名称
	int size; //当前block已用空间的大小
	bool isDirty; //isDirty = 1表示有待写回文件
	//bool isDelete; //isDelete = 1表示块已经被删除
	int offset; //当前block在文件中的偏移量
	int nextOffset; //下一块block在文件中的偏移量
	char content[4096]; //block中的数据 char[4096]
	Block() = default;
	Block(string table);
	Block(string table, int loc);
	int setContent(char* data, int length); //在当前content基础上增加数据,返回0代表当前块已满
	void setDirty(); //isDirty = 1
	char* getContent();//读取块内数据
	~Block() = default;//在写回文件后析构Block
};

//records信息
class Row {
public:
	vector<string> columns;//一行record由各列上的值组成
};
class Records { //查询返回的records是row的集合
public:
	vector<Row> rows;
};

