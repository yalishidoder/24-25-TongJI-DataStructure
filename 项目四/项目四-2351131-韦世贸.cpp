#define _CRT_SECURE_NO_WARNINGS        //����strcpy
#include <iostream>
#include <cstring>

//������Ľ���ֱ�������׺���ʽ��ֵ

using namespace std;

double res = 0;

class MyStringIterator           //�ַ����������࣬����ʹ���±����Ԫ�أ�����ʹ��begin(),end()
{
public:
	MyStringIterator(char* ptr) : m_ptr(ptr) {}

	char& operator*() const        //ͨ�����������ʺ��޸��ַ����е��ַ�
	{
		return *m_ptr;
	}

	MyStringIterator& operator++()  //++ǰ׺
	{
		++m_ptr;
		return *this;
	}

	MyStringIterator operator++(int) //++��׺
	{
		MyStringIterator temp = *this;
		++m_ptr;
		return temp;
	}

	bool operator==(const MyStringIterator& other) const
	{
		return m_ptr == other.m_ptr;
	}

	bool operator!=(const MyStringIterator& other) const
	{
		return m_ptr != other.m_ptr;
	}

	char& operator[](size_t index) const   //�±����
	{
		return *(m_ptr + index);
	}

private:
	char* m_ptr;
};

//������в������ƶ����죬��ֵ������������Ϊ��ֵʱ����ʹ�ÿ�������Ϳ�����ֵ���
class MyString
{
public:
	MyString(const char* cstr = 0);   //Ĭ�Ϲ��캯��
	MyString(const MyString& str);    //�������캯��
	~MyString() {}
	MyString& operator=(const MyString& str);  //������ֵ���������
	MyString(MyString&& str); //�ƶ����캯��
	MyString& operator=(MyString&& str);   //�ƶ���ֵ
	MyString& operator+=(const MyString& other);        //����+=����� ƴ��string
	MyString& operator+=(const char* str);              //ƴ�ӳ����ַ���
	MyString& operator+=(const char ch);                //ƴ�ӵ����ַ�
	char* get_c_str()const
	{
		return m_data;
	}
	size_t get_length()const
	{
		return strlen(get_c_str());
	}
	friend ostream& operator<<(ostream& os, const MyString& str);    //��������������
	friend istream& operator>>(istream& is, MyString& str);          //���������������

	MyStringIterator begin()
	{
		return MyStringIterator(m_data);
	}

	MyStringIterator end()
	{
		return MyStringIterator(m_data + strlen(m_data));
	}

	char& operator[](size_t index)
	{
		return m_data[index];
	}

	const char& operator[](size_t index) const
	{
		return m_data[index];
	}
private:
	char* m_data;
};

MyString::MyString(const char* cstr)
{
	if (cstr)
	{
		int len = strlen(cstr) + 1;  //��ҪԤ��\0��λ��
		m_data = new char[len];

		strcpy(m_data, cstr);        //cpy���\0���ƹ���
	}
	else {
		m_data = new char[1];
		*m_data = '\0';
	}

}

MyString::MyString(const MyString& str)
{
	m_data = new char[strlen(str.m_data) + 1];

	strcpy(m_data, str.m_data);
}

MyString::MyString(MyString&& str)   //�ƶ����죬�൱�����,str��ʱΪ��ֵ
{
	m_data = str.m_data;
	str.m_data = NULL;
}

MyString& MyString::operator=(const MyString& str)    //������ֵ
{
	if (this == &str)               //�Լ����Լ�����
		return *this;

	delete[]m_data;                 //���Ƕ�̬�����ڴ�

	//�����뿽�����캯����ͬ
	m_data = new char[strlen(str.m_data) + 1];
	strcpy(m_data, str.m_data);

	return *this;
}

MyString& MyString::operator=(MyString&& str)            //�ƶ���ֵ,str��ʱΪ��ֵ
{
	delete[]m_data;

	m_data = str.m_data;
	str.m_data = NULL;

	return *this;
}

