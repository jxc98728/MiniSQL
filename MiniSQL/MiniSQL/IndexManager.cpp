#include "IndexManager.h"
using namespace std;


IndexManager::IndexManager(){}

IndexManager::~IndexManager()
{
	writeBackToDisk();
}

void IndexManager::writeBackToDisk(void)
{
	string file_path;

	fstream file;

	for (map<string, int>::iterator int_Iter = intTreeList.begin(); int_Iter != intTreeList.end(); int_Iter++)
	{
		file_path = int_Iter->first + ".isf";

		file.open(file_path, ios::out | ios::in);

		BPlusTreeNode<int>* p = int_treelist[int_Iter->second - 1].getHeadNode();

		while (p != nullptr) {
			for (int i = 0; i < p->cnt; i++)
				file << p->keys[i] << " " << p->keyOffset[i] << endl;
			p = p->sibling;
		}

		file.close();
	}

	for (map<string, int>::iterator flt_Iter = fltTreeList.begin(); flt_Iter != fltTreeList.end(); flt_Iter++)
	{
		file_path = flt_Iter->first + ".isf";

		file.open(file_path, ios::out | ios::in);

		BPlusTreeNode<float>* p = flt_treelist[flt_Iter->second - 1].getHeadNode();

		while (p != nullptr) {
			for (int i = 0; i < p->cnt; i++)
				file << p->keys[i] << " " << p->keyOffset[i] << endl;
			p = p->sibling;
		}

		file.close();
	}

	for (map<string, int>::iterator str_Iter = strTreeList.begin(); str_Iter != strTreeList.end(); str_Iter++)
	{
		file_path = str_Iter->first + ".isf";

		file.open(file_path, ios::out | ios::in);

		BPlusTreeNode<string> * p = str_treelist[str_Iter->second - 1].getHeadNode();

		while (p != nullptr) {
			for (int i = 0; i < p->cnt; i++)
				file << p->keys[i] << " " << p->keyOffset[i] << endl;
			p = p->sibling;
		}

		file.close();
	}
}

bool IndexManager::createIndex(const string & tableName, const string & indexName, int type)
{
	string file_path = tableName + "#" + indexName + ".isf";

	fstream file;

	string line;

	file.open(file_path, ios::in);

	if (file)
	{
		if (type == 0)
		{
			BPlusTree<int> * ptrTree = new BPlusTree<int>(sizeof(int), 4092 / (sizeof(int) + 4));	
			while (!file.eof())
			{
				getline(file, line);
				if (line == "")
					continue;
				int p = line.find(" ");
				(*ptrTree).insert(atoi((line.substr(0, p).c_str())), atoi(line.substr(p + 1).c_str()));
			}

			int_treelist.push_back(*ptrTree);
			intTreeList[tableName + "#" + indexName] = int_treelist.size();
		}
		else if (type == -1)
		{
			BPlusTree<float> * ptrTree = new BPlusTree<float>(sizeof(float), 4092 / (sizeof(float) + 4));
			
			while (!file.eof())
			{
				getline(file, line);
				if (line == "")
					continue;
				int p = line.find(" ");
				(*ptrTree).insert(atof((line.substr(0, p).c_str())), atof(line.substr(p + 1).c_str()));
			}

			flt_treelist.push_back(*ptrTree);
			fltTreeList[tableName + "#" + indexName] = flt_treelist.size();

		}
		else
		{
			BPlusTree<string> * ptrTree = new BPlusTree<string>(type, 4092 / (type + 4));
			
			while (!file.eof())
			{
				getline(file, line);
				if (line == "")
					continue;
				int p = line.find(" ");
				(*ptrTree).insert(line.substr(0, p), atoi(line.substr(p + 1).c_str()));
			}

			str_treelist.push_back(*ptrTree);
			strTreeList[tableName + "#" + indexName] = str_treelist.size();
		}
	}
	else
	{
		file.open(file_path, ios::out);
		if (type == 0)
		{
			BPlusTree<int> * ptrTree = new BPlusTree<int>(sizeof(int), 4092 / (sizeof(int) + 4));
			int_treelist.push_back(*ptrTree);
			intTreeList[tableName + "#" + indexName] = int_treelist.size();
		}
		else if (type == -1)
		{
			BPlusTree<float> * ptrTree = new BPlusTree<float>(sizeof(float), 4092 / (sizeof(float) + 4));
			flt_treelist.push_back(*ptrTree);
			fltTreeList[tableName + "#" + indexName] = flt_treelist.size();
		}
		else
		{
			BPlusTree<string> * ptrTree = new BPlusTree<string>(type, 4092 / (type + 4));	
			str_treelist.push_back(*ptrTree);
			strTreeList[tableName + "#" + indexName] = str_treelist.size();
		}
	}
	
	file.close();
	return true;
}

