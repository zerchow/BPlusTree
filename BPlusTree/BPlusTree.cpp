#include "BPlusTree.h"
int main()
{
	BPlusTreeData leafleaf;
	BPlusTreeNode nodenode;
	cout<<sizeof(leafleaf)<<endl;
	cout<<sizeof(nodenode)<<endl;
	return 0;
	Data * test = new Data[16384000]; //32 * 27 = 864 (B);
	
	for (int i = 0; i < 16384000; i++)
	{
		test[i].index = 10 + i;
		strcpy(test[i].data, "test");
	}
	
	BPlusTree * tree = new BPlusTree();
	for (int i = 0; i < 16384000; i++)
	{
		tree->insert(test[i]);
	}
	
	BPlusTreeNode * root = tree->getroot();
	int rootnum = root->index_num;
	for (int i = 0; i < rootnum; i++)
		cout << root->indexs[i] << " ";
	cout << endl;
	system("pause");
	return 0;
}