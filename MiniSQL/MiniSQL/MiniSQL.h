#pragma once
/*
	注意在程序中尽量用string操作，对文件中的数据用二进制进行读取和写入
	Minisql.h定义了其他Manager中需要用到的基本类
	
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
	int fileTail; //文件尾的offset作为Block读取的参考
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


/* Block类用于buffermanager中缓存的写入与读取,大小为4K bytes
   4K byte为真实大小，实际文件用二进制文件读写，方便确定位置
   文件中只存真实数据，一条Record的组成为Valid Byte + Record */

class Block {
public:
	string tableName; //block对应的表名
	int blockid ; //当前block在对应的文件中的标号
	int size; //当前block已用空间的大小
	bool isDirty; //isDirty = 1表示有待写回文件
	char content[BLOCK_SIZE]; //block中的数据 char[4096]

public:
	Block();
	Block(string table);
	Block(string table, int blockid);
	bool insertRecord(char* src, int length);
	void setDirty(); //isDirty = 1
	void show();
	char* getContent(int start, int bytes);//读取块内数据
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

