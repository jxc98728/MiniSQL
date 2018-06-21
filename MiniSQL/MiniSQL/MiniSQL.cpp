/* MiniSQL.h中定义的类的函数实现 */
#include "MiniSQL.h"
#include <iostream>
using namespace std;

Block::Block()
{
	tableName = "NULL"; //标记空表
	blockid = -1;//判断时如果blockid < 0就说明为空block，不用写回
	size = 0;
	isDirty = false;
	memset(content, 0, BLOCK_SIZE);
}

Block::Block(string table)
{
	tableName = table;
	blockid = -1;//判断时如果blockid < 0就说明为空block或，不用写回
	size = 0;
	isDirty = false;
	memset(content, 0, BLOCK_SIZE);
}

//确认了table和blockid就可以实际读取
Block::Block(string table, int blockid)
{
	tableName = table;
	this->blockid= blockid;
	size = 0;
	isDirty = false;
	memset(content, 0, BLOCK_SIZE);
}

//将record转为二进制存于char content[4096]中，返回块是否够插入
bool Block::insertRecord(char *src, int length)
{
	if (length + size + 1 > BLOCK_SIZE) {
		return false;
	}
	memset(content + size, 1, 1);
	memcpy(content + size + 1, src, length);
	size += (length + 1);
	return true;
}

void Block::setDirty()
{
	isDirty = true;
}

void Block::show()
{
	cout << "Table: " << tableName << " Blockid: " << blockid << endl;
	cout << "Already used size: " << size << "   " << "isDirty: " << isDirty << endl;
}

char * Block::getContent(int start, int bytes)
{
	char result[BLOCK_SIZE];
	memset(result, 0, BLOCK_SIZE);
	memcpy(result, content + start + 1, bytes);//start为带valid byte的数据段
	return result;
}

void Table::show()
{
	cout << "table " << name << ":" << attriNum << " attributes" << endl;
	for (auto elem : attributes) {
		elem.show();
	}
}

void Attribute::show()
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
		cout << " char(" << charlength << ")";
		break;
	}
	if (PK) {
		cout << " Primary Key";
	}
	else if(unique){
		cout << " Unique";
	}
	cout << endl;
}
