#include "MiniSQL.h"
#include "BufferManager.h"
#include <iostream>

using namespace std;

int main() {
	BufferManager a;
	char c[10] = { 'a','b','c','d','e' };
	Table t("book");
	t.fileTail = 400;
	t.blockNum = 1;
	Block result;
	Block b;
	/*Block block("book", 0);
	for (int i = 0; i < 100; i++) {
		block.insertRecord(c, 4);
	}
	block.setDirty();
	a.writeBlock(block);*/
	
	result = a.readBlock(t, 0);
	result.show();

	b = a.findBlk(t, 100);
	b.show();

	system("pause");
	return 0;
}
