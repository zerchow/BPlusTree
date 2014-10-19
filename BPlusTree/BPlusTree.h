#include <iostream>
#include <list>

using namespace std;

#define M 5
//在内存中建立一个数据文件，文件大小必须在500M以上，具体数据项自行决定，项数20M以上;
struct  BPlusTreeNode//B+树节点;
{
	int indexnum;                   //索引数目;
	int indexs[M];                  //索引;
	void * pointers[M + 1];         //子节点指针;
	BPlusTreeNode * parent;         //父母节点;
	int childnum;                   //是父母的第几个孩子;
	bool isLeaf;                    //是否为叶子节点;
};

struct Data
{
	Data * next;
};

class BPlusTree
{
private:
	BPlusTreeNode * root;
public:
};