#include "RecordManager.h"
#include <string>
#include <iostream>
using namespace std;

//现阶段测试只涉及BufferManager和RecordManager，如果RecordManager能跑对就好办了
int main() {
	IndexManager im;
	BufferManager bm;
	RecordManager rm(&bm, &im);

	Attribute a1("sid", 10, 1, 1);
	Attribute a2("name", 8, 0, 0);
	Attribute a3("age", 0, 0, 0);
	vector<Attribute> as = { a1, a2,a3 };

	Table t1("student", 3, as);
	t1.recLength = 22; //TODO:要在interpreter构造table时手动赋值
	rm.createTable("student"); //done
	//rm.dropTable("student"); //done

	//testInsert
	Row row1,row2,row3;
	Condition c1;
	Records rst;
	c1.attriName = "name";
	c1.columnNum = 1;
	c1.cond = "Jack";
	c1.opcode = Eq;
	vector<Condition> c = { c1 };
	row1.columns = { "111111111","Jack","18"};
	t1.fileTail = 1; //test,注意要在插入中修改
	
	rm.insertRecord(t1, row1);

	rst = rm.selectRecord(t1, c);

	system("pause");
	return 0;
}