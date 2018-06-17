#pragma once
/*
BufferManager 用于缓冲区的管理，作为其他部件和文件读写的接口。
一个BufferManager对象对应一个数据库 在读数据时如果buffer中有所需数据则返回，
如果没有则依据LRU（最近最少使用）作为替换算法替换Buffer中的块。
只有在BufferManager被析构或这个块被LRU替换时会将dirty块写进文件

注意：BufferManager中可以存一个数据库中的所有表的blocks
构造时用数据库名作参数BufferManager(dbName)

数据按表存-table1.dat -table2.dat

TODO:LRU算法更高效的实现。
*/
#include "MiniSQL.h"
#include <string>
#include <list>
#include <iostream>
#include <fstream>

using namespace std;

#define BLOCK_SIZE 4096 /* block of size 4K bytes */
#define MAX_BLOCKS 64 /* 缓冲区的最大块数, 待定 */
class BufferManager
{
public:
	list<Block> buffer; //数据缓冲区

public: //构造&析构
	BufferManager() = default;
	//析构函数要写回所有脏块
	~BufferManager();

public: //先在buffer中查找块，找不到则从文件读到buffer
	Block & findBlk(string table, int offset);

public: //buffermanager <-> file BLOCK读写
	Block readBlock(string table, int offset);
	void writeBlock(Block &block);//在替换时if block.isDirty=true写回
	void allWrite();//析构时调用

//TODO:BufferManager给IndexManager的接口
//TODO:BufferManager给RecordManager的接口

};