#include <iostream>
#include <list>

using namespace std;

class BPlusTreeNode//�ڲ��ڵ�;
{
private:
public:
	struct IndexUnit          //������Ϣ�ṹ;
	{
		int index;            //����;
		BPlusTreeNode * child;//����ָ��Ľڵ�;
	};
	int indexnum;          //������Ŀ;
	list<IndexUnit> indexs;//�ڵ��ϵ�������Ϣ;
	BPlusTreeNode * parent;//��ĸ�ڵ�;
	int childnum;          //�Ǹ�ĸ�ĵڼ�������;
	bool isLeaf;           //�Ƿ�ΪҶ�ӽڵ�;
};

class BPlusTreeLeaf : BPlusTreeNode//Ҷ�ӽڵ�;
{
/*
* ������������;
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