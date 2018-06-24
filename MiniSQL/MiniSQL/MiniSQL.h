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
	bool PK; //primary key
	bool unique;
	Attribute() = default;
	Attribute(string n, int t, bool p, bool u)
		:name(n), type(t), PK(p), unique(u)
	{ };
	void show();
};

/* Table class in CatalogManager, only table info without records */
class Table {
public:
	string name; //name of the table
	int attriNum; //number of the attributes
	vector<Attribute> attributes; //list of the attributes
	int recNum;
	int recLength; //length of a record(һ��record�ж���byte���̶�ֵ)
	int blockNum;
	int fileTail; //�ļ�β��offset��ΪBlock��ȡ�Ĳο�
	Table() = default;
	Table(string n)
		: name(n)
	{ };
	Table(string n, int attrNum, std::vector<Attribute> attrs)
		: name(n), attriNum(attrNum), attributes(attrs), recNum(0), blockNum(1),fileTail(0)
	{
		int length = 0;
		/*for (auto elem : attrs) {
			length += elem.size();
		}
		recLength = length;*/
	}
	void show();
};

/* index class in IndexManager, information of a index created */
class Index {
public:
	string tableName;
	string indexName;
	string attributeName;
	int type; //TODO:������ʱ��ֵtype
	Index() = default;
	Index(string tbName, string idName, string attr)
		:tableName(tbName), indexName(idName), attributeName(attr)
	{ }  //TODO:��API�ڸ�ֵtype
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
	Row() = default;
};
class Records { //��ѯ���ص�records��row�ļ���
public:
	vector<Row> rows;
};

enum { Gt, Lt, Ge, Le, Eq, Ne };
class Condition {
public:
	string attriName;
	int columnNum; //������attributes�е�index
	string cond;
	int opcode;
};

class SQLCommand {
public:
	Table tableInfo;
	Index indexInfo;
	int opName;
	vector<Condition> condList;
	vector<string> valuesList;
};

//�ڲ�ͬ����֮��ת���ĺ���
char* string2int(string s); //��stringת��Ϊ���ֽڱ�ʾ��int
char* string2float(string s); //��stringת��Ϊ���ֽڱ�ʾ��int

string int2string(char * i); //�����ֽڱ�ʾ��intת��Ϊstring
string float2string(char * f); //�����ֽڱ�ʾ��floatת��Ϊstring

int char2int(char * i); //�����ֽڱ�ʾ��intת��Ϊint
float char2float(char * f); //�����ֽڱ�ʾ��floatת��Ϊfloat