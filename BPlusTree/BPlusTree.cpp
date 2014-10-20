#include "BPlusTree.h"

int main()
{
	Data dd = {1,"",2};
	cout<<sizeof(dd)<<endl;
	cout << sizeof(new BPlusTreeData()) << endl;
	cout << sizeof(new BPlusTreeNode()) << endl;
	int limit = 20 * 1024 * 1024;
	for(int i = 0; i < limit; i ++)
		cout<<i<<endl;
	return 0;
}