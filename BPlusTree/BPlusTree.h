#include <iostream>
#include <list>

using namespace std;

#define M 5
//���ڴ��н���һ�������ļ����ļ���С������500M���ϣ��������������о���������20M����;
struct  BPlusTreeNode//B+���ڵ�;
{
	int indexnum;                   //������Ŀ;
	int indexs[M];                  //����;
	void * pointers[M + 1];         //�ӽڵ�ָ��;
	BPlusTreeNode * parent;         //��ĸ�ڵ�;
	int childnum;                   //�Ǹ�ĸ�ĵڼ�������;
	bool isLeaf;                    //�Ƿ�ΪҶ�ӽڵ�;
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