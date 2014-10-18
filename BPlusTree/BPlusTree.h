#include <iostream>
#include <list>

using namespace std;

class BPlusTreeNode//内部节点;
{
private:
public:
	struct IndexUnit          //索引信息结构;
	{
		int index;            //索引;
		BPlusTreeNode * child;//索引指向的节点;
	};
	int indexnum;          //索引数目;
	list<IndexUnit> indexs;//节点上的索引信息;
	BPlusTreeNode * parent;//父母节点;
	int childnum;          //是父母的第几个孩子;
	bool isLeaf;           //是否为叶子节点;
};

class BPlusTreeLeaf : BPlusTreeNode//叶子节点;
{
/*
* 增加数据内容;
*/
private:
	list<int> datas;
public:
};

class BPlusTree
{
private:
	BPlusTreeNode * root;
public:
};