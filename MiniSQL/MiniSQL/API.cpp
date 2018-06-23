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
		DropTable(sqlCommand.tableInfo.name);
		break;
	case CreateIndexCmd:
		CreateIndex(sqlCommand.indexInfo);
		break;
	case DropIndexCmd:
		DropIndex(sqlCommand.indexInfo.indexName);
		break;
	case SelectCmd:
		Select(sqlCommand.tableInfo.name, sqlCommand.condList, sqlCommand.attriNameList);
		break;
	case InsertCmd:
		Insert(sqlCommand.tableInfo.name, sqlCommand.valuesList);
		break;
	case DeleteCmd:
		Delete(sqlCommand.tableInfo.name, sqlCommand.condList);
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
	//WARNING:这里的index名字和属性名相同，属于预先创建
	Index idtemp;
	int i = 0;
	for (auto elem : table.attributes) {
		if (elem.PK == true || elem.unique == true) {
			idtemp.tableName = table.name; idtemp.attributeName = elem.name;
			idtemp.type = elem.type; idtemp.indexName = elem.name;
			api->rm->indexm->createIndex(table.name, elem.name, elem.type);
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
	Table temp;
	temp = api->cm->getTable(index.tableName);
	//判断是否有这一属性
	int flag = 0;
	for (auto elem : temp.attributes) {
		if (elem.name == index.attributeName) {
			flag = 1;
		}
	}
	if (flag == 0) {
		cerr << "Attribute " << index.attributeName << " does not exist." << endl;
	}

	// check if the attribute is unique;
	for (auto elem : temp.attributes) {
		if (elem.name == index.attributeName && elem.PK == false && elem.unique == false) {
			cerr << "Attribute " << elem.name << " is not unique." << endl;
			return false;
		}
	}

	// create the index
	api->rm->indexm->createIndex(index.tableName, index.attributeName, index.type);
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
		cerr << "Table " << tableName << " does not exist." << endl;
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
		cerr << "Table " << tableName << " does not exist." << endl;
		return false;
	}

	// TODO:select and return the records( type: records )



	return true;
}

bool API::Insert(string tableName, vector<string> valuesList)
{
	// check if the table exist
	if (!api->cm->tableExist(tableName)) {
		cerr << "Table " << tableName << " does not exist." << endl;
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
