#include "API.h"
using namespace std;
//全局的static对象
API *API::api = new API();

//construct the API class with calling the managers
API::API()
{
	auto bm = new BufferManager;
	auto im = new IndexManager;
	this->rm = new RecordManager(bm, im);
	this->cm = new CatalogManager;

	// 对存储在log中的每个index rebuild B+ tree（在内存中）
	for (auto elem : cm->indexes) {
		im->createIndex(elem.tableName, elem.indexName, elem.type);
	}
}

//free the mamagers
API::~API()
{
	delete api;
	delete rm->bufferm;
	delete rm->indexm;
	delete rm;
	delete cm;
}

//Execute为唯一对上Interpreter负责的函数，这里注意要添加sqlCommand的变量
bool API::Execute(SQLCommand sqlCommand)
{
	switch (sqlCommand.opName)
	{
	case CreateTableCmd:
		CreateTable(sqlCommand.tableInfo);
		break;
	case DropTableCmd:
		DropTable(sqlCommand.tableName);
		break;
	case CreateIndexCmd:
		CreateIndex(sqlCommand.indexInfo);
		break;
	case DropIndexCmd:
		DropIndex(sqlCommand.indexName);
		break;
	case SelectCmd:
		Select(sqlCommand.tableName, sqlCommand.condList, sqlCommand.attriNameList);
		break;
	case InsertCmd:
		Insert(sqlCommand.tableName, sqlCommand.valuesList);
		break;
	case DeleteCmd:
		Delete(sqlCommand.tableName, sqlCommand.condList);
		break;
	default:
		break;
	}
	return true;
}

bool API::CreateTable(Table table)
{
	// check if the table exists
	if (api->cm->tableExist(table.name)) {
		cerr << "Table " << table.name << " has already existed." << endl;
		return false;
	}

	//create the table
	api->cm->createTable(table);
	api->rm->createTable(table.name);

	//对表中的primary key和unique直接创建index
	Index idtemp;
	int i = 0;
	for (auto elem : table.attributes) {
		if (elem.PK == true || elem.unique == true) {
			idtemp.tableName = table.name; idtemp.indexName = elem.name;
			idtemp.type = elem.type; idtemp.columnIndex = i;
			idtemp.blockNum = table.blockNum;
			api->cm->createIndex(idtemp);
		}
		i++;
	}
	api->cm->writeIndexes();
	return true;
}

bool API::CreateIndex(Index index)
{
	// check if the table exist
	if (api->cm->tableExist(index.tableName)) {
		cerr << "Table " << index.tableName << " does not exist." << endl;
		return false;
	}

	// check if the index exist
	if (api->cm->indexExist(index.indexName)) {
		cerr << "Index " << index.indexName << " exists." << endl;
		return false;
	}

	//TODO:判断是否有这一属性由Interpreter完成
	// check if the attribute is unique
	Table temp;
	temp = api->cm->getTable(index.tableName);
	if (temp.attributes[index.columnIndex].unique == false && temp.attributes[index.columnIndex].PK == false) {
		cerr << "Attribute " << temp.attributes[index.columnIndex].name << " is not unique." << endl;
	}

	// create the index
	api->cm->createIndex(index);
	api->cm->writeIndexes();

	return true;
}

bool API::DropTable(string tableName)
{
	// check if the table exists
	if (!api->cm->tableExist(tableName)) {
		std::cerr << "Table " << tableName << " does not exist." << std::endl;
		return false;
	}

	// delete the table
	Table &tableInfo = api->cm->getTable(tableName);
	api->cm->dropTable(tableName); // includes DeleteIndex
	api->cm->writeTables();
	return true;
}

bool API::DropIndex(string indexName)
{
	// check if the index exists
	Index &index = api->cm->getIndex(indexName);
	if (index.tableName.empty()) {
		cerr << "Index " << indexName << " does not exist." << endl;
		return false;
	}

	// delete the index
	api->cm->dropIndex(indexName);
	api->cm->writeIndexes();
	return false;
}

bool API::Delete(string tableName, vector<Condition> condList)
{
	// check if the table exist
	Table &table = api->cm->getTable(tableName);
	if (!api->cm->tableExist(tableName)) {
		cerr << "Table " << tableName << " does not exist" << endl;
		return false;
	}

	// delete the records
	api->rm->deleterecord(table, condList);
	return true;
}

bool API::Select(string tableName, vector<Condition> condList, vector<string> attrList)
{
	// check if the table exist
	if (!api->cm->tableExist(tableName)) {
		cerr << "Table " << tableName << " does not exist" << endl;
		return false;
	}

	// TODO:select and return the records( type: records )

	return true;
}

bool API::Insert(string tableName, vector<string> valuesList)
{
	// check if the table exist
	if (!api->cm->tableExist(tableName)) {
		cerr << "Table " << tableName << " does not exist" << endl;
		return false;
	}

	// check if the valueList are valid
	Table &tableInfo = api->cm->getTable(tableName);
	if (tableInfo.attributes.size() != valuesList.size()) {
		cerr << "Invalid size of value list." << endl; //属性数目不同
		return false;
	}
	//check primarykey or unique
	for (int i = 0; i < tableInfo.attributes.size(); i++) {
		if (tableInfo.attributes[i].unique == true || tableInfo.attributes[i].PK == true) {
			if (api->rm->indexm->selectEqual(tableInfo.name, tableInfo.attributes[i].name,
				valuesList[i], tableInfo.attributes[i].type) > 0) {
				cerr << "Attribute " << tableInfo.attributes[i].name << " is unique." << endl;
				return false;
			}
		}
	}
	
	// insert the valueList as the record in the table
	Row row;
	for (int i = 0; i < valuesList.size(); i++) {
		row.columns.push_back(valuesList[i]);
	}
	api->rm->insertRecord(tableInfo, row);

	return true;
}

bool API::Exit()
{
	delete api;
	return true;
}
