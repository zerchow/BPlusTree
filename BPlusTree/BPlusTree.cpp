#include "BPlusTree.h"
int main()
{
	BPlusTreeData leafleaf;
	BPlusTreeNode nodenode;
	cout<<sizeof(leafleaf)<<endl;
	cout<<sizeof(nodenode)<<endl;
	//return 0;

	const int DATA_NUM = 20 * 1024 *1024;
	Data * test = new Data[DATA_NUM]; //32 * 20 * 1024 * 1024 = 640 (M);
	
	for (int i = 0; i < DATA_NUM; i++)
	{
		test[i].index = 10 + i;
		strcpy(test[i].data, "test");
	}
	
	BPlusTree * tree = new BPlusTree();
	for (int i = 0; i < DATA_NUM; i++)
	{
		tree->insert(test[i]);
	}
	
	BPlusTreeNode * root = tree->getroot();
	int rootnum = root->index_num;
	for (int i = 0; i < rootnum; i++)
		cout << root->indexs[i] << " ";
	cout << endl;

	cout << "height : " << tree->getheight() << endl;
	system("pause");
	return 0;
}