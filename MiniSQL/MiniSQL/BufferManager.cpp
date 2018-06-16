#include "BufferManager.h"
using namespace std;

BufferManager::BufferManager(string db)
{
	dbName = db;
}

BufferManager::~BufferManager()
{
	allWrite();//写回脏数据到文件
}

Block & BufferManager::findBlk(string table, int offset)
{
	//先在buffer中查找，如果找到将其提到list最前
	for (list<Block>::iterator i = buffer.begin(); i != buffer.end(); i++) {
		if (i->tableName == table && offset >= i->offset && offset <= i->nextOffset) {
			buffer.splice(buffer.begin(), buffer, i, std::next(i));
			return (buffer.front());
		}
	}
	//buffer中找不到则从文件读取
	if (buffer.size() >= MAX_BLOCKS) {//写满则根据LRU将最后一个写回
		writeBlock(*(buffer.end()));
		buffer.pop_back();
	}
	buffer.push_front(readBlock(table,offset));
	
	return (buffer.front());//返回队列最前的block
}

Block BufferManager::readBlock(string table, int offset)
{
	Block block(table);
	string path = table + ".dat";
	fstream File;
	File.open(path, ios::in | ios::out | ios::binary);

	// read the block according to offset
	File.seekg(offset);
	File.read(block.content, BLOCK_SIZE);

	//还没改动自然not dirty
	block.isDirty = false;
	block.offset = offset;
	block.tableName = table;

	block.size = strlen(block.content);
	
	File.close();
	return block;
}

/* 写回Block到对应的table对应的.dat文件 */
void BufferManager::writeBlock(Block & block)
{
	string path(block.tableName);
	path += ".dat";

	fstream file;
	file.open(path, ios::in | ios::out | ios::binary );
	if (!file.good()) {
		cerr << "Fail to open file:" << path << endl;
	}

	if (block.isDirty) {
		block.isDirty = false;//写回后就不是脏数据了
	}
	else {
		return; //不需要写回
	}

	//write block head
	file.seekp(block.offset);//通过offset确定写block在文件中位置

	//write back block content

	file.seekp(block.offset);//预留100字符位作为block head
	file.write(block.content, BLOCK_SIZE);

	file.close();
	return;
}

void BufferManager::allWrite()
{
	for (list<Block>::iterator i = buffer.begin(); i != buffer.end(); i++) {
		writeBlock(*i);
	}
	return;
}
