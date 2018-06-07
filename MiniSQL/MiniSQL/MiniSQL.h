#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;
typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;
#define BLOCK_SIZE 4096 /* block of size 4K bytes */

/* block类用于buffermanager中缓存的写入与读取,大小为4K bytes */
class block {
public:
	char table[32]; //block对应的表名称
	int size; //当前block已用空间的大小
	uint8_t content[BLOCK_SIZE]; //block中的数据
	bool isDirty; //isDirty = 1表示有待写回文件
	int offset; //当前block在文件中的偏移量
	int next; //下一块block在文件中的偏移量

public:
	block();
	void setContent(uint8_t* data); //在当前content基础上增加数据
	uint8_t* getContent();
	~block();
};


