#include "BPlusTree.h"
int main()
{
	Data test[27]; //32 * 27 = 864 (KB);
	for (int i = 0; i < 27; i++)
	{
		test[i].index = 10 + i;
		strcpy(test[i].data, "test");
	}
	BPlusTree * tree = new BPlusTree();
	for (int i = 0; i < 27; i++)
		tree->insert(test[i]);
	
	BPlusTreeNode * root = tree->getroot();
	int rootnum = root->index_num;
	for (int i = 0; i < rootnum; i++)
		cout << root->indexs[i] << " ";
	cout << endl;
	return 0;
}