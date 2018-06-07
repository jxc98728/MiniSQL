#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;
typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;
#define BLOCK_SIZE 4096 /* block of size 4K bytes */

/* block������buffermanager�л����д�����ȡ,��СΪ4K bytes */
class block {
public:
	char table[32]; //block��Ӧ�ı�����
	int size; //��ǰblock���ÿռ�Ĵ�С
	uint8_t content[BLOCK_SIZE]; //block�е�����
	bool isDirty; //isDirty = 1��ʾ�д�д���ļ�
	int offset; //��ǰblock���ļ��е�ƫ����
	int next; //��һ��block���ļ��е�ƫ����

public:
	block();
	void setContent(uint8_t* data); //�ڵ�ǰcontent��������������
	uint8_t* getContent();
	~block();
};


