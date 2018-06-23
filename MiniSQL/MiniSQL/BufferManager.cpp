#include "BufferManager.h"
using namespace std;

BufferManager::~BufferManager()
{
	allWrite();//写回脏数据到文件
}

Block & BufferManager::findBlk(Table table, int offset)
{
	//先在buffer中查找，如果找到将其提到list最前
	for (list<Block>::iterator i = buffer.begin(); i != buffer.end(); i++) {
		if (i->tableName == table.name && offset >= i->blockid*BLOCK_SIZE && offset <= (i->blockid + 1)*BLOCK_SIZE) {
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

void BufferManager::block2buf(Block & block)
{
	//先在buffer中查找，如果找到将其提到list最前
	for (list<Block>::iterator i = buffer.begin(); i != buffer.end(); i++) {
		if (i->tableName == block.tableName && i->blockid == block.blockid) {
			buffer.splice(buffer.begin(), buffer, i, std::next(i));
			return;
		}
	}
	//此时buffer中没有该块，则将block直接放到list的head位置
	buffer.push_front(block);
	return;
}

Block& BufferManager::readBlock(Table table, int blockid)
{
	Block block(table.name,blockid);
	string path = table.name + ".dat";
	fstream File;
	//file以二进制进行读写
	File.open(path, ios::in | ios::out | ios::binary );

	// read the block according to blockid
	File.seekg(blockid * BLOCK_SIZE);
	File.read(block.content, BLOCK_SIZE);
	//还没改动自然not dirty
	block.isDirty = false;
	if (blockid != table.blockNum - 1)
		block.size = BLOCK_SIZE;
	else {
		block.size = table.fileTail % BLOCK_SIZE;
	}
	block2buf(block); //写入buffer缓冲区
	File.close();
	return block;
}

/* 写回Block到对应的table对应的.dat文件 */
void BufferManager::writeBlock(Block & block)
{
	string path(block.tableName);
	path += ".dat";
	fstream file;
	//file以二进制进行读写
	file.open(path, ios::in | ios::out | ios::binary );
	if (!file.good()) {
		cerr << "Fail to open file:" << path << endl;
	}
	if (block.isDirty) {
		block.isDirty = false;//写回后就不是脏数据了
	}
	else {
		return ; //不需要写回
	}
	file.seekp(block.blockid * BLOCK_SIZE);
	file.write(block.content, BLOCK_SIZE);
	file.close();
	return ;
}

void BufferManager::allWrite()
{
	for (list<Block>::iterator i = buffer.begin(); i != buffer.end(); i++) {
		writeBlock(*i);
	}
	return;
}
