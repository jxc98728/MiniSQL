#include "BufferManager.h"
using namespace std;

BufferManager::~BufferManager()
{
	allWrite();//д�������ݵ��ļ�
}

Block & BufferManager::findBlk(Table table, int offset)
{
	//����buffer�в��ң�����ҵ������ᵽlist��ǰ
	for (list<Block>::iterator i = buffer.begin(); i != buffer.end(); i++) {
		if (i->tableName == table.name && offset >= i->blockid*BLOCK_SIZE && offset <= (i->blockid + 1)*BLOCK_SIZE) {
			buffer.splice(buffer.begin(), buffer, i, std::next(i));
			return (buffer.front());
		}
	}
	//buffer���Ҳ�������ļ���ȡ
	if (buffer.size() >= MAX_BLOCKS) {//д�������LRU�����һ��д��
		writeBlock(*(buffer.end()));
		buffer.pop_back();
	}
	buffer.push_front(readBlock(table,offset));
	
	return (buffer.front());//���ض�����ǰ��block
}

void BufferManager::block2buf(Block & block)
{
	//����buffer�в��ң�����ҵ������ᵽlist��ǰ
	for (list<Block>::iterator i = buffer.begin(); i != buffer.end(); i++) {
		if (i->tableName == block.tableName && i->blockid == block.blockid) {
			buffer.splice(buffer.begin(), buffer, i, std::next(i));
			return;
		}
	}
	//��ʱbuffer��û�иÿ飬��blockֱ�ӷŵ�list��headλ��
	buffer.push_front(block);
	return;
}

Block& BufferManager::readBlock(Table table, int blockid)
{
	Block block(table.name,blockid);
	string path = table.name + ".dat";
	fstream File;
	//file�Զ����ƽ��ж�д
	File.open(path, ios::in | ios::out | ios::binary );

	// read the block according to blockid
	File.seekg(blockid * BLOCK_SIZE);
	File.read(block.content, BLOCK_SIZE);
	//��û�Ķ���Ȼnot dirty
	block.isDirty = false;
	if (blockid != table.blockNum - 1)
		block.size = BLOCK_SIZE;
	else {
		block.size = table.fileTail % BLOCK_SIZE;
	}
	block2buf(block); //д��buffer������
	File.close();
	return block;
}

/* д��Block����Ӧ��table��Ӧ��.dat�ļ� */
void BufferManager::writeBlock(Block & block)
{
	string path(block.tableName);
	path += ".dat";
	fstream file;
	//file�Զ����ƽ��ж�д
	file.open(path, ios::in | ios::out | ios::binary );
	if (!file.good()) {
		cerr << "Fail to open file:" << path << endl;
	}
	if (block.isDirty) {
		block.isDirty = false;//д�غ�Ͳ�����������
	}
	else {
		return ; //����Ҫд��
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
