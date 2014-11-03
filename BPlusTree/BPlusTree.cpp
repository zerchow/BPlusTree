#include "BPlusTree.h"
/*
1�����ڴ��н���һ�������ļ����ļ���С������500M���ϣ��������������о���������20M���ϣ�
2��������������ļ�����B+�������ڴ��������ڴ����������ܹ���Ӧ�����ļ���¼�Ĳ��롢ɾ���͸��¡�
3���ĵ�Ҫ��ͬǰ����Ҫ��ʹ������ǰ���ѯ�������ɾ�����ݵ�Ч�ʶԱȡ�
*/
int main()
{
	time_t start_time, end_time;

	//����560M���ڴ�����;
	start_time = clock();
	const int DATA_NUM = 20 << 20; //const int DATA_NUM = 20 * 1024 * 1024;
	Data * memory_datas = new Data[DATA_NUM]; //28 * 20 * 1024 * 1024 = 560 (M);
	for (int i = 0; i < DATA_NUM; ++i)
	{
		memory_datas[i].index = i;
		strcpy(memory_datas[i].data, "file_content");
	}
	end_time = clock();
	cout<<"�������ݺ�ʱ��"<<end_time - start_time<<" ms"<<endl;

	//��560M���ݲ���B+������������;
	start_time = clock();
	BPlusTree * tree = new BPlusTree();
	for (int i = 0; i < DATA_NUM; ++i)
	{
		tree->insert(&memory_datas[i]);
	}
	end_time = clock();
	cout<<"�������ݺ�ʱ��"<<end_time - start_time<<" ms"<<endl;
	
	//�����ã���ӡ���ڵ���Ϣ���Լ����ĸ߶�;
	/*BPlusTreeNode * root = tree->getroot();
	short rootnum = root->index_num;
	for (short i = 0; i < rootnum; ++i)
		cout << root->indexs[i] << " ";
	cout << endl;

	cout << "height : " << tree->getheight() << endl;*/
	
	srand((unsigned)time(0));
	int query_number = 10;
	//��ѯ����;
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
	cout<<"B+����ѯ���ݺ�ʱ��"<<end_time - start_time<<" ms"<<endl;

	//��ͨ��ѯ;
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
	cout<<"��ͨ��ѯ���ݺ�ʱ��"<<end_time - start_time<<" ms"<<endl;
	

	system("pause"); //��ͣ���򣬲鿴��ռ�ڴ�;

	return 0;
}