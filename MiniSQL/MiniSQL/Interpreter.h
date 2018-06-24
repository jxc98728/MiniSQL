#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "MiniSQL.h"
using namespace std;

/* Clause修改为Minisql.h中的Condition类 */
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
	int opName;

	// 操作的列名（属性/Attribute）
	// 仅适用于 select, create index
	// 注意: attriNameList[0] == "*" 时表示select所有列
	vector<string> attriNameList;

	// 条件子句 E.g  ... where attri opcode cond
	//	Like		... where s_id	   =   "111"
	// 仅适用于 select, delete
	vector<Condition> condList;

	// 赋值
	// 仅适用于 insert. 
	// 插入一条record时values值被解析到这个list中
	vector<string> valuesList;
};

class Interpreter {
public:
	// 当status为false时停止执行
    bool status;

	// SQL语句
	string query;

	// 文件流
	fstream file;

	// 经过解析后的操作
	SQLCommand op;

	// Constructor
    Interpreter();

	// Destroyer
    ~Interpreter();
    
	// 从键盘读取sql语句
	void readquery(void);

	/* 核心：解析器
	   功能：
	   1. 分析SQL语句，检查语法，生产对应语法树。
	   2. 语句不符合语法则报错并返回非零值，若符合则执行。
	*/
	int  parser(void);

	// API接口，根据语法树执行相应的操作
	void execute(void);

	// 执行sql文件
	void executefile(void);

	// 输出结果
	void print(void);

};

#endif