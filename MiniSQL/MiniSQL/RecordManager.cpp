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

//将row（即一条record）插入table对应的最后一个block（在buffer中而非直接在文件中）
void RecordManager::insertRecord(Table & table, Row & row)
{
	Block &blockptr = bufferm->findBlk(table, table.fileTail);
	char temp[BLOCK_SIZE];
	int offset = 0;
	//把row中的内容按照attribute的类型和顺序译为char数组
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
	//在现有的块中insert
	blockptr.insertRecord(temp, table.recLength);
	blockptr.isDirty = true;
	bufferm->allWrite(); //test
	return;
}

//先根据cond找出块对应的offset（调用Index）再将record的valid byte置0(在buffer中)
//同时在indexmanager将其有index的remove掉
void RecordManager::deleterecord(const Table & table, const vector<Condition> cond)
{
	/* 这里condition都是and连接，找到满足条件的一系列offset */
	set<int > temp;	//找出目标的offsets存于set<int > temp
	Row row;
	Block blockptr;
	indexm->SearchResult.clear();

	for (int i = 0; i < cond.size(); i++) {
		indexm->SearchResult.clear();
		//在这一条件的属性为PK或unique时可以通过IndexManager索引查找
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
			//如果是第一个条件取并集，否则取交集
			if (i == 0) { 
				temp = indexm->SearchResult;
				//set_union(temp.begin(), temp.end(), indexm->SearchResult.begin(), indexm->SearchResult.end(),temp.begin());//注意第五个参数的形式
			}
			else {
				auto iter = set_intersection(temp.begin(), temp.end(), indexm->SearchResult.begin(), indexm->SearchResult.end(), inserter(temp, temp.begin()));
			}
		}
		else { //条件的属性为一般属性，遍历读取.dat
			for (int j = 0; j < table.blockNum; j++) {
				blockptr = bufferm->readBlock(table, j);
				for (int byte = 0; byte < BLOCK_SIZE; byte += table.recLength) {
					if (j*BLOCK_SIZE + byte >= table.fileTail)
						break;
					row = byte2row(table, blockptr.content + byte);
					if (Comparator(table, row, cond) == true) { //符合条件，应该被删除
						memset(blockptr.content + byte, 0, 1); //将valid位置0
					}
				}
			}
			return ; //在循环的第一次就return了，因为遍历做的是多条件的判断
		}
	}

	//读取Blocks并在buffermanager中把这些offset的valid byte都置0
	//由IndexManager找到的offset直接就是valid byte所在的位置
	for (auto elem : temp) {
		int offrecord  = 0;
		blockptr = bufferm->findBlk(table, elem);
		offrecord = elem % BLOCK_SIZE;
		memset(blockptr.content + offrecord, 0, 1);
	}
	return ;
}

//select将buffer中满足条件的record选出并返回已经转为string的records
Records& RecordManager::selectRecord(const Table & table, const vector<Condition> cond)
{
	Records result;
	Row row;

	set<int > temp;	//找出目标的offsets存于set<int > temp
	indexm->SearchResult.clear();

	for (int i = 0; i < cond.size(); i++) {
		indexm->SearchResult.clear();
		//在这一条件的属性为PK或unique时可以通过IndexManager索引查找
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
			//如果是第一个条件取并集，否则取交集
			if (i == 0) {
				temp = indexm->SearchResult;
			}
			else {
				auto iter = set_intersection(temp.begin(), temp.end(), indexm->SearchResult.begin(), indexm->SearchResult.end(), inserter(temp,temp.begin()));
			}
		}
		else { //条件的属性为一般属性，遍历读取.dat
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
			return result; //在循环的第一次就return了，因为遍历做的是多条件的判断
		}
	}

	//由IndexManager找到的offset直接就是valid byte所在的位置
	for (auto elem : temp) {
		int offrecord = 0;
		Block &blockptr = bufferm->findBlk(table, elem);
		offrecord = elem % BLOCK_SIZE;
		row = byte2row(table, blockptr.content + offrecord);
	}
	return result;
}

/*---------------------------辅助函数-------------------------------*/

//将以字节表示的数据转换为row（string）的形式 done
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

//把一行row转换为byte数据 done
char * RecordManager::row2byte(Table table, Row row)
{
	char result[BLOCK_SIZE];
	memset(result, 0, BLOCK_SIZE);
	int offset = 0;
	
	//把row中的内容按照attribute的类型和顺序译为char数组
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

//用于一行record（string）和条件的判断
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