#include "BufferManager.h"
using namespace std;

BufferManager::BufferManager(string db)
{
	dbName = db;
}

BufferManager::~BufferManager()
{
	allWrite();//д�������ݵ��ļ�
}

Block & BufferManager::findBlk(string table, int offset)
{
	//����buffer�в��ң�����ҵ������ᵽlist��ǰ
	for (list<Block>::iterator i = buffer.begin(); i != buffer.end(); i++) {
		if (i->tableName == table && offset >= i->offset && offset <= i->nextOffset) {
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

Block BufferManager::readBlock(string table, int offset)
{
	Block block(table);
	string path = table + ".dat";
	fstream File;
	File.open(path, std::fstream::in | std::fstream::out);

	// read block head
	File.seekg(offset);
	File.read(block.tableName, TABLE_NAME_LENGTH);
	File.read((char *)&(block.size), sizeof(int));
	File.read((char *)&(block.isDirty), sizeof(bool));
	File.read((char *)&(block.offset), sizeof(int));
	File.read((char *)&(block.nextOffset), sizeof(int));

	// read the block according to offset
	File.seekg(offset + BLOCK_HEAD);
	File.read(block.content, BLOCK_SIZE);

	//��û�Ķ���Ȼnot dirty
	block.isDirty = false;
	return block;
}

/* д��Block����Ӧ��table��Ӧ��.dat�ļ� */
void BufferManager::writeBlock(Block & block)
{
	string path(block.tableName);
	path += ".dat";
	fstream file;
	file.open(path, std::fstream::in | std::fstream::out);
	if (block.isDirty) {
		block.isDirty = false;//д�غ�Ͳ�����������
	}
	else {
		return; //����Ҫд��
	}

	//write block head
	file.seekp(block.offset);//ͨ��offsetȷ��дblock���ļ���λ��
	file.write(block.tableName, TABLE_NAME_LENGTH);
	file.write((char *)&(block.size), sizeof(int));
	file.write((char *)&(block.isDirty), sizeof(bool));
	file.write((char *)&(block.offset), sizeof(int));
	file.write((char *)&(block.nextOffset), sizeof(int));

	//write back block content
	file.seekp(block.offset + BLOCK_HEAD);//Ԥ��100�ַ�λ��Ϊblock head
	file.write(block.content, BLOCK_SIZE);

	return;
}

void BufferManager::allWrite()
{
	for (list<Block>::iterator i = buffer.begin(); i != buffer.end(); i++) {
		writeBlock(*i);
	}
	return;
}
