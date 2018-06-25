#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

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
	void show()
	{
		cout << name;
		switch (type)
		{
		case 0:
			cout << " int";
			break;
		case 1:
			cout << " float";
			break;
		case 2:
			cout << " char(" << type << ")";
			break;
		}
		if (PK) {
			cout << " Primary Key";
		}
		else if (unique) {
			cout << " Unique";
		}
		cout << endl;
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
	Table(string n) : name(n) { };
	Table(string n, int attrNum, std::vector<Attribute> attrs)
		: name(n), attriNum(attrNum), attributes(attrs), recNum(0), blockNum(1), fileTail(0)
	{
		int length = 0;
		/*for (auto elem : attrs) {
		length += elem.size();
		}
		recLength = length;*/
	}
	void show() {
		cout << "table " << name << ":" << attriNum << " attributes" << endl;
		for (auto elem : attributes) {
			elem.show();
		}
	}
};

/* index class in IndexManager, information of a index created */
class Index {
public:
	string tableName;
	string indexName;
	string attributeName;
	int type; //TODO:创建的时候赋值type
	Index() = default;
	Index(string tbName, string idName, string attr)
		:tableName(tbName), indexName(idName), attributeName(attr)
	{ }  //TODO:在API内赋值type
};

//class SQLCommand {
	/*
		Example1: select * from t1 where a1 = c1 and where a2 >= c2;

		After parsing:
		(1) tableName = "t1";
		(2) opName = 4;
		(3) attriNameList = ["*"];
		(4) condList = [{"a1", "c1", 4}, {"a2", "c2", 2}];
		(5) valusList = Empty Set

		Example2: insert into t2 values(1, "ABC", 0.0023);

		After parsing:
		(1) tableName = "t2";
		(2) opName = 5;
		(3) attriNameList = Empty Set;
		(4) condList = Empty Set;
		(5) valusList = ["1", "ABC", "0.0023"]

		Example3: delete from t3 where bno = "888";

		After parsing:
		(1) tableName = "t3";
		(2) opName = 6;
		(3) attriNameList = Empty Set;
		(4) condList = [{"bno", "888", 4}];
		(5) valuesList = Empty Set


		Example4: create index newIndex on student (s_id);

		After parsing:
		(1) tableName = "student";
		(2) opName = 2;
		(3) attriNameList = Empty Set;
		(4) condList = [{"bno", "888", 4}];
		(5) valuesList = Empty Set
	*/

	// 操作的表 E.g. t1
	//string tableName;

	// 对该表的操作 E.g. select, insert, create table, drop, etc.
	/*
		对应关系：
		create table:	0
		drop table	:	1
		create index:	2
		drop index	:	3
		select		:	4
		insert		:	5
		delete		:	6

		(quit和execfile直接判断跳转)
	*/
	//int opName;

	// 操作的列名（属性/Attribute）
	// 仅适用于 select, create index
	// 注意: attriNameList[0] == "*" 时表示select所有列
	//vector<string> attriNameList;

	// 条件子句 E.g  ... where attri opcode cond
	//	Like		... where s_id	   =   "111"
	// 仅适用于 select, delete
	//vector<Clause> condList;

	// 赋值
	// 仅适用于 insert. 
	// 插入一条record时values值被解析到这个list中
	//vector<string> valuesList;


enum { Gt, Lt, Ge, Le, Eq, Ne };
class Condition {
public:
	string attriName;
	int columnNum; //属性在attributes中的index
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

	void clear(void) {
		tableInfo.attributes.clear();
		condList.clear();
		valuesList.clear();
	}
};

class Interpreter {
public:

	// SQL语句
	string query;

	// tokenier
	vector<string> tokenizer;

	// 文件流
	fstream file;

	// 经过解析后的操作
	SQLCommand sqlcmd;

	// Constructor
    Interpreter();

	// Destroyer
    ~Interpreter();
    
	// 从键盘读取sql语句, 并分词
	void readquery(void);

	/* 核心：解析器
	   功能：
	   1. 分析SQL语句，检查语法，生产对应语法树。
	   2. 语句不符合语法则报错并返回非零值，若符合则执行。
	*/
	int parser(void);

	// API接口，根据语法树执行相应的操作, 并检查语义正确性（逻辑层)。
	void execute(void);

	// 执行sql文件
	void executefile(void);

	// 输出结果
	void print(void);

	string & trim(string & str);

	void token_debug(void);

	void parse_debug(void);

};
