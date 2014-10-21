#include <iostream>
#include <cstring>
using namespace std;

#define M 5
#define L 5
//在内存中建立一个数据文件，文件大小必须在500M以上，具体数据项自行决定，项数20M以上;
class BPlusTreeBase
{
public:
	bool is_leaf;          //是否为叶子节点;
	int index_num;         //索引数目;
};

class  BPlusTreeNode : public BPlusTreeBase//B+树节点;
{
public:
	int indexs[M];                  //索引;
	BPlusTreeBase * pointers[M];    //子节点指针;
	BPlusTreeNode * parent;         //父母节点;
};

struct Data
{
	int index;             //键值，充当索引;
	char data[28];         //附属数据;
};

class BPlusTreeData : public BPlusTreeBase
{
public:
	Data datas[L];         //数据;
	BPlusTreeData * next;  //兄弟;
	BPlusTreeNode * parent;//父母节点;
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
	void split_leaf(BPlusTreeData *& old_node, Data data)
	{
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
			this->fix_leaf_parent(old_node);
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
			this->fix_leaf_parent(old_node);
			this->insert_in_node(new_node->parent, new_node->datas[new_node->index_num - 1].index, new_node);
			this->insert_in_leaf(new_node, data);
		}
	}
	void split_node(BPlusTreeNode *& old_node, int index, BPlusTreeBase * child)
	{
		BPlusTreeNode * new_node = new BPlusTreeNode();
		int old_count = old_node->index_num;
		int mid_index = old_count / 2;
		if(old_node->parent == NULL)
		{
			this->root = new BPlusTreeNode();
			this->root->is_leaf = false;
			this->root->parent = NULL;
			this->root->index_num = 1;
			old_node->parent = this->root;
			this->root->indexs[0] = old_node->indexs[this->root->index_num - 1];
			this->root->pointers[0] = old_node;
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
			this->fix_node_parent(old_node);
			this->insert_in_node(new_node->parent, new_node->indexs[new_node->index_num - 1], new_node);
			if (child->is_leaf)
			{
				((BPlusTreeData*)child)->parent = old_node;
			}
			else
			{
				((BPlusTreeNode*)child)->parent = old_node;
			}
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
			this->fix_node_parent(old_node);
			this->insert_in_node(new_node->parent, new_node->indexs[new_node->index_num - 1], new_node);
			if (child->is_leaf)
			{
				((BPlusTreeData*)child)->parent = new_node;
			}
			else
			{
				((BPlusTreeNode*)child)->parent = new_node;
			}
			this->insert_in_node(new_node, index, child);
		}
	}
	void insert_in_leaf(BPlusTreeData *& leaf, Data data)
	{
		if (leaf->index_num == L)
		{
			this->split_leaf(leaf, data);
		}
		else
		{
			int i;
			for (i = leaf->index_num - 1; i >= 0; i--)
			{
				if (leaf->datas[i].index > data.index)
					leaf->datas[i + 1] = leaf->datas[i];
				else
					break;
			}
			leaf->datas[i + 1] = data;
			++leaf->index_num;
			this->fix_leaf_parent(leaf);
		}
	}
	void insert_in_node(BPlusTreeNode *& node, int index, BPlusTreeBase * child)
	{
		if (node->index_num == M)
		{
			this->split_node(node, index, child);
		}
		else
		{
			int i;
			for (i = node->index_num - 1; i >= 0; i--)
			{
				if (node->indexs[i] > index)
				{
					node->indexs[i + 1] = node->indexs[i];
					node->pointers[i + 1] = node->pointers[i];
				}
				else
					break;
			}
			node->indexs[i + 1] = index;
			node->pointers[i + 1] = child;
			++node->index_num;
			this->fix_node_parent(node);
		}
	}
	void fix_leaf_parent(BPlusTreeData *& actual_leaf)
	{
		for (int i = 0; i < actual_leaf->parent->index_num; i++)
		{
			if ((actual_leaf->parent)->pointers[i] == actual_leaf)
			{
				(actual_leaf->parent)->indexs[i] = actual_leaf->datas[actual_leaf->index_num - 1].index;
				this->fix_node_parent(actual_leaf->parent);
				break;
			}
		}
	}
	void fix_node_parent(BPlusTreeNode *& actual_node)
	{
		if (actual_node->parent == NULL)
			return;
		for (int i = 0; i < actual_node->parent->index_num; i++)
		{
			if ((actual_node->parent)->pointers[i] == actual_node)
			{
				(actual_node->parent)->indexs[i] = actual_node->indexs[actual_node->index_num - 1];
				this->fix_node_parent(actual_node->parent);
				break;
			}
		}
	}
public:
	BPlusTree() //构造函数，初始化作为根节点的内部节点;
	{
		this->root = new BPlusTreeNode();//新节点;
		this->root->is_leaf = false;     //非叶子;
		this->root->index_num = 0;       //无存储;
		this->root->parent = NULL;       //无父母;
	}
	bool insert(Data data) //插入;
	{
		if (this->root->index_num == 0) //如果插入的是第一个数据，则新建一个节点进行存储，并进行相应的初始化工作;
		{
			BPlusTreeData * first_data_node = new BPlusTreeData();//新叶子;
			first_data_node->is_leaf = true;                      //是叶子;
			first_data_node->parent = this->root;                 //父母是根节点;
			first_data_node->next = NULL;                         //无兄弟;
			first_data_node->datas[0] = data;                     //存储数据;
			first_data_node->index_num = 1;                       //仅有第一个数据;
			this->root->indexs[0] = data.index;                   //定根的索引;
			this->root->pointers[0] = first_data_node;            //定根的孩子;
			this->root->index_num = 1;                            //仅有第一个孩子;
			this->start = first_data_node;                        //范围查询的起点（因为分裂趋势向右，所以起点以后不会变）;
			return true;                                          //第一个就此返回;
		}

		//第二次及以后的数据插入;
		BPlusTreeData * leaf = (BPlusTreeData*)this->find_leaf(data.index); //找到放置数据的叶子;
		
		//重复数据不予插入;
		for(int i = 0; i < leaf->index_num; i ++)
			if(((BPlusTreeData*)leaf)->datas[i].index == data.index)
				return false;

		this->insert_in_leaf(leaf, data);//往叶子插入数据;
		return true;
	}
	BPlusTreeNode * getroot()//just for TMD test
	{
		return this->root;
	}
};