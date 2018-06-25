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
	int recLength; //length of a record(һ��record�ж���byte���̶�ֵ)
	int blockNum;
	int fileTail; //�ļ�β��offset��ΪBlock��ȡ�Ĳο�
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
	int type; //TODO:������ʱ��ֵtype
	Index() = default;
	Index(string tbName, string idName, string attr)
		:tableName(tbName), indexName(idName), attributeName(attr)
	{ }  //TODO:��API�ڸ�ֵtype
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

	// �����ı� E.g. t1
	//string tableName;

	// �Ըñ�Ĳ��� E.g. select, insert, create table, drop, etc.
	/*
		��Ӧ��ϵ��
		create table:	0
		drop table	:	1
		create index:	2
		drop index	:	3
		select		:	4
		insert		:	5
		delete		:	6

		(quit��execfileֱ���ж���ת)
	*/
	//int opName;

	// ����������������/Attribute��
	// �������� select, create index
	// ע��: attriNameList[0] == "*" ʱ��ʾselect������
	//vector<string> attriNameList;

	// �����Ӿ� E.g  ... where attri opcode cond
	//	Like		... where s_id	   =   "111"
	// �������� select, delete
	//vector<Clause> condList;

	// ��ֵ
	// �������� insert. 
	// ����һ��recordʱvaluesֵ�����������list��
	//vector<string> valuesList;


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

	void clear(void) {
		tableInfo.attributes.clear();
		condList.clear();
		valuesList.clear();
	}
};

class Interpreter {
public:

	// SQL���
	string query;

	// tokenier
	vector<string> tokenizer;

	// �ļ���
	fstream file;

	// ����������Ĳ���
	SQLCommand sqlcmd;

	// Constructor
    Interpreter();

	// Destroyer
    ~Interpreter();
    
	// �Ӽ��̶�ȡsql���, ���ִ�
	void readquery(void);

	/* ���ģ�������
	   ���ܣ�
	   1. ����SQL��䣬����﷨��������Ӧ�﷨����
	   2. ��䲻�����﷨�򱨴����ط���ֵ����������ִ�С�
	*/
	int parser(void);

	// API�ӿڣ������﷨��ִ����Ӧ�Ĳ���, �����������ȷ�ԣ��߼���)��
	void execute(void);

	// ִ��sql�ļ�
	void executefile(void);

	// ������
	void print(void);

	string & trim(string & str);

	void token_debug(void);

	void parse_debug(void);

};
