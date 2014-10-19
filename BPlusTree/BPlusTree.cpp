#include "BPlusTree.h"

int main()
{
	cout << sizeof(new Data) << endl;
	int i = 100;
	int *p1 = &i;
	int *p2 = &i;
	cout << (p1 == p2) << endl;
	cout << p1 << endl;
	cout << p2 << endl;
	return 0;
}