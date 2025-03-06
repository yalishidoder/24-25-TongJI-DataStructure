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

	//������
	bool EnQueue(T x)
	{
		if (front == NULL) {
			//������Ϊ��ʱ���½����Ϊ���еĵ�һ����㣬ͬʱ��Ϊ���׺Ͷ�β
			front = rear = new LinkNode<T>(x);
			if (front == NULL) {
				cout << "�洢����ʧ�ܣ�" << endl;
				return false;
			}
		}
		else {
			//���в��գ����½��ӵ���β��
			rear->link = new LinkNode<T>(x);
			if (rear->link == NULL) {
				cout << "�洢����ʧ�ܣ�" << endl;
				return false;
			}

			rear = rear->link;     //���¶�β
		}

		return true;
	}

	//������
	bool DeQueue(T& x)
	{
		if (isEmpty())       //����Ϊ��
			return false;

		LinkNode<T>* del = front;
		x = del->data;

		front = front->link;
		delete del;

		return true;
	}

	//ȡ�ö���Ԫ��
	bool getFront(T& x)
	{
		if (isEmpty())       //����Ϊ��
			return false;

		x = front->data;

		return true;
	}

	//ȡ�ö��г���
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

	//������������
	template<class T>
	friend ostream& operator<< (ostream& os, LinkedQueue<T>& Queue);
private:
	LinkNode<T>* front, * rear;
};

LinkedQueue<int>A, B, All_order;              //�������еĶ���

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
	cout << "������һ�������������е�һ����N��N<=1000��Ϊ�˿��������������Nλ�˿͵ı��" << endl;
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
		cout << "��ʱ����û�пͻ���" << endl;
}

void Sort()
{
	int data;

	while (A.getLength() >= 2 && !B.isEmpty()) {
		//��a���ڴ���������ҵ��ʱ��b���ڴ�����һ��ҵ���������a���ڵĿͻ����
		A.DeQueue(data);
		All_order.EnQueue(data);
		A.DeQueue(data);
		All_order.EnQueue(data);

		B.DeQueue(data);
		All_order.EnQueue(data);
	}

	//����ʣ��Ŀͻ�
	if (A.getLength() < 2 && !B.isEmpty()) {
		//a���ڵ������������
		int alen = A.getLength();
		for (int i = 0; i < alen; i++) {
			A.DeQueue(data);
			All_order.EnQueue(data);
		}

		//��ʱֻʣ��b���ڵ����
		int blen = B.getLength();
		for (int i = 0; i < blen; i++) {
			B.DeQueue(data);
			All_order.EnQueue(data);
		}
	}
	else if (B.isEmpty()) {
		//��ʱֻʣ��a���ڵ����
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