// ��tableName�ı���ɾ����indexNameΪkey��index, ��ʧ�ܻ�index��������return false.
bool IndexManager::dropIndex(const string tableName, const string & indexName, int type)
{
	int tree_id;


	// ���Ҷ�Ӧ�� tree_id

	switch (type)
	{
		case 0: 
		{
			tree_id = intTreeList[tableName + "#" + indexName];
			
			if (tree_id)
			{
				intTreeList[tableName + "#" + indexName] = 0;
				int_treelist.erase(int_treelist.begin() + tree_id - 1);
				return true;
			}
			else {
				return false;
			}
			break;
		}

		case -1: 
		{
			tree_id = fltTreeList[tableName + "#" + indexName];

			if (tree_id)
			{
				fltTreeList[tableName + "#" + indexName] = 0;
				flt_treelist.erase(flt_treelist.begin() + tree_id - 1);
				return true;
			}
			else {
				return false;
			}
			break;
		}

		default: 
		{
			tree_id = strTreeList[tableName + "#" + indexName];

			if (tree_id)
			{
				strTreeList[tableName + "#" + indexName] = 0;
				str_treelist.erase(str_treelist.begin() + tree_id - 1);
				return true;
			}
			else {
				return false;
			}
			break;
		}
		
	}

}

// �ڽ���insert����ʱ�Զ�Ӧ��B+�����в�����£�Ϊ�˷��㣬keyֵͳһ��string���ݣ�����catalog�е�type������Ӧ��ת����offset�ǰ�����key��record���ļ��е�ƫ������
bool IndexManager::insert(const string tableName, const string & indexName, const string & key, int type, int offset)
{
	int tree_id;
	
	switch (type)
	{
	case 0:
	{
		tree_id = intTreeList[tableName + "#" + indexName];

		if (tree_id)
			return int_treelist[tree_id - 1].insert(atoi(key.c_str()), offset);
		else
			return false;
		break;
	}

	case -1:
	{
		tree_id = fltTreeList[tableName + "#" + indexName];

		if (tree_id)
			return flt_treelist[tree_id - 1].insert(atoi(key.c_str()), offset);
		else
			return false;
		break;
	}

	default:
	{
		tree_id = strTreeList[tableName + "#" + indexName];

		if (tree_id)
			return str_treelist[tree_id - 1].insert(key, offset);
		else
			return false;
		break;
	}
	}

}

// �ڽ���delete����ʱ�Զ�Ӧ��B+�����и��£�Ϊ�˷��㣬keyֵͳһ��string���ݣ�����catalog��type������Ӧ��ת��
// ע�⣺��ɾ�����м�ֵΪkey���
bool IndexManager::remove(const string tableName, const string & indexName, const string & key, int type)
{
	int tree_id;
	switch (type)
	{
	case 0:
	{
		tree_id = intTreeList[tableName + "#" + indexName];

		if (tree_id)
			return int_treelist[tree_id - 1].remove(atoi(key.c_str()));
		else
			return false;
		break;
	}

	case -1:
	{
		tree_id = fltTreeList[tableName + "#" + indexName];

		if (tree_id)
			return flt_treelist[tree_id - 1].remove(atof(key.c_str()));
		else
			return false;
		break;
	}

	default:
	{
		tree_id = strTreeList[tableName + "#" + indexName];

		if (tree_id)
			return str_treelist[tree_id - 1].remove(key);
		else
			return false;
		break;
	}
	}
}


