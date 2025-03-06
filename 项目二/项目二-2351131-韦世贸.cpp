#include <iostream>

using namespace std;

struct LinkNode                         //链表节点
{
	int data;                           //链表元素
	LinkNode* link;                     //链表指针

	LinkNode(LinkNode* ptr = NULL)      //输入信息
		: data(0)
	{
		link = ptr;
	}
};

class MyList                            //链表
{
private:
	LinkNode* first;                    //链表头指针 

public:
	MyList()                   //构造函数
	{
		first = new LinkNode();         //附加头结点
	}

	~MyList()                  //析构函数
	{
		;
	}

	void CreateNewList()               //建立新链表（后插法）
	{
		LinkNode* newNode, * last;

		last = first;

		while (1) {     //按顺序输入元素
			newNode = new LinkNode();
			cin >> newNode->data;

			if (newNode == NULL) {
				cout << "存储分配错误！" << endl;
				return;
			}
			else if (newNode->data < 0) {
				delete newNode;
				return;
			}

			last->link = newNode;          //尾指针指向新结点，连接起新结点
			last = newNode;                //更新尾指针的位置
		}

		last->link = NULL;                 //链表结尾
		return;
	}

	void output_subset(int length)                 //输出子表
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

	void Check_repeat(MyList List1, MyList List2)             //两表查重
	{

		if (List1.first == NULL || List2.first == NULL)       //两表有一为空
			cout << "NULL" << endl;
		else {
			LinkNode* first_current = List1.first->link,      //从第一个有效结点开始遍历
				* subset_current = first;
			int subset_length = 0;                            //子表的长度

			//遍历两表
			while (first_current != NULL) {
				LinkNode* second_current = List2.first->link; //每次循环从第一个有效结点开始遍历

				while (second_current != NULL)
				{

					if (first_current->data == second_current->data) {
						LinkNode* newNode = new LinkNode();

						newNode->data = first_current->data;
						subset_current->link = newNode;
						subset_current = newNode;             //保存结点位置，相当于last

						subset_length++;                      //更新子表的长度
					}
					second_current = second_current->link;
				}

				first_current = first_current->link;
			}

			if (subset_length == 0)                          //两表没有交集
				cout << "NULL";
			else
				output_subset(subset_length);                //打印子表内容

			cout << endl;
		}
	}
}first, second, subset;

int main()
{
	cout << "请输入两个正整数链表序列，非降序排列，以-1表示序列结束" << endl;

	first.CreateNewList();
	second.CreateNewList();

	subset.Check_repeat(first, second);

	return 0;
}