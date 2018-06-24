#pragma once
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <iomanip>
#include "BPlusTree.h"

using namespace std;


class TreeInfo {

public:
	string tableName;
	string indexName;

	/*
	list[id] -> B+ Tree
	*/
	int		id;

	TreeInfo(const string & tableName, const string & indexName, int type, int id)
		:tableName(tableName), indexName(indexName), id(id) {}
};

class IndexManager {
public:

	//BufferManager * bufferm;
	//CatalogManager * catalogm;

	// 管理多颗B+树
	map<string, int>	intTreeList;
	map<string, int>	fltTreeList;
	map<string, int>	strTreeList;

	vector<BPlusTree<int>>		int_treelist;
	vector<BPlusTree<float>>	flt_treelist;
	vector<BPlusTree<string>>	str_treelist;

	set<int> SearchResult;

	// Constructor
	IndexManager();

	// Destroyer
	~IndexManager();

	// 
	void writeBackToDisk(void);


	// 在tableName的表中加入一个以indexName为key的index, 若失败return false.
	bool createIndex(const string & tableName, const string & indexName, int type);

	// 在tableName的表中删除以indexName为key的index, 若失败或index不存在则return false.
	bool dropIndex(const string tableName, const string & indexName, int type);

	// 在进行insert操作时对对应的B+树进行插入更新，为了方便，key值统一用string传递，根据catalog中的type再做相应的转换，offset是包含该key的record在文件中的偏移量。
	bool insert(const string tableName, const string & indexName, const string & key, int type, int offset);

	// 在进行delete操作时对对应的B+树进行更新，为了方便，key值统一用string传递，根据catalog的type再做相应的转换
	// 注意：会删除所有键值为key的项！
	bool remove(const string tableName, const string & indexName, const string & key, int type);


	/* 等值查找：用于执行 “=” 运算符
		1. 在tableName的表以indexName为索引值的B+树中查找键值为key的所有项, 返回值为查询结果的个数。
		2. 查询结果是匹配键值所在record在文件中的偏移量，放在全局set数组SearchResult中（定义在MiniSQL.cpp中），
		   执行时不会清空SearchResult数组（方便多条件查询）。需要调用者在操作前自行清空。
		3. type是key的数据类型
			#0: int
			#-1: float
			#n: char(n)
	*/
	int selectEqual(const string tableName, const string & indexName, const string & key, int type);

	/* 不等值查找：用于执行 “<>” 运算符
		1. 在tableName的表以indexName为索引值的B+树中查找键值不为key的所有项, 返回值为查询结果的个数。
		2. 查询结果是匹配键值所在record在文件中的偏移量，放在全局set数组SearchResult中（定义在MiniSQL.cpp中）
		   执行时不会清空SearchResult数组（方便多条件查询）。需要调用者在操作前自行清空。
		3. type是key的数据类型
			#0: int
			#-1: float
			#n: char(n)
	*/
	int selectInequal(const string tableName, const string & indexName, const string & key, int type);

	/* 条件查找(Range Query)：用于执行 “>, < <=, >=” 运算符
		1. 在tableName的表以indexName为索引值的B+树中查找符合键值满足条件关系的所有项, 例如“ key >= 1234 ”。
		   返回值为查询结果的个数。
		2. 查询结果是匹配键值所在record在文件中的偏移量，放在全局set数组SearchResult中（定义在MiniSQL.cpp中）
		   执行时不会清空SearchResult数组（方便多条件查询）。需要调用者在操作前自行清空。
		3. type是key的数据类型：
			#0: int
			#-1: float
			#n: char(n)
		4. opcode是比较符类型：
			#0: >
			#1: <
			#2: >=
			#3. <=
			#4: =
			#5: <>
		5. cond是条件值
	*/
	int selectRange(const string tableName, const string & indexName, const string & cond, int opcode, int type);

};
