#include <iostream>

using namespace std;

struct LinkNode                         //����ڵ�
{
	int data;                           //����Ԫ��
	LinkNode* link;                     //����ָ��

	LinkNode(LinkNode* ptr = NULL)      //������Ϣ
		: data(0)
	{
		link = ptr;
	}
};

class MyList                            //����
{
private:
	LinkNode* first;                    //����ͷָ�� 

public:
	MyList()                   //���캯��
	{
		first = new LinkNode();         //����ͷ���
	}

	~MyList()                  //��������
	{
		;
	}

	void CreateNewList()               //������������巨��
	{
		LinkNode* newNode, * last;

		last = first;

		while (1) {     //��˳������Ԫ��
			newNode = new LinkNode();
			cin >> newNode->data;

			if (newNode == NULL) {
				cout << "�洢�������" << endl;
				return;
			}
			else if (newNode->data < 0) {
				delete newNode;
				return;
			}

			last->link = newNode;          //βָ��ָ���½�㣬�������½��
			last = newNode;                //����βָ���λ��
		}

		last->link = NULL;                 //�����β
		return;
	}

	void output_subset(int length)                 //����ӱ�
	{
		LinkNode* current = first->link;

		while (length > 0) {
			cout << current->data;
			if (length != 1)
				cout << " ";

			current = current->link;
			length--;
		}
	}

	void Check_repeat(MyList List1, MyList List2)             //�������
	{

		if (List1.first == NULL || List2.first == NULL)       //������һΪ��
			cout << "NULL" << endl;
		else {
			LinkNode* first_current = List1.first->link,      //�ӵ�һ����Ч��㿪ʼ����
				* subset_current = first;
			int subset_length = 0;                            //�ӱ�ĳ���

			//��������
			while (first_current != NULL) {
				LinkNode* second_current = List2.first->link; //ÿ��ѭ���ӵ�һ����Ч��㿪ʼ����

				while (second_current != NULL)
				{

					if (first_current->data == second_current->data) {
						LinkNode* newNode = new LinkNode();

						newNode->data = first_current->data;
						subset_current->link = newNode;
						subset_current = newNode;             //������λ�ã��൱��last

						subset_length++;                      //�����ӱ�ĳ���
					}
					second_current = second_current->link;
				}

				first_current = first_current->link;
			}

			if (subset_length == 0)                          //����û�н���
				cout << "NULL";
			else
				output_subset(subset_length);                //��ӡ�ӱ�����

			cout << endl;
		}
	}
}first, second, subset;

int main()
{
	cout << "�����������������������У��ǽ������У���-1��ʾ���н���" << endl;

	first.CreateNewList();
	second.CreateNewList();

	subset.Check_repeat(first, second);

	return 0;
}