MyString& MyString::operator+=(const MyString& other)
{
	int newlen = get_length() + other.get_length();
	char* newdata = new char[newlen + 1];

	//���Ƶ�ǰ���ַ���
	strcpy(newdata, m_data);
	//ƴ����һ���ַ���
	strcat(newdata, other.m_data);

	//�ͷ��ڴ�
	delete[] m_data;

	m_data = newdata;               //�����ַ���

	return *this;
}

MyString& MyString::operator+=(const char* str)
{
	int other_len = int(strlen(str));
	int newlen = get_length() + other_len;
	char* newdata = new char[newlen + 1];

	//���Ƶ�ǰ���ַ���
	strcpy(newdata, m_data);
	//ƴ����һ���ַ���
	strcat(newdata, str);

	//�ͷ��ڴ�
	delete[] m_data;

	m_data = newdata;               //�����ַ���

	return *this;
}

MyString& MyString::operator+=(const char ch)   //������ǰ������������
{
	int new_len = get_length() + 1;
	char* new_data = new char[new_len + 1];

	strcpy(new_data, m_data);

	new_data[new_len - 1] = ch;
	new_data[new_len] = '\0';

	delete[] m_data;

	m_data = new_data;

	return *this;
}

ostream& operator<<(ostream& os, const MyString& str)    //��������������
{
	os << str.m_data;
	//os << str.get_c_str();
	return os;
}

istream& operator>>(istream& is, MyString& str)
{
	is >> str.m_data;
	return is;
}

MyString input, output;

void input_inline(MyString& str)         //����һ���е�����
{
	while (1) {
		char ch = cin.get();
		if (ch == EOF || ch == '\n')
			break;
		str += ch;
	}
}

void Input()
{
	cout << "����һ���и����Կո�ָ���ͬ�������׺���ʽ���ɰ���+, -, *, /, -, *, /�Լ��������ţ����ʽ������20���ַ����������ո�" << endl;
	input_inline(input);
}

struct	Option
{
	char data;                   //ֻ���շ�����ջ
	int icp;                     //ջ�����ȼ�
	int isp;                     //ջ�����ȼ�

	Option(char ch = ' ', int ISP = 0, int ICP = 0)
		:data(ch), isp(ISP), icp(ICP)
	{
		switch (data)
		{
			//ջ�׷��ź�������ֹ����
			case'#':
				isp = 0;
				icp = 0;
				break;
				//����
			case'(':
				isp = 1;
				icp = 6;
				break;
			case')':
				isp = 6;
				icp = 1;
				break;
				//�߼������
			case'*':
			case'/':
			case'%':
				isp = 5;
				icp = isp - 1;
				break;
				//��ͨ�����
			case'+':
			case'-':
				isp = 3;
				icp = isp - 1;
			default:
				break;
		}
	}
};

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

template <class T>
class LinkedStack
{
public:
	LinkedStack() :top(NULL) {};
	~LinkedStack() {};

	bool isEmpty()const
	{
		return top == NULL;
	}

	void Push(T x)
	{
		LinkNode<T>* New = new LinkNode<T>(x);

		if (New == NULL) {
			cout << "�洢����ʧ��" << endl;
			return;
		}

		New->link = top;
		top = New;
	}

	bool Pop(T& x)
	{
		if (isEmpty())
			return false;

		LinkNode<T>* temp = top;
		top = top->link;
		x = temp->data;
		delete temp;

		return true;
	}

	bool getTop(T& x)const
	{
		if (isEmpty())
			return false;

		x = top->data;
		return true;
	}
private:
	LinkNode<T>* top;
};

LinkedStack <Option> changeStack;
LinkedStack <double> calStack;

void Change()
{
	MyString temp;

	Option bottom('#');                   //����ջ�׷���
	changeStack.Push(bottom);
	input += " #";                        //������ֹ����

	int inLen = int(input.get_length());  //����ı��ʽ�ĳ���

	for (int i = 0; i < inLen; i++) {
		if (input[i] != ' ') {            //û�ж����ո������������ֱ�������ո�
			temp += input[i];
			continue;
		}
		else {                            //�����ո�ִ����ջ�����������
			//�������
			if ((temp[0] >= '0' && temp[0] <= '9') || temp.get_length() >= 2)
			{
				if (temp[0] == '+')       //����ǰ�Զ�ʡ������
					for (int i = 1; i < int(temp.get_length()); i++)
						output += temp[i];
				else
					output += temp;

				if (i != inLen - 1)
					output += " ";
			}
			else {
				Option ch(temp[0]);

				if (changeStack.isEmpty()) {
					changeStack.Push(ch);
				}
				else {
					Option top;
					changeStack.getTop(top);

					while (ch.icp < top.isp)       //��ջ���ȼ����ڳ�ջ���ȼ�
					{
						Option out;
						changeStack.Pop(out);
						output += out.data;
						if (i != inLen - 1)
							output += " ";
						changeStack.getTop(top);
					}

					if (ch.icp > top.isp)          //��ջ���ȼ����ڳ�ջ���ȼ�
						changeStack.Push(ch);
					if (ch.icp == top.isp)         //��ջ�����
						changeStack.Pop(top);

				}
			}
			temp = "";                    //����ÿ�temp�ַ���
		}
	}

	while (!changeStack.isEmpty()) {        //������������ջ�ڻ���Ԫ�أ������ջ
		Option out;
		changeStack.Pop(out);

		if (out.data != '#')                //ջ�׷��Ų����
			output += out.data;
		if (!changeStack.isEmpty())
			output += " ";
	}
}

double to_beNum(MyString& change)
{
	double value = 0, calDecimal = 10;
	bool isDecimal = false;       //�жϴ�ʱ��ת���Ƿ�ΪС��λ��ת��

	// ʹ�õ����������ַ���
	for (MyStringIterator it = change.begin(); it != change.end(); ++it) {

		if (*it == '-')
			continue;

		if (*it == '.') {
			isDecimal = true;
			continue;
		}

		if (isDecimal) {
			value += ((*it - '0') * 1.0 / calDecimal);
			calDecimal *= 10;
		}
		else
			value = value * 10 + (*it - '0') * 1.0;
	}

	if (change[0] == '-')
		value = -value;

	return value;
}

bool get2Append(double& left, double& right)
{
	if (calStack.isEmpty())
	{
		cout << "ȱ���Ҳ�������" << endl;
		return false;
	}

	calStack.Pop(right);

	if (calStack.isEmpty())
	{
		cout << "ȱ�����������" << endl;
		return false;
	}

	calStack.Pop(left);

	return true;
}

void DoOperator(MyString& option)
{
	double value = 0, left = 0, right = 0;

	if (get2Append(left, right)) {
		switch (option[0])
		{
			case'+':
				value = left + right;
				break;
			case'-':
				value = left - right;
				break;
			case'*':
				value = left * right;
				break;
			case'/':
				if (right == 0) {
					cout << "��������Ϊ0��" << endl;
					return;
				}
				value = left / right;
				break;
			default:
				break;
		}
		calStack.Push(value);
	}
}

void Conversion()
{
	//ȡ������
	//������������Բ�������������
	//����󽫽������ջ
	int outLen = int(output.get_length());

	MyString temp;
	for (int i = 0; i < outLen; i++) {
		if (output[i] != ' ') {
			temp += output[i];
			continue;
		}
		else {
			//�������
			if ((temp[0] >= '0' && temp[0] <= '9') || temp.get_length() >= 2)
				calStack.Push(to_beNum(temp));
			//�����
			else
				DoOperator(temp);

			temp = "";   //����ÿ�temp�ַ���
		}
	}
}

void Caculate()
{
	Conversion();

	//������ɺ�ջ�ڵ����һ��Ԫ�ؾ��Ǳ��ʽ�Ľ��
	calStack.getTop(res);
}

void printRes()
{
	cout << "ת����ĺ�׺���ʽ��" << endl;
	cout << output << endl;

	//<<" \n"[i == outLen - 1];

	cout << "�ú�׺���ʽ�Ľ���ǣ�" << res << endl;
}

//2 + 3 * ( 7 - 4 ) + 8 / 4
//( ( 2 + 3 ) * 4 - ( 8 + 2 ) ) / 5
//1314 + 25.5 * 12
//-2 * ( +3 )
//123
int main()
{
	Input();
	Change();
	Caculate();
	printRes();
	return 0;
}