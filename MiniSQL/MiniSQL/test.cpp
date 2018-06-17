#include "MiniSQL.h"
#include "BufferManager.h"
#include <iostream>

using namespace std;

int main() {
	BufferManager a("bookstore");
	Block b("book", 0);
	b.setContent((char*)("666"), 3);

	a.writeBlock(b);
	a.allWrite();
	system("pause");
	return 0;
}
