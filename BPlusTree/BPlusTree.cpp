#include "BPlusTree.h"
/*
1）在内存中建立一个数据文件，文件大小必须在500M以上，具体数据项自行决定，项数20M以上；
2）针对上述数据文件，用B+树建立内存索引，内存索引必须能够适应数据文件记录的插入、删除和更新。
3）文档要求同前，需要有使用索引前后查询、插入和删除数据的效率对比。
*/
int main()
{
	time_t start_time, end_time;

	//生成560M的内存数据;
	start_time = clock();
	const int DATA_NUM = 20 << 20; //const int DATA_NUM = 20 * 1024 * 1024;
	Data * memory_datas = new Data[DATA_NUM]; //28 * 20 * 1024 * 1024 = 560 (M);
	for (int i = 0; i < DATA_NUM; ++i)
	{
		memory_datas[i].index = i;
		strcpy(memory_datas[i].data, "file_content");
	}
	end_time = clock();
	cout<<"生成数据耗时："<<end_time - start_time<<" ms"<<endl;

	//把560M数据插入B+树，建立索引;
	start_time = clock();
	BPlusTree * tree = new BPlusTree();
	for (int i = 0; i < DATA_NUM; ++i)
	{
		tree->insert(&memory_datas[i]);
	}
	end_time = clock();
	cout<<"插入数据耗时："<<end_time - start_time<<" ms"<<endl;
	
	//测试用，打印根节点信息，以及树的高度;
	/*BPlusTreeNode * root = tree->getroot();
	short rootnum = root->index_num;
	for (short i = 0; i < rootnum; ++i)
		cout << root->indexs[i] << " ";
	cout << endl;

	cout << "height : " << tree->getheight() << endl;*/
	
	srand((unsigned)time(0));
	int query_number = 10;
	//查询索引;
	start_time = clock();
	for(int i = 0; i < query_number; ++i)
	{
		int rand_num = rand() % DATA_NUM;
		cout<<rand_num<<" : ";
		Data * result = tree->query(rand_num);
		if(result == NULL)
			cout<<"Not found !"<<endl;
		else
			cout<<"Result: index->"<<result->index<<" , data->"<<result->data<<endl;
	}
	end_time = clock();
	cout<<"B+树查询数据耗时："<<end_time - start_time<<" ms"<<endl;

	//普通查询;
	start_time = clock();
	for(int i = 0; i < query_number; ++i)
	{
		int rand_num = rand() % DATA_NUM;
		cout<<rand_num<<" : ";
		for(int i = 0; i < DATA_NUM; ++i)
		{
			if(memory_datas[i].index == rand_num)
			{
				cout<<"Result: index->"<<memory_datas[i].index<<" , data->"<<memory_datas[i].data<<endl;
				break;
			}
			else if(i == DATA_NUM - 1)
			{
				cout<<"Not found !"<<endl;
			}
		}
	}
	end_time = clock();
	cout<<"普通查询数据耗时："<<end_time - start_time<<" ms"<<endl;
	

	system("pause"); //暂停程序，查看所占内存;

	return 0;
}