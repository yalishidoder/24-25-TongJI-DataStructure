#include <iostream>

using namespace std;

template <class T>
struct LinkNode
{
	T data;
	LinkNode<T>* link;
	LinkNode(const T& item, LinkNode<T>* ptr = NULL)
		:data(item), link(ptr)
	{
	}
};

template<class T>
class LinkedQueue
{
public:
	LinkedQueue() :front(NULL), rear(NULL) {}

	~LinkedQueue() {}

	bool isEmpty()
	{
		return front == NULL;
	}

	void makeEmpty()
	{
		LinkNode<T>* del;
		while (front != NULL)
		{
			del = front;
			front = front->link;
			delete del;
		}
	}

	//进队列
	bool EnQueue(T x)
	{
		if (front == NULL) {
			//当队列为空时，新结点作为队列的第一个结点，同时作为队首和队尾
			front = rear = new LinkNode<T>(x);
			if (front == NULL) {
				cout << "存储分配失败！" << endl;
				return false;
			}
		}
		else {
			//队列不空，将新结点接到队尾后
			rear->link = new LinkNode<T>(x);
			if (rear->link == NULL) {
				cout << "存储分配失败！" << endl;
				return false;
			}

			rear = rear->link;     //更新队尾
		}

		return true;
	}

	//出队列
	bool DeQueue(T& x)
	{
		if (isEmpty())       //队列为空
			return false;

		LinkNode<T>* del = front;
		x = del->data;

		front = front->link;
		delete del;

		return true;
	}

	//取得队首元素
	bool getFront(T& x)
	{
		if (isEmpty())       //队列为空
			return false;

		x = front->data;

		return true;
	}

	//取得队列长度
	int getLength()
	{
		LinkNode<T>* current = front;
		int len = 0;

		while (current != NULL) {
			current = current->link;
			len++;
		}

		return len;
	}

	//输出运算符重载
	template<class T>
	friend ostream& operator<< (ostream& os, LinkedQueue<T>& Queue);
private:
	LinkNode<T>* front, * rear;
};

LinkedQueue<int>A, B, All_order;              //三个队列的定义

template<class T>
ostream& operator<< (ostream& os, LinkedQueue<T>& Queue)
{
	LinkNode<T>* current = Queue.front;

	for (LinkNode<T>* current = Queue.front; current != NULL; current = current->link)
		os << current->data << " \n"[current->link == NULL];

	return os;
}

void Input()
{
	cout << "请输入一行正整数，其中第一数字N（N<=1000）为顾客总数，后面跟着N位顾客的编号" << endl;
	int N, num;

	cin >> N;

	for (int i = 0; i < N; i++) {
		cin >> num;

		if (num % 2)
			A.EnQueue(num);
		else
			B.EnQueue(num);
	}

	if (N == 0)
		cout << "此时银行没有客户！" << endl;
}

void Sort()
{
	int data;

	while (A.getLength() >= 2 && !B.isEmpty()) {
		//当a窗口处理完两个业务时，b窗口处理完一个业务且先输出a窗口的客户编号
		A.DeQueue(data);
		All_order.EnQueue(data);
		A.DeQueue(data);
		All_order.EnQueue(data);

		B.DeQueue(data);
		All_order.EnQueue(data);
	}

	//处理剩余的客户
	if (A.getLength() < 2 && !B.isEmpty()) {
		//a窗口的输出即将结束
		int alen = A.getLength();
		for (int i = 0; i < alen; i++) {
			A.DeQueue(data);
			All_order.EnQueue(data);
		}

		//此时只剩余b窗口的输出
		int blen = B.getLength();
		for (int i = 0; i < blen; i++) {
			B.DeQueue(data);
			All_order.EnQueue(data);
		}
	}
	else if (B.isEmpty()) {
		//此时只剩余a窗口的输出
		int alen = A.getLength();
		for (int i = 0; i < alen; i++) {
			A.DeQueue(data);
			All_order.EnQueue(data);
		}
	}
}

void outputRes()
{
	cout << All_order;
}


int main()
{
	Input();
	Sort();
	outputRes();
	return 0;
}