#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>
using namespace std;

#define M 2048
#define L 64
//在内存中建立一个数据文件，文件大小必须在500M以上，具体数据项自行决定，项数20M以上;
class BPlusTreeBase //基类节点，派生内部节点和叶子节点;
{
public:
	bool is_leaf;          //是否为叶子节点;
	short index_num;       //索引数目;
};

class  BPlusTreeNode : public BPlusTreeBase//内部索引节点;
{
public:
	int indexs[M];                  //索引;
	BPlusTreeBase * pointers[M];    //子节点指针;
	BPlusTreeNode * parent;         //父母节点;
};

struct Data
{
	int index;             //键值，充当索引;
	char data[24];         //附属数据;
};

class BPlusTreeData : public BPlusTreeBase//叶子数据节点;
{
public:
	Data * datas[L];       //数据指针;
	BPlusTreeData * next;  //兄弟;
	BPlusTreeNode * parent;//父母节点;
};

class BPlusTree
{
private:
	BPlusTreeNode * root; //指向根节点的指针;
	BPlusTreeData * start;//指向叶节点的指针;

	BPlusTreeBase * find_leaf(int index) //插入时需要插入到叶子，首先要根据键值找到叶子;
	{
		BPlusTreeBase * finder = this->root;//从根节点开始找;
		while(finder && !finder->is_leaf)
		{
			short i;
			for(i = 0; i < finder->index_num; ++i)
			{
				if(((BPlusTreeNode*)finder)->indexs[i] >= index)
				{
					finder = ((BPlusTreeNode*)finder)->pointers[i];
					break;
				}
			}
			if (i == finder->index_num)//如果键值大于所有索引，则插入到最后一个索引;
			{
				finder = ((BPlusTreeNode*)finder)->pointers[i - 1];
			}
		}
		return finder;
	}

	void split_leaf(BPlusTreeData * old_node, Data * data) //分裂叶子节点;
	{
		BPlusTreeData * new_node = new BPlusTreeData();
		short old_count = old_node->index_num;
		short mid_index = old_count >> 1;
		new_node->is_leaf = true;
		new_node->next = old_node->next;
		old_node->next = new_node;
		new_node->parent = old_node->parent;
		/*
		* 根据所要插入键值相对中间值的大小，分裂一个叶子节点;
		* 顺序如下：处理新叶子节点 -> 把新叶子节点插入到父母内部节点 -> 将还未插入的键值进行插入;
		*/
		if (data->index < old_node->datas[mid_index]->index)
		{
			old_node->index_num = mid_index;
			new_node->index_num = old_count - old_node->index_num;
			for (short i = 0; i < new_node->index_num; ++i)
			{
				new_node->datas[i] = old_node->datas[i + mid_index];
			}
			this->fix_leaf_parent(old_node);
			this->insert_in_node(new_node->parent, new_node->datas[new_node->index_num - 1]->index, new_node);
			this->insert_in_leaf(old_node, data);
		}
		else
		{
			old_node->index_num = mid_index + 1;
			new_node->index_num = old_count - old_node->index_num;
			for (short i = 0; i < new_node->index_num; ++i)
			{
				new_node->datas[i] = old_node->datas[i + mid_index + 1];
			}
			this->fix_leaf_parent(old_node);
			this->insert_in_node(new_node->parent, new_node->datas[new_node->index_num - 1]->index, new_node);
			this->insert_in_leaf(new_node, data);
		}
	}