/* ��ֵ���ң�����ִ�� ��=�� �����
1. ��tableName�ı���indexNameΪ����ֵ��B+���в��Ҽ�ֵΪkey��������, ����ֵΪ��ѯ����ĸ�����
2. ��ѯ�����ƥ���ֵ����record���ļ��е�ƫ����������ȫ��set����SearchResult�У�������MiniSQL.cpp�У���
ִ��ʱ�������SearchResult���飨�����������ѯ������Ҫ�������ڲ���ǰ������ա�
3. type��key����������
#0: int
#-1: float
#n: char(n)
*/
int IndexManager::selectEqual(const string tableName, const string & indexName, const string & key, int type)
{
	int tree_id, temp_offset;

	switch (type)
	{
	case 0:
	{
		tree_id = intTreeList[tableName + "#" + indexName];

		if (tree_id) 
		{
			temp_offset = int_treelist[tree_id - 1].find(atoi(key.c_str()));
			if (temp_offset >= 0) {
				SearchResult.insert(temp_offset);
				return 1;
			}
		}
		else
			return 0;
		break;
	}

	case -1:
	{
		tree_id = fltTreeList[tableName + "#" + indexName];

		if (tree_id)
		{
			temp_offset = flt_treelist[tree_id - 1].find(atof(key.c_str()));
			if (temp_offset >= 0) {
				SearchResult.insert(temp_offset);
				return 1;
			}
		}
		else
			return 0;
		break;
	}

	default:
	{
		tree_id = strTreeList[tableName + "#" + indexName];

		if (tree_id)
		{
			temp_offset = str_treelist[tree_id - 1].find(key);
			if (temp_offset >= 0) {
				SearchResult.insert(temp_offset);
				return 1;
			}
		}
		else
			return 0;
		break;
	}
	}
}

/* ����ֵ���ң�����ִ�� ��<>�� �����
1. ��tableName�ı���indexNameΪ����ֵ��B+���в��Ҽ�ֵ��Ϊkey��������, ����ֵΪ��ѯ����ĸ�����
2. ��ѯ�����ƥ���ֵ����record���ļ��е�ƫ����������ȫ��set����SearchResult�У�������MiniSQL.cpp�У�
ִ��ʱ�������SearchResult���飨�����������ѯ������Ҫ�������ڲ���ǰ������ա�
3. type��key����������
#0: int
#-1: float
#n: char(n)
*/
int IndexManager::selectInequal(const string tableName, const string & indexName, const string & key, int type)
{
	int tree_id, count = 0;

	switch (type)
	{
	case 0:
	{
		tree_id = intTreeList[tableName + "#" + indexName];

		if (tree_id)
		{
			BPlusTreeNode<int>* q = int_treelist[tree_id - 1].getHeadNode();
			while (q != nullptr) {
				for (int i = 0; i < q->cnt; i++)
				{
					if (q->keys[i] != atoi(key.c_str()))
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
				}
				q = q->sibling;
			}
		}
		break;
	}

	case -1:
	{
		tree_id = fltTreeList[tableName + "#" + indexName];

		if (tree_id)
		{
			BPlusTreeNode<float>* q = flt_treelist[tree_id - 1].getHeadNode();
			while (q != nullptr) {
				for (int i = 0; i < q->cnt; i++)
				{
					if (q->keys[i] != atof(key.c_str()))
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
				}
				q = q->sibling;
			}
		}
		break;
	}

	default:
	{
		tree_id = strTreeList[tableName + "#" + indexName];

		if (tree_id)
		{
			BPlusTreeNode<string>* q = str_treelist[tree_id - 1].getHeadNode();
			while (q != nullptr) {
				for (int i = 0; i < q->cnt; i++)
				{
					if (q->keys[i] != key)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
				}
				q = q->sibling;
			}
		}
		break;
	}
	}
	return count;
}

/* ��������(Range Query)������ִ�� ��>, < <=, >=�� �����
1. ��tableName�ı���indexNameΪ����ֵ��B+���в��ҷ��ϼ�ֵ����������ϵ��������, ���硰 key >= 1234 ����
����ֵΪ��ѯ����ĸ�����
2. ��ѯ�����ƥ���ֵ����record���ļ��е�ƫ����������ȫ��set����SearchResult�У�������MiniSQL.cpp�У�
ִ��ʱ�������SearchResult���飨�����������ѯ������Ҫ�������ڲ���ǰ������ա�
3. type��key���������ͣ�
#0: int
#-1: float
#n: char(n)
4. opcode�ǱȽϷ����ͣ�
#0: >
#1: <
#2: >=
#3. <=
#4: =
#5: <>
5. cond������ֵ
*/
int IndexManager::selectRange(const string tableName, const string & indexName, const string & cond, int opcode, int type)
{
	int tree_id, count = 0;

	switch (type)
	{
	case 0:
	{
		tree_id = intTreeList[tableName + "#" + indexName];

		if (tree_id)
		{
			BPlusTreeNode<int>* q = int_treelist[tree_id - 1].getHeadNode();
			while (q != nullptr) {
				for (int i = 0; i < q->cnt; i++)
				{
					if (q->keys[i] > atoi(cond.c_str()) && opcode == 0)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
					if (q->keys[i] < atoi(cond.c_str()) && opcode == 1)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
					if (q->keys[i] >= atoi(cond.c_str()) && opcode == 2)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
					if (q->keys[i] <= atoi(cond.c_str()) && opcode == 3)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
					if (q->keys[i] == atoi(cond.c_str()) && opcode == 4)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
					if (q->keys[i] != atoi(cond.c_str()) && opcode == 5)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
				}
				q = q->sibling;
			}
		}
		break;
	}

	case -1:
	{
		tree_id = fltTreeList[tableName + "#" + indexName];

		if (tree_id)
		{
			BPlusTreeNode<float>* q = flt_treelist[tree_id - 1].getHeadNode();
			while (q != nullptr) {
				for (int i = 0; i < q->cnt; i++)
				{
					if (q->keys[i] > atof(cond.c_str()) && opcode == 0)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
					if (q->keys[i] < atof(cond.c_str()) && opcode == 1)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
					if (q->keys[i] >= atof(cond.c_str()) && opcode == 2)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
					if (q->keys[i] <= atof(cond.c_str()) && opcode == 3)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
					if (q->keys[i] == atof(cond.c_str()) && opcode == 4)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
					if (q->keys[i] != atof(cond.c_str()) && opcode == 5)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
				}
				q = q->sibling;
			}
		}
		break;
	}

	default:
	{
		tree_id = strTreeList[tableName + "#" + indexName];

		if (tree_id)
		{
			BPlusTreeNode<string>* q = str_treelist[tree_id - 1].getHeadNode();
			while (q != nullptr) {
				for (int i = 0; i < q->cnt; i++)
				{
					if (q->keys[i] > cond && opcode == 0)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
					if (q->keys[i] < cond && opcode == 1)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
					if (q->keys[i] >= cond && opcode == 2)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
					if (q->keys[i] <= cond && opcode == 3)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
					if (q->keys[i] == cond && opcode == 4)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
					if (q->keys[i] != cond && opcode == 5)
					{
						SearchResult.insert(q->keyOffset[i]);
						count++;
					}
				}
				q = q->sibling;
			}
		}
		break;
	}
	}
	return count;
}