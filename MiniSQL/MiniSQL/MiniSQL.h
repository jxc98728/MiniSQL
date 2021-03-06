#pragma once
/*
	注意在程序中尽量用string操作，对文件中的数据用二进制进行读取和写入
	Minisql.h定义了其他Manager中需要用到的基本类
	
*/
#include <iostream>
#include <vector>
#include <string>
using namespace std;

/* type定义：int -1 float 0 char(n) n */
#define BLOCK_SIZE 4096 /* block of size 4K bytes */

/* Attribute consisting the info of type/length/primary key/unique */
class Attribute {
public:
	string name;
	int type;
	bool PK; //primary key
	bool unique;
	Attribute() = default;
	Attribute(string n, int t, bool p, bool u)
		:name(n), type(t), PK(p), unique(u)
	{ };
	void show();
	int size() {
		if (type == -1) //INT
			return 4;
		else if (type == 0) //FLOAT
			return 4;
		else return type; //CHAR(n)
	}
};

/* Table class in CatalogManager, only table info without records */
class Table {
public:
	string name; //name of the table
	int attriNum; //number of the attributes
	vector<Attribute> attributes; //list of the attributes
	int recNum;
	int recLength; //length of a record(一个record有多少byte，固定值)
	int blockNum;
	int fileTail; //文件尾的offset作为Block读取的参考
	Table() = default;
	Table(string n)
		: name(n)
	{ };
	Table(string n, int attrNum, std::vector<Attribute> attrs)
		: name(n), attriNum(attrNum), attributes(attrs), recNum(0), blockNum(1),fileTail(0)
	{
		//计算得出reclength
		int length = 0;
		for (auto elem : attrs) {
			length += elem.size();
		}
		recLength = length;
	}
	void show();
};

/* index class in IndexManager, information of a index created */
class Index {
public:
	string tableName;
	string indexName; //index名字
	string attributeName; //index对应属性名字
	int type; //TODO:创建的时候赋值type（用catalogmanager找到该属性的type）
	Index() = default;
	Index(string tbName, string idName, string attr)
		:tableName(tbName), indexName(idName), attributeName(attr)
	{ }  //TODO:在API内赋值type
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
	char content[BLOCK_SIZE]; //block中的数据 char[4096] 用二进制格式保存
public:
	Block();
	Block(string table);
	Block(string table, int blockid);
	bool insertRecord(char* src, int length);
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

enum { Gt, Lt, Ge, Le, Eq, Ne }; //五种条件判断的opcode值
class Condition {
public:
	string attriName;
	int columnNum; //属性在attributes中的index
	string cond;
	int opcode;
	Condition() = default;
	Condition(string attr, int opcode, string cond)
		:attriName(attr), opcode(opcode), cond(cond)
	{}; //TODO:在Interprete时把columnNum赋值上
};

class SQLCommand {
public:
	Table tableInfo;
	Index indexInfo;
	int opName;
	vector<Condition> condList;
	vector<string> valuesList;
	SQLCommand() = default; //TODO:在Interpreter中构造Command把SQLCommand成员赋值
};

//在不同类型之间转换的函数 done
char* string2int(string s); //将string转换为四字节表示的int done
char* string2float(string s); //将string转换为四字节表示的int done

int char2int(char * i); //将四字节表示的int转换为int done
float char2float(char * f); //将四字节表示的float转换为float done