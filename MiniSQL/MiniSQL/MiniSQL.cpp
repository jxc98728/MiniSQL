/* MiniSQL.h�ж������ĺ���ʵ�� */
#include "MiniSQL.h"
#include <iostream>
using namespace std;

Block::Block()
{
	tableName = "NULL"; //��ǿձ�
	blockid = -1;//�ж�ʱ���blockid < 0��˵��Ϊ��block������д��
	size = 0;
	isDirty = false;
	memset(content, 0, BLOCK_SIZE);
}

Block::Block(string table)
{
	tableName = table;
	blockid = -1;//�ж�ʱ���blockid < 0��˵��Ϊ��block�򣬲���д��
	size = 0;
	isDirty = false;
	memset(content, 0, BLOCK_SIZE);
}

//ȷ����table��blockid�Ϳ���ʵ�ʶ�ȡ
Block::Block(string table, int blockid)
{
	tableName = table;
	this->blockid= blockid;
	size = 0;
	isDirty = false;
	memset(content, 0, BLOCK_SIZE);
}

//��recordתΪ�����ƴ���char content[4096]�У����ؿ��Ƿ񹻲���
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
	memcpy(result, content + start + 1, bytes);//startΪ��valid byte�����ݶ�
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
