#include <iostream>
#include <list>

using namespace std;

#define M 5
#define L 5
//在内存中建立一个数据文件，文件大小必须在500M以上，具体数据项自行决定，项数20M以上;
class BPlusTreeBase
{
public:
	bool is_leaf;          //是否为叶子节点;
	int index_num;         //索引数目;
	BPlusTreeBase * parent;//父母节点;
};

class  BPlusTreeNode : public BPlusTreeBase//B+树节点;
{
public:
	int indexs[M];                  //索引;
	BPlusTreeBase * pointers[M];    //子节点指针;
};

struct Data
{
	int index;             //
	char data[28];         //
};

class BPlusTreeData : public BPlusTreeBase
{
public:
	Data datas[L];         //
	BPlusTreeData * next;  //
};

class BPlusTree
{
private:
	BPlusTreeNode * root;
	BPlusTreeData * start;
	BPlusTreeBase * find_leaf(int index)
	{
		BPlusTreeBase * finder = this->root;
		while(finder && !finder->is_leaf)
		{
			int i;
			for(i = 0; i < finder->index_num; i ++)
			{
				if(((BPlusTreeNode*)finder)->indexs[i] >= index)
				{
					finder = ((BPlusTreeNode*)finder)->pointers[i];
					break;
				}
			}
			if (i == finder->index_num)
				finder = ((BPlusTreeNode*)finder)->pointers[i - 1];
		}
		return finder;
	}
	void split_leaf(BPlusTreeBase * leaf, Data data)
	{
		
		BPlusTreeData * old_node = (BPlusTreeData *)leaf;
		BPlusTreeData * new_node = new BPlusTreeData();
		int old_count = old_node->index_num;
		int mid_index = old_count / 2;
		new_node->is_leaf = true;
		new_node->next = old_node->next;
		old_node->next = new_node;
		new_node->parent = old_node->parent;
		if (data.index < old_node->datas[mid_index].index)
		{
			old_node->index_num = mid_index;
			new_node->index_num = old_count - mid_index;
			for (int i = 0; i < new_node->index_num; i++)
			{
				new_node->datas[i] = old_node->datas[i + mid_index];
			}
			this->insert_in_node(new_node->parent, new_node->datas[new_node->index_num - 1].index, new_node);
			this->insert_in_leaf(old_node, data);
		}
		else
		{
			old_node->index_num = mid_index + 1;
			new_node->index_num = old_count - mid_index - 1;
			for (int i = 0; i < new_node->index_num; i++)
			{
				new_node->datas[i] = old_node->datas[i + mid_index + 1];
			}
			this->insert_in_node(new_node->parent, new_node->datas[new_node->index_num - 1].index, new_node);
			this->insert_in_leaf(new_node, data);
		}
	}
	void split_node(BPlusTreeBase * node, int index, BPlusTreeBase * child)
	{//千万别忘了处理分裂节点是根节点的情况;
		BPlusTreeNode * old_node = (BPlusTreeNode *)node;
		BPlusTreeNode * new_node = new BPlusTreeNode();
		int old_count = old_node->index_num;
		int mid_index = old_count / 2;
		if(old_node->parent == NULL)
		{
			this->root = new BPlusTreeNode();
			this->root->is_leaf = false;
			this->root->parent = NULL;
			old_node->parent = this->root;
			new_node->is_leaf = false;
			new_node->parent = old_node->parent;
			if (index < old_node->indexs[mid_index])
			{
				old_node->index_num = mid_index;
				new_node->index_num = old_count - mid_index;
				for (int i = 0; i < new_node->index_num; i++)
				{
					new_node->indexs[i] = old_node->indexs[i + mid_index];
					new_node->pointers[i] = old_node->pointers[i + mid_index];
				}
				this->root->pointers[0] = old_node;
				this->root->indexs[0] = old_node->indexs[old_node->index_num - 1];
				this->root->pointers[1] = new_node;
				this->root->indexs[1] = new_node->indexs[new_node->index_num - 1];
				this->root->index_num = 2;
				this->insert_in_node(old_node, index, child);
			}
			else
			{
				old_node->index_num = mid_index + 1;
				new_node->index_num = old_count - mid_index - 1;
				for (int i = 0; i < new_node->index_num; i++)
				{
					new_node->indexs[i] = old_node->indexs[i + mid_index + 1];
					new_node->pointers[i] = old_node->pointers[i + mid_index + 1];
				}
				this->root->pointers[0] = old_node;
				this->root->indexs[0] = old_node->indexs[old_node->index_num - 1];
				this->root->pointers[1] = new_node;
				this->root->indexs[1] = new_node->indexs[new_node->index_num - 1];
				this->root->index_num = 2;
				this->insert_in_node(new_node, index, child);
			}
			this->fix_node_parent(old_node);
			this->fix_node_parent(new_node);
			return;
		}
		new_node->is_leaf = false;
		new_node->parent = old_node->parent;
		if (index < old_node->indexs[mid_index])
		{
			old_node->index_num = mid_index;
			new_node->index_num = old_count - mid_index;
			for (int i = 0; i < new_node->index_num; i++)
			{
				new_node->indexs[i] = old_node->indexs[i + mid_index];
				new_node->pointers[i] = old_node->pointers[i + mid_index];
			}
			this->insert_in_node(new_node->parent, new_node->indexs[new_node->index_num - 1], new_node);
			this->insert_in_node(old_node, index, child);
		}
		else
		{
			old_node->index_num = mid_index + 1;
			new_node->index_num = old_count - mid_index - 1;
			for (int i = 0; i < new_node->index_num; i++)
			{
				new_node->indexs[i] = old_node->indexs[i + mid_index + 1];
				new_node->pointers[i] = old_node->pointers[i + mid_index + 1];
			}
			this->insert_in_node(new_node->parent, new_node->indexs[new_node->index_num - 1], new_node);
			this->insert_in_node(new_node, index, child);
		}
	}
	void insert_in_leaf(BPlusTreeBase * leaf, Data data)
	{
		if (leaf->index_num == L)
		{
			this->split_leaf(leaf, data);
		}
		else
		{
			BPlusTreeData * actual_leaf = (BPlusTreeData *)leaf;
			int i;
			for (i = actual_leaf->index_num - 1; i >= 0; i--)
			{
				if (actual_leaf->datas[i].index > data.index)
					actual_leaf->datas[i] = actual_leaf->datas[i + 1];
				else
					break;
			}
			actual_leaf->datas[i + 1] = data;
			++actual_leaf->index_num;
		}
		this->fix_leaf_parent(leaf);
	}
	void insert_in_node(BPlusTreeBase * node, int index, BPlusTreeBase * child)
	{
		if (node->index_num == M)
		{
			this->split_node(node, index, child);
		}
		else
		{
			BPlusTreeNode * actual_node = (BPlusTreeNode *)node;
			int i;
			for (i = actual_node->index_num - 1; i >= 0; i--)
			{
				if (actual_node->indexs[i] > index)
				{
					actual_node->indexs[i] = actual_node->indexs[i + 1];
					actual_node->pointers[i] = actual_node->pointers[i + 1];
				}
				else
					break;
			}
			actual_node->indexs[i + 1] = index;
			actual_node->pointers[i + 1] = child;
			++actual_node->index_num;
		}
		this->fix_node_parent(node);
	}
	void fix_leaf_parent(BPlusTreeBase * leaf)
	{
		BPlusTreeData * actual_leaf = (BPlusTreeData*)leaf;
		for (int i = 0; i < actual_leaf->parent->index_num; i++)
		{
			if (((BPlusTreeNode*)actual_leaf->parent)->pointers[i] == actual_leaf)
			{
				((BPlusTreeNode*)actual_leaf->parent)->indexs[i] = actual_leaf->datas[actual_leaf->index_num - 1].index;
				break;
			}
		}
	}
	void fix_node_parent(BPlusTreeBase * node)
	{
		BPlusTreeNode * actual_node = (BPlusTreeNode*)node;
		if (actual_node->parent == NULL)
			return;
		for (int i = 0; i < actual_node->parent->index_num; i++)
		{
			if (((BPlusTreeNode*)actual_node->parent)->pointers[i] == actual_node)
			{
				((BPlusTreeNode*)actual_node->parent)->indexs[i] = actual_node->indexs[actual_node->index_num - 1];
			}
		}
	}
public:
	BPlusTree()
	{
		this->root = new BPlusTreeNode();
		this->root->is_leaf = false;
		this->root->index_num = 0;
		this->root->parent = NULL;
	}
	bool insert(Data data)
	{
		if (this->root->index_num == 0)
		{
			BPlusTreeData * first_data_node = new BPlusTreeData();
			first_data_node->is_leaf = true;
			first_data_node->parent = this->root;
			first_data_node->next = NULL;
			first_data_node->datas[0] = data;
			first_data_node->index_num = 1;
			this->root->indexs[0] = data.index;
			this->root->pointers[0] = first_data_node;
			this->root->index_num = 1;
			this->start = first_data_node;
			return true;
		}
		BPlusTreeBase * leaf = this->find_leaf(data.index);
		for(int i = 0; i < leaf->index_num; i ++)
			if(((BPlusTreeData*)leaf)->datas[i].index == data.index)
				return false;
		this->insert_in_leaf(leaf, data);
		return true;
	}
	BPlusTreeNode * getroot()//just for TMD test
	{
		return this->root;
	}
};