	void split_node(BPlusTreeNode * old_node, int index, BPlusTreeBase * child) //分裂内部节点;
	{
		BPlusTreeNode * new_node = new BPlusTreeNode();
		short old_count = old_node->index_num;
		short mid_index = old_count >> 1;
		if(old_node->parent == NULL) //若父母为NULL，则说明此次要分裂的是根节点;
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
		/*
		* 根据所要插入键值相对中间值的大小，分裂一个内部节点;
		* 顺序如下：处理新内部节点 -> 把新内部节点插入到父母内部节点 -> 将还未插入的键值进行插入;
		*/
		if (index < old_node->indexs[mid_index])
		{
			old_node->index_num = mid_index;
			new_node->index_num = old_count - old_node->index_num;
			for (short i = 0; i < new_node->index_num; ++i)
			{
				new_node->indexs[i] = old_node->indexs[i + mid_index];
				new_node->pointers[i] = old_node->pointers[i + mid_index];
			}
			this->fix_node_parent(old_node);
			this->insert_in_node(new_node->parent, new_node->indexs[new_node->index_num - 1], new_node);
			//更改父母指针，如果不改，则会生成不正确的树;
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
			new_node->index_num = old_count - old_node->index_num;
			for (short i = 0; i < new_node->index_num; ++i)
			{
				new_node->indexs[i] = old_node->indexs[i + mid_index + 1];
				new_node->pointers[i] = old_node->pointers[i + mid_index + 1];
			}
			this->fix_node_parent(old_node);
			this->insert_in_node(new_node->parent, new_node->indexs[new_node->index_num - 1], new_node);
			//更改父母指针，如果不改，则会生成不正确的树;
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

	void insert_in_leaf(BPlusTreeData * leaf, Data * data) //将数据指针插入到叶子节点;
	{
		if (leaf->index_num == L) //如果叶子节点数据指针已经达到最大数目，则先分裂，再插入;
		{
			this->split_leaf(leaf, data);
		}
		else //否则通过数组的处理进行插入;
		{
			short i;
			for (i = leaf->index_num - 1; i >= 0; --i)
			{
				if (leaf->datas[i]->index > data->index)
					leaf->datas[i + 1] = leaf->datas[i];
				else
					break;
			}
			leaf->datas[i + 1] = data;
			++leaf->index_num;
			this->fix_leaf_parent(leaf);
		}
	}
	void insert_in_node(BPlusTreeNode * node, int index, BPlusTreeBase * child)
	//将作为孩子节点的叶子节点或内部节点插入到父母节点;
	{
		if (node->index_num == M) //如果父母节点数据指针已经达到最大数目，则先分裂，再插入;
		{
			this->split_node(node, index, child);
		}
		else //否则通过数组的处理进行插入;
		{
			short i;
			for (i = node->index_num - 1; i >= 0; --i)
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

	void fix_leaf_parent(BPlusTreeData * actual_leaf) //叶子节点改变，父母的索引要根据键值进行修复;
	{
		//1:二分查找（平均情况下比较快）;
		/*int start_index = 0;
		int end_index = actual_leaf->index_num;
		int mid_index;
		while(start_index < end_index)
		{
			mid_index = (start_index + end_index) >> 1;
			if(actual_leaf->parent->pointers[mid_index] == actual_leaf)
			{
				actual_leaf->parent->indexs[mid_index] = actual_leaf->datas[actual_leaf->index_num - 1]->index;
				this->fix_node_parent(actual_leaf->parent);
				break;
			}
			else if(actual_leaf->parent->pointers[mid_index] > actual_leaf)
				end_index = mid_index;
			else
				start_index = mid_index + 1;
		}*/
		//2:顺序查找（平均查找速度较慢）;
		for (short i = 0; i < actual_leaf->parent->index_num; ++i)
		{
			if (actual_leaf->parent->pointers[i] == actual_leaf)
			{
				actual_leaf->parent->indexs[i] = actual_leaf->datas[actual_leaf->index_num - 1]->index;
				this->fix_node_parent(actual_leaf->parent);
				break;
			}
		}
	}

	void fix_node_parent(BPlusTreeNode * actual_node) //孩子节点索引改变，父母索引要相应进行修复;
	{
		if (actual_node->parent == NULL)
		{
			return;
		}
		//1:二分查找（平均情况下比较快）;
		/*int start_index = 0;
		int end_index = actual_node->index_num;
		int mid_index;
		while(start_index < end_index)
		{
		mid_index = (start_index + end_index) >> 1;
		if(actual_node->parent->pointers[mid_index] == actual_node)
		{
		actual_node->parent->indexs[mid_index] = actual_node->indexs[actual_node->index_num - 1];
		this->fix_node_parent(actual_node->parent);
		break;
		}
		else if(actual_node->parent->pointers[mid_index] > actual_node)
		end_index = mid_index;
		else
		start_index = mid_index + 1;
		}*/
		//2:顺序查找（平均查找速度较慢）;
		for (short i = 0; i < actual_node->parent->index_num; ++i)
		{
			if (actual_node->parent->pointers[i] == actual_node)
			{
				actual_node->parent->indexs[i] = actual_node->indexs[actual_node->index_num - 1];
				if(actual_node->parent != NULL)
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
	bool insert(Data * data) //插入;
	{
		if (this->root->index_num == 0) //如果插入的是第一个数据，则新建一个节点进行存储，并进行相应的初始化工作;
		{
			BPlusTreeData * first_data_node = new BPlusTreeData();//新叶子;
			first_data_node->is_leaf = true;                      //是叶子;
			first_data_node->parent = this->root;                 //父母是根节点;
			first_data_node->next = NULL;                         //无兄弟;
			first_data_node->datas[0] = data;                     //存储数据;
			first_data_node->index_num = 1;                       //仅有第一个数据;
			this->root->indexs[0] = data->index;                   //定根的索引;
			this->root->pointers[0] = first_data_node;            //定根的孩子;
			this->root->index_num = 1;                            //仅有第一个孩子;
			this->start = first_data_node;                        //范围查询的起点（因为分裂趋势向右，所以起点以后不会变）;
			return true;                                          //第一个就此返回;
		}

		//第二次及以后的数据插入;
		BPlusTreeData * leaf = (BPlusTreeData*)this->find_leaf(data->index); //找到放置数据的叶子;
		
		//重复数据不予插入;
		//1:二分查找（平均情况下比较快）;
		int start_index = 0;
		int end_index = leaf->index_num;
		int mid_index;
		while(start_index < end_index)
		{
			mid_index = (start_index + end_index) >> 1;
			if(leaf->datas[mid_index]->index == data->index)
				return false;
			else if(leaf->datas[mid_index]->index > data->index)
				end_index = mid_index;
			else
				start_index = mid_index + 1;
		}
		//2:顺序查找（平均查找速度较慢）;
		/*for(short i = 0; i < leaf->index_num; ++i)
			if(leaf->datas[i]->index == data->index)
				return false;*/

		this->insert_in_leaf(leaf, data);//往叶子插入数据;
		return true;
	}

	Data * query(int index)
	{
		BPlusTreeData * leaf = (BPlusTreeData*)this->find_leaf(index); //找到放置数据的叶子;
		for(int i = 0; i < leaf->index_num; ++i)
		{
			if(leaf->datas[i]->index == index)
				return leaf->datas[i];
		}
		return NULL;
	}

	BPlusTreeNode * getroot() //前期测试，获取B+树的根节点;
	{
		return this->root;
	}

	int getheight()//前期测试，获取B+树的高度;
	{
		BPlusTreeBase * height_calculator = this->root;
		int height = 0;
		while (height_calculator)
		{
			++height;
			if (height_calculator->is_leaf)
				break;
			else
				height_calculator = ((BPlusTreeNode*)height_calculator)->pointers[0];
		}
		return height;
	}
};