#include <iostream>
#include <list>

using namespace std;

#define M 5
#define L 5
//���ڴ��н���һ�������ļ����ļ���С������500M���ϣ��������������о���������20M����;
class BPlusTreeBase
{
public:
	bool is_leaf;          //�Ƿ�ΪҶ�ӽڵ�;
	int index_num;         //������Ŀ;
	BPlusTreeBase * parent;//��ĸ�ڵ�;
};

class  BPlusTreeNode : public BPlusTreeBase//B+���ڵ�;
{
public:
	int indexs[M];                  //����;
	BPlusTreeBase * pointers[M];    //�ӽڵ�ָ��;
};

struct Data
{
	int index;             //
	char data[12];         //
	int secondkey;         //
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
	BPlusTreeBase * root;
	BPlusTreeBase * findLeaf(int index)
	{
		BPlusTreeBase * finder = this->root;
		while(! finder->is_leaf)
		{
			for(int i = 0; i < finder->index_num; i ++)
			{
				if(((BPlusTreeNode*)finder)->indexs[i] >= index)
				{
					finder = ((BPlusTreeNode*)finder)->pointers[i];
					break;
				}
			}
		}
		return finder;
	}
	void split_leaf(BPlusTreeBase * leaf, Data data)
	{
		BPlusTreeBase * new_node = new BPlusTreeBase();
	}
	void split_node(BPlusTreeBase * node, int index)
	{
		BPlusTreeBase * new_node = new BPlusTreeBase();
	}
	void insert_in_leaf(BPlusTreeBase * leaf, Data data)
	{
		if(leaf->index_num == L)
			this->split_leaf(leaf, data);
		else
		{
			int i;
			for(i = ((BPlusTreeData*)leaf)->index_num - 1; i >= 0; i --)
			{
				if(((BPlusTreeData*)leaf)->datas[i].index > data.index)
					((BPlusTreeData*)leaf)->datas[i] = ((BPlusTreeData*)leaf)->datas[i + 1];
				else
					break;
			}
			((BPlusTreeData*)leaf)->datas[i + 1] = data;
			++((BPlusTreeData*)leaf)->index_num;
		}
	}
	void insert_in_node(BPlusTreeBase * node, int index)
	{
		if(node->index_num == M)
			this->split_node(node, index);
		else
		{

		}
	}
public:
	BPlusTree()
	{
		this->root = new BPlusTreeData();
		this->root->is_leaf = true;
		this->root->index_num = 0;
		this->root->parent = NULL;
	}
	bool insert(Data data)
	{
		BPlusTreeBase * leaf = this->findLeaf(data.index);
		for(int i = 0; i < leaf->index_num; i ++)
			if(((BPlusTreeData*)leaf)->datas[i].index == data.index)
				return false;
		this->insert_in_leaf(leaf, data);
		return true;
	}
};