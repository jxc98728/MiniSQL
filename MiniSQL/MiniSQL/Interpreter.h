#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "MiniSQL.h"
using namespace std;

/* Clause�޸�ΪMinisql.h�е�Condition�� */
class Clause {
	string attriName;
	string cond;
	int	opcode;
	/*
		#0: >
		#1: <
		#2: >=
		#3. <=
		#4: =
		#5: <>
	*/
};

class SQLCommand {
public:
	//ADD:
	Table tableInfo;
	Index indexInfo;
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
	int opName;

	// ����������������/Attribute��
	// �������� select, create index
	// ע��: attriNameList[0] == "*" ʱ��ʾselect������
	vector<string> attriNameList;

	// �����Ӿ� E.g  ... where attri opcode cond
	//	Like		... where s_id	   =   "111"
	// �������� select, delete
	vector<Condition> condList;

	// ��ֵ
	// �������� insert. 
	// ����һ��recordʱvaluesֵ�����������list��
	vector<string> valuesList;
};

class Interpreter {
public:
	// ��statusΪfalseʱִֹͣ��
    bool status;

	// SQL���
	string query;

	// �ļ���
	fstream file;

	// ����������Ĳ���
	SQLCommand op;

	// Constructor
    Interpreter();

	// Destroyer
    ~Interpreter();
    
	// �Ӽ��̶�ȡsql���
	void readquery(void);

	/* ���ģ�������
	   ���ܣ�
	   1. ����SQL��䣬����﷨��������Ӧ�﷨����
	   2. ��䲻�����﷨�򱨴����ط���ֵ����������ִ�С�
	*/
	int  parser(void);

	// API�ӿڣ������﷨��ִ����Ӧ�Ĳ���
	void execute(void);

	// ִ��sql�ļ�
	void executefile(void);

	// ������
	void print(void);

};

#endif