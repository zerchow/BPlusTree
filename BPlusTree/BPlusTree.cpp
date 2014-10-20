#include "BPlusTree.h"

int main()
{
	BPlusTree * tree = new BPlusTree();
	Data test1 = {10,""};
	Data test2 = {11,""};
	Data test3 = {12,""};
	Data test4 = {13,""};
	Data test5 = {14,""};
	Data test6 = {15,""};
	Data test7 = {16,""};
	Data test8 = {17,""};
	Data test9 = {18,""};
	Data test10 = {19,""};
	Data test11 = {20,""};
	Data test12 = {21,""};
	Data test13 = {22,""};
	Data test14 = {23,""};
	Data test15 = {24,""};
	Data test16 = {25,""};
	Data test17 = {26,""};
	Data test18 = {27,""};
	tree->insert(test1);
	tree->insert(test2);
	tree->insert(test3);
	tree->insert(test4);
	tree->insert(test5);
	tree->insert(test6);
	tree->insert(test7);
	tree->insert(test8);
	tree->insert(test9);
	tree->insert(test10);
	tree->insert(test11);
	tree->insert(test12);
	tree->insert(test13);
	tree->insert(test14);
	tree->insert(test15);
	tree->insert(test16);
	tree->insert(test17);
	tree->insert(test18);
	BPlusTreeNode * root = tree->getroot();
	int rootnum = root->index_num;
	for (int i = 0; i < rootnum; i++)
		cout << root->indexs[i] << " ";
	cout << endl;
	for(int i = 0; i < rootnum; i ++)
	{
		BPlusTreeNode * node = (BPlusTreeNode*)root->pointers[i];
		for (int j = 0; j < node->index_num; j ++)
		{
			cout<< node->indexs[j]<<" ";
		}
		cout<<endl;
	}
	/*BPlusTreeData * leaf = (BPlusTreeData*)root->pointers[0];
	while (leaf)
	{
		cout << leaf->index_num << endl;
		leaf = leaf->next;
	}*/
	return 0;
}