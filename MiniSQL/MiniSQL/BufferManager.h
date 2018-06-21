#pragma once
/*
	BufferManager ���ڻ������Ĺ�����Ϊ�����������ļ���д�Ľӿڡ�
	�ڶ�����ʱ���buffer�������������򷵻أ�
	���û��������LRU���������ʹ�ã���Ϊ�滻�㷨�滻Buffer�еĿ顣
	ֻ����BufferManager������������鱻LRU�滻ʱ�Ὣdirty��д���ļ�

	ע�⣺BufferManager�п��Դ����б��blocks
	���ݰ����-table1.dat -table2.dat
*/
#include "MiniSQL.h"
#include <string>
#include <list>
#include <iostream>
#include <fstream>

using namespace std;

#define BLOCK_SIZE 4096 /* block of size 4K bytes */
#define MAX_BLOCKS 64 /* ��������������, ���� */
class BufferManager
{
public:
	list<Block> buffer; //���ݻ�����

public: //����&����
	BufferManager() = default;
	//��������Ҫд���������
	~BufferManager();

public: //����buffer�в��ҿ飬�Ҳ�������ļ�����buffer
	Block & findBlk(Table table, int offset);//����offset��record�ľ���offset

public: //buffermanager <-> file BLOCK��д
	Block readBlock(Table table, int blockid);
	void writeBlock(Block &block);//���滻ʱif block.isDirty=trueд��
	void allWrite();//����ʱ����
};