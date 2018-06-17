/* MiniSQL.h中定义的类的函数实现 */
#include "MiniSQL.h"
#include <iostream>
using namespace std;


Block::Block(string table)
{
	tableName = table;
}

Block::Block(string table, int loc)
{
	tableName = table;
	offset = loc;
	nextOffset = 0;
	size = 0;
	isDirty = true;
	memset(content, 0, BLOCK_SIZE);
}

int Block::setContent(char* data, int length)
{
	if (size + length <= BLOCK_SIZE) {
		memcpy(content + size, data, length);
		size += length;
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