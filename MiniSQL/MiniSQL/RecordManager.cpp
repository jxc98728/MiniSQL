#include "RecordManager.h"
#include "MiniSQL.h"

void RecordManager::createTable(string tableName)
{
	string fileName = tableName + ".dat";
	ofstream file(fileName);
	file.close();
	return;
}

void RecordManager::dropTable(string tableName)
{
	string fileName = tableName + ".dat";
	remove(fileName.c_str());
	return ;
}

//��row����һ��record������table��Ӧ�����һ��block����buffer�ж���ֱ�����ļ��У�
void RecordManager::insertRecord(Table & table, Row & row)
{
	Block &blockptr = bufferm->findBlk(table, table.fileTail);
	char temp[BLOCK_SIZE];
	int offset = 0;
	//��row�е����ݰ���attribute�����ͺ�˳����Ϊchar����
	memset(temp, 0, BLOCK_SIZE);
	memcpy(temp, row2byte(table, row), table.recLength + 1);
	if (blockptr.size + table.recLength + 1 > BLOCK_SIZE) {
		Block newBlock(table.name, table.blockNum);
		table.blockNum += 1;
		table.fileTail += BLOCK_SIZE * (table.blockNum - 1) + table.recLength + 1;
		newBlock.insertRecord(temp,table.recLength);
		bufferm->block2buf(blockptr);
		return;
	}
	//�����еĿ���insert
	blockptr.insertRecord(temp, table.recLength);
	blockptr.isDirty = true;
	bufferm->allWrite(); //test
	return;
}

//�ȸ���cond�ҳ����Ӧ��offset������Index���ٽ�record��valid byte��0(��buffer��)
//ͬʱ��indexmanager������index��remove��
void RecordManager::deleterecord(const Table & table, const vector<Condition> cond)
{
	/* ����condition����and���ӣ��ҵ�����������һϵ��offset */
	set<int > temp;	//�ҳ�Ŀ���offsets����set<int > temp
	Row row;
	Block blockptr;
	indexm->SearchResult.clear();

	for (int i = 0; i < cond.size(); i++) {
		indexm->SearchResult.clear();
		//����һ����������ΪPK��uniqueʱ����ͨ��IndexManager��������
		if (table.attributes[cond[i].columnNum].PK == true || table.attributes[cond[i].columnNum].unique == true) {
			if (cond[i].opcode == 4) { //==
				indexm->selectEqual(table.name, cond[i].attriName, cond[i].cond, table.attributes[cond[i].columnNum].type);
			}
			else if (cond[i].opcode == 5) { //!=
				indexm->selectInequal(table.name, cond[i].attriName, cond[i].cond, table.attributes[cond[i].columnNum].type);
			}
			else {
				indexm->selectRange(table.name, cond[i].attriName, cond[i].cond, cond[i].opcode, table.attributes[cond[i].columnNum].type);
			}
			//����ǵ�һ������ȡ����������ȡ����
			if (i == 0) { 
				temp = indexm->SearchResult;
				//set_union(temp.begin(), temp.end(), indexm->SearchResult.begin(), indexm->SearchResult.end(),temp.begin());//ע��������������ʽ
			}
			else {
				auto iter = set_intersection(temp.begin(), temp.end(), indexm->SearchResult.begin(), indexm->SearchResult.end(), inserter(temp, temp.begin()));
			}
		}
		else { //����������Ϊһ�����ԣ�������ȡ.dat
			for (int j = 0; j < table.blockNum; j++) {
				blockptr = bufferm->readBlock(table, j);
				for (int byte = 0; byte < BLOCK_SIZE; byte += table.recLength) {
					if (j*BLOCK_SIZE + byte >= table.fileTail)
						break;
					row = byte2row(table, blockptr.content + byte);
					if (Comparator(table, row, cond) == true) { //����������Ӧ�ñ�ɾ��
						memset(blockptr.content + byte, 0, 1); //��validλ��0
					}
				}
			}
			return ; //��ѭ���ĵ�һ�ξ�return�ˣ���Ϊ���������Ƕ��������ж�
		}
	}

	//��ȡBlocks����buffermanager�а���Щoffset��valid byte����0
	//��IndexManager�ҵ���offsetֱ�Ӿ���valid byte���ڵ�λ��
	for (auto elem : temp) {
		int offrecord  = 0;
		blockptr = bufferm->findBlk(table, elem);
		offrecord = elem % BLOCK_SIZE;
		memset(blockptr.content + offrecord, 0, 1);
	}
	return ;
}

//select��buffer������������recordѡ���������Ѿ�תΪstring��records
Records& RecordManager::selectRecord(const Table & table, const vector<Condition> cond)
{
	Records result;
	Row row;

	set<int > temp;	//�ҳ�Ŀ���offsets����set<int > temp
	indexm->SearchResult.clear();

	for (int i = 0; i < cond.size(); i++) {
		indexm->SearchResult.clear();
		//����һ����������ΪPK��uniqueʱ����ͨ��IndexManager��������
		if (table.attributes[cond[i].columnNum].PK == true || table.attributes[cond[i].columnNum].unique == true) {
			if (cond[i].opcode == 4) { //==
				indexm->selectEqual(table.name, cond[i].attriName, cond[i].cond, table.attributes[cond[i].columnNum].type);
			}
			else if (cond[i].opcode == 5) { //!=
				indexm->selectInequal(table.name, cond[i].attriName, cond[i].cond, table.attributes[cond[i].columnNum].type);
			}
			else {
				indexm->selectRange(table.name, cond[i].attriName, cond[i].cond, cond[i].opcode, table.attributes[cond[i].columnNum].type);
			}
			//����ǵ�һ������ȡ����������ȡ����
			if (i == 0) {
				temp = indexm->SearchResult;
			}
			else {
				auto iter = set_intersection(temp.begin(), temp.end(), indexm->SearchResult.begin(), indexm->SearchResult.end(), inserter(temp,temp.begin()));
			}
		}
		else { //����������Ϊһ�����ԣ�������ȡ.dat
			for (int j = 0; j < table.blockNum; j++) {
				Block blockptr = bufferm->readBlock(table, j);
				blockptr.show();
				for (int byte = 0; byte < BLOCK_SIZE; byte += table.recLength) {
					if (j * BLOCK_SIZE + byte >= table.fileTail)
						break;
					if (blockptr.content[byte] == 0) // has deleted
						continue; 
					row = byte2row(table, blockptr.content + byte);
					if (Comparator(table, row, cond) == true) {
						result.rows.push_back(row);
					}
				}
			}
			return result; //��ѭ���ĵ�һ�ξ�return�ˣ���Ϊ���������Ƕ��������ж�
		}
	}

	//��IndexManager�ҵ���offsetֱ�Ӿ���valid byte���ڵ�λ��
	for (auto elem : temp) {
		int offrecord = 0;
		Block &blockptr = bufferm->findBlk(table, elem);
		offrecord = elem % BLOCK_SIZE;
		row = byte2row(table, blockptr.content + offrecord);
	}
	return result;
}

/*---------------------------��������-------------------------------*/

//�����ֽڱ�ʾ������ת��Ϊrow��string������ʽ done
Row RecordManager::byte2row(Table table, char * src)
{
	Row row;
	string temp;
	src += 1; //overread the valid byte
	for (int i = 0; i < table.attriNum; i++) {
		if (table.attributes[i].type == 0) { //INT
			temp = to_string(char2int(src));
			src += 4;
		}
		else if(table.attributes[i].type == -1){ //FLOAT
			temp = to_string(char2float(src));
			src += 4;
		}
		else { //CHAR(n)
			temp = string(src, table.attributes[i].type);
			src += table.attributes[i].type;
		}
		row.columns.push_back(temp);
	}
	return row;
}

//��һ��rowת��Ϊbyte���� done
char * RecordManager::row2byte(Table table, Row row)
{
	char result[BLOCK_SIZE];
	memset(result, 0, BLOCK_SIZE);
	int offset = 0;
	
	//��row�е����ݰ���attribute�����ͺ�˳����Ϊchar����
	for (int i = 0; i < table.attriNum; i++) {
		if (table.attributes[i].type == 0) { //int
			memcpy(result + offset, string2int(row.columns[i]), 4);
			offset += 4;
		}
		else if (table.attributes[i].type == -1) {// float
			memcpy(result + offset, string2float(row.columns[i]), 4);
			offset += 4;
		}
		else {
			memcpy(result +offset, row.columns[i].c_str(), table.attributes[i].type);
			offset += table.attributes[i].type; //char(n)
		}
	}
	return result;
}

//����һ��record��string�����������ж�
bool RecordManager::Comparator(Table table, Row row, vector<Condition> conditions)
{
	for (int i = 0; i<conditions.size(); i++) {
		int colNum = conditions[i].columnNum;
		string value1 = row.columns[colNum];
		string value2 = conditions[i].cond;
		int int1 = atoi(value1.c_str());
		int int2 = atoi(value2.c_str());
		float fvalue1 = atof(value1.c_str());
		float fvalue2 = atof(value2.c_str());
		switch (table.attributes[colNum].type)
		{
		case 0: //INT
			switch (conditions[i].opcode)
			{
			case Lt:
				if (int1 >= int2) return 0;
				break;
			case Le:
				if (int1 > int2) return 0;
				break;
			case Gt:
				if (int1 <= int2) return 0;
				break;
			case Ge:
				if (int1 < int2) return 0;
				break;
			case Eq:
				if (int1 != int2) return 0;
				break;
			case Ne:
				if (int1 == int2) break;
				break;
			}
			break;
		case -1: // FLOAT
			switch (conditions[i].opcode)
			{
			case Lt:
				if (fvalue1 >= fvalue2) return 0;
				break;
			case Le:
				if (fvalue1 > fvalue2) return 0;
				break;
			case Gt:
				if (fvalue1 <= fvalue2) return 0;
				break;
			case Ge:
				if (fvalue1 < fvalue2) return 0;
				break;
			case Eq:
				if (fvalue1 != fvalue2) return 0;
				break;
			case Ne:
				if (fvalue1 == fvalue2) return 0;
				break;
			}
		default: //CHAR(n)
			switch (conditions[i].opcode)
			{
			case Lt:
				if (value1 >= value2) return 0;
				break;
			case Le:
				if (value1 > value2) return 0;
				break;
			case Gt:
				if (value1 <= value2) return 0;
				break;
			case Ge:
				if (value1 < value2) return 0;
				break;
			case Eq:
				if (value1 != value2) return 0;
				break;
			case Ne:
				if (value1 == value2) return 0;
				break;
			}
			break;
		}
	}
	return 1;
}