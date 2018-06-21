#include "CatalogManager.h"
#include <fstream>
using namespace std;

CatalogManager::CatalogManager()
{
	readTables();
	readIndexes();
}

void CatalogManager::readTables()
{
	string fileName = "Tables.log";
	fstream file(fileName);
	file >> tableNum;
	//����Tables.dat�е���Ϣ��vector<Table>��
	Table table;
	Attribute attribute;
	int type;
	for (int i = 0; i < tableNum; i++) {
		file >> table.name;
		file >> table.attriNum;
		file >> table.fileTail;
		file >> table.blockNum;
		//������м�¼��attributes����Ϣ
		for (int j = 0; j < table.attriNum; j++) {
			file >> attribute.name;
			file >> attribute.type;
			file >> attribute.charlength;
			file >> attribute.PK;
			file >> attribute.unique;
			table.attributes.push_back(attribute);
		}
		tables.push_back(table);
	}
	file.close();
}

void CatalogManager::readIndexes()
{
	string fileName = "Indexes.log";
	fstream file(fileName);
	file >> indexNum;
	//����.index�е���Ϣ��vector<Index>��
	Index index;
	for (int i = 0; i < indexNum; i++)
	{
		file >> index.indexName;
		file >> index.tableName;
		file >> index.columnIndex;
		file >> index.attrLength;
		file >> index.blockNum;
		indexes.push_back(index);
	}
	file.close();
}

void CatalogManager::writeTables()
{
	string fileName = "Tables.log";
	fstream  file(fileName);
	file << tableNum << endl;
	//��.table��д�뵱ǰManager�еı���Ϣ
	//(�������Ϣ�������൱������дһ�������ļ�)
	for (int i = 0; i < tableNum; i++)
	{
		file << tables[i].name << " ";
		file << tables[i].attriNum << " ";
		file << tables[i].fileTail << " ";
		file << tables[i].blockNum << endl;
		//д�뵥���������������Ϣ
		//һ��attributeһ��
		for (int j = 0; j < tables[i].attriNum; j++)
		{
			file << tables[i].attributes[j].name << " ";
			file << tables[i].attributes[j].type << " ";
			file << tables[i].attributes[j].charlength << " ";
			file << tables[i].attributes[j].PK << " ";
			file << tables[i].attributes[j].unique << endl;
		}
	}
	file.close();
}


void CatalogManager::writeIndexes()
{
	string fileName = "Indexes.log";
	fstream file(fileName);
	file << indexNum << endl;
	//��.index��д�뵱ǰManager�е�������Ϣ
	for (int i = 0; i < indexNum; i++)
	{
		file << indexes[i].indexName << " ";
		file << indexes[i].tableName << " ";
		file << indexes[i].columnIndex << " ";
		file << indexes[i].attrLength << " ";
		file << indexes[i].blockNum << endl;
	}
	file.close();
}

void CatalogManager::createTable(const Table & table)
{
	tableNum++;
	tables.push_back(table);
}

void CatalogManager::createIndex(Index index)
{
	indexNum++;
	indexes.push_back(index);
}

//���ϲ����ʱ��ͨ��exist�ж�ָ���Ƿ�Ϸ�
//ֻ����ȷ�������table��Ϣ��ʱ��Ż����dropTabel
void CatalogManager::dropTable(string tableName)
{
	//erase the table from tables
	for (int i = 0; i < tableNum; i++) {
		if (tables[i].name == tableName) {
			tables.erase(tables.begin()+ i);
			tableNum--;
		}
	}
	//earse all the indexes of the table
	for (int i = 0; i < indexNum; i++) {
		if (indexes[i].tableName == tableName) {
			indexes.erase(indexes.begin() + i);
			indexNum--;
		}
	}
}

//���ϲ����ʱ��ͨ��exist�ж�ָ���Ƿ�Ϸ�
//ֻ����ȷ�������index��Ϣ��ʱ��Ż����dropIndex
void CatalogManager::dropIndex(string indexName)
{
	for (int i = 0; i < indexNum; i++) {
		if (indexes[i].indexName == indexName) {
			indexes.erase(indexes.begin()+i);
			indexNum--;
		}
	}
}

bool CatalogManager::tableExist(string tableName)
{
	for (int i = 0; i < tableNum; i++) {
		if (tables[i].name == tableName) {
			return true;
		}
	}
	return false;
}

bool CatalogManager::indexExist(string indexName)
{
	for (int i = 0; i < indexNum; i++) {
		if (indexes[i].indexName == indexName) {
			return true;
		}
	}
	return false;
}

Table CatalogManager::getTable(string tableName)
{
	Table result;
	for (int i = 0; i < tableNum; i++) {
		if (tables[i].name == tableName) {
			result = tables[i];
			break;
		}
	}
	return result;
}

Index CatalogManager::getIndex(string indexName)
{
	Index result;
	for (int i = 0; i < indexNum; i++) {
		if (indexes[i].indexName == indexName) {
			result = indexes[i];
			break;
		}
	}
	return result;
}
//�г����б���Ϣ�����ڵ���
void CatalogManager::listTables()
{
	cout << "All the tables are listed as: " << endl;
	for (int i = 0; i < tableNum; i++) {
		cout << "Table " << i << ":" << tables[i].name << endl;
		cout << tables[i].blockNum << " blocks" << endl;
		cout << tables[i].attriNum << " attributes" << endl;
		for (int j = 0; j < tables[i].attriNum; j++) {
			cout << tables[i].attributes[j].name << endl;
		}
	}
}
//�г�����index��Ϣ�����ڵ���
void CatalogManager::listIndexes()
{
	cout << "All the indexes are listed as:" << endl;
	for (int i = 0; i < indexNum; i++) {
		cout << "Index " << i << ":" << indexes[i].indexName << endl;
	}
}

int CatalogManager::attriIndex(Table & table, string attriName)
{
	for (int i = 0; i < table.attriNum; i++) {
		if (table.attributes[i].name == attriName)
			return i;
	}
	return -1;//-1�������ڸ�����
}

//����ʱ��Manager��Ϣд��log�ļ�
CatalogManager::~CatalogManager()
{
	writeIndexes();
	writeTables();
}