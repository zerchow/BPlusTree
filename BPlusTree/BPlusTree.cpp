#include "BPlusTree.h"

int main()
{
	BPlusTree * tree = new BPlusTree();
	tree->insert({ 10, "" });
	tree->insert({ 11, "" });
	tree->insert({ 12, "" });
	tree->insert({ 13, "" });
	tree->insert({ 14, "" });
	tree->insert({ 15, "" });
	BPlusTreeNode * root = tree->getroot();
	int rootnum = root->index_num;
	for (int i = 0; i < rootnum; i++)
		cout << root->indexs[i] << " ";
	cout << endl;
	BPlusTreeData * leaf = (BPlusTreeData*)root->pointers[0];
	while (leaf)
	{
		cout << leaf->index_num << endl;
		leaf = leaf->next;
	}
	return 0;
}