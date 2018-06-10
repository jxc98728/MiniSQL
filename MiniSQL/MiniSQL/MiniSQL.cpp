/* MiniSQL.h中定义的类的函数实现 */
#include "MiniSQL.h"
#include <iostream>
using namespace std;


Block::Block(string table)
{
	strcpy(tableName, table.c_str());
}

Block::Block(string table, int loc)
{
	strcpy(tableName, table.c_str());
	offset = loc;
	nextOffset = 0;
	size = 0;
	isDirty = true;
	memset(content, 0, BLOCK_SIZE);
}

int Block::setContent(uint8_t * data, int length)
{
	if (size + length <= BLOCK_SIZE) {
		memcpy(content + size, data, size);
		return 1;
	}
	else return 0;
}

void Block::setDirty()
{
	isDirty = true;
}

char * Block::getContent()
{
	return content;
}

Block::~Block()
{ }
