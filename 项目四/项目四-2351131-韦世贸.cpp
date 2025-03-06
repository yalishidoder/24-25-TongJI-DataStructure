#define _CRT_SECURE_NO_WARNINGS        //用了strcpy
#include <iostream>
#include <cstring>

//补充与改进，直接算出后缀表达式的值

using namespace std;

double res = 0;

class MyStringIterator           //字符串迭代器类，可以使用下标访问元素，可以使用begin(),end()
{
public:
	MyStringIterator(char* ptr) : m_ptr(ptr) {}

	char& operator*() const        //通过迭代器访问和修改字符串中的字符
	{
		return *m_ptr;
	}

	MyStringIterator& operator++()  //++前缀
	{
		++m_ptr;
		return *this;
	}

	MyStringIterator operator++(int) //++后缀
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

	char& operator[](size_t index) const   //下标访问
	{
		return *(m_ptr + index);
	}

private:
	char* m_ptr;
};

//如果类中不包含移动构造，赋值函数，当参数为右值时，会使用拷贝构造和拷贝赋值替代
class MyString
{
public:
	MyString(const char* cstr = 0);   //默认构造函数
	MyString(const MyString& str);    //拷贝构造函数
	~MyString() {}
	MyString& operator=(const MyString& str);  //拷贝赋值运算符重载
	MyString(MyString&& str); //移动构造函数
	MyString& operator=(MyString&& str);   //移动赋值
	MyString& operator+=(const MyString& other);        //重载+=运算符 拼接string
	MyString& operator+=(const char* str);              //拼接常量字符串
	MyString& operator+=(const char ch);                //拼接单个字符
	char* get_c_str()const
	{
		return m_data;
	}
	size_t get_length()const
	{
		return strlen(get_c_str());
	}
	friend ostream& operator<<(ostream& os, const MyString& str);    //重载流输出运算符
	friend istream& operator>>(istream& is, MyString& str);          //重载流输入运算符

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
		int len = strlen(cstr) + 1;  //需要预留\0的位置
		m_data = new char[len];

		strcpy(m_data, cstr);        //cpy会把\0复制过来
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

MyString::MyString(MyString&& str)   //移动构造，相当于深拷贝,str此时为右值
{
	m_data = str.m_data;
	str.m_data = NULL;
}

MyString& MyString::operator=(const MyString& str)    //拷贝赋值
{
	if (this == &str)               //自己给自己拷贝
		return *this;

	delete[]m_data;                 //考虑动态分配内存

	//操作与拷贝构造函数相同
	m_data = new char[strlen(str.m_data) + 1];
	strcpy(m_data, str.m_data);

	return *this;
}

MyString& MyString::operator=(MyString&& str)            //移动赋值,str此时为右值
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

	//复制当前的字符串
	strcpy(newdata, m_data);
	//拼接另一个字符串
	strcat(newdata, other.m_data);

	//释放内存
	delete[] m_data;

	m_data = newdata;               //更新字符串

	return *this;
}

MyString& MyString::operator+=(const char* str)
{
	int other_len = int(strlen(str));
	int newlen = get_length() + other_len;
	char* newdata = new char[newlen + 1];

	//复制当前的字符串
	strcpy(newdata, m_data);
	//拼接另一个字符串
	strcat(newdata, str);

	//释放内存
	delete[] m_data;

	m_data = newdata;               //更新字符串

	return *this;
}

MyString& MyString::operator+=(const char ch)   //过程与前几个函数类似
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

ostream& operator<<(ostream& os, const MyString& str)    //重载流输出运算符
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

void input_inline(MyString& str)         //输入一整行的内容
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
	cout << "请在一行中给出以空格分隔不同对象的中缀表达式，可包含+, -, *, /, -, *, /以及左右括号，表达式不超过20个字符（不包括空格）" << endl;
	input_inline(input);
}

struct	Option
{
	char data;                   //只接收符号入栈
	int icp;                     //栈外优先级
	int isp;                     //栈内优先级

	Option(char ch = ' ', int ISP = 0, int ICP = 0)
		:data(ch), isp(ISP), icp(ICP)
	{
		switch (data)
		{
			//栈底符号和输入终止符号
			case'#':
				isp = 0;
				icp = 0;
				break;
				//括号
			case'(':
				isp = 1;
				icp = 6;
				break;
			case')':
				isp = 6;
				icp = 1;
				break;
				//高级运算符
			case'*':
			case'/':
			case'%':
				isp = 5;
				icp = isp - 1;
				break;
				//普通运算符
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
			cout << "存储分配失败" << endl;
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

	Option bottom('#');                   //推入栈底符号
	changeStack.Push(bottom);
	input += " #";                        //输入终止符号

	int inLen = int(input.get_length());  //输入的表达式的长度

	for (int i = 0; i < inLen; i++) {
		if (input[i] != ' ') {            //没有读到空格，往后继续读入直至读到空格
			temp += input[i];
			continue;
		}
		else {                            //读到空格，执行入栈或者输出操作
			//非运算符
			if ((temp[0] >= '0' && temp[0] <= '9') || temp.get_length() >= 2)
			{
				if (temp[0] == '+')       //正数前自动省略正号
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

					while (ch.icp < top.isp)       //入栈优先级大于出栈优先级
					{
						Option out;
						changeStack.Pop(out);
						output += out.data;
						if (i != inLen - 1)
							output += " ";
						changeStack.getTop(top);
					}

					if (ch.icp > top.isp)          //入栈优先级大于出栈优先级
						changeStack.Push(ch);
					if (ch.icp == top.isp)         //退栈不输出
						changeStack.Pop(top);

				}
			}
			temp = "";                    //最后置空temp字符串
		}
	}

	while (!changeStack.isEmpty()) {        //遍历结束后若栈内还有元素，则清空栈
		Option out;
		changeStack.Pop(out);

		if (out.data != '#')                //栈底符号不输出
			output += out.data;
		if (!changeStack.isEmpty())
			output += " ";
	}
}

double to_beNum(MyString& change)
{
	double value = 0, calDecimal = 10;
	bool isDecimal = false;       //判断此时的转换是否为小数位的转换

	// 使用迭代器遍历字符串
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
		cout << "缺少右操作数！" << endl;
		return false;
	}

	calStack.Pop(right);

	if (calStack.isEmpty())
	{
		cout << "缺少左操作数！" << endl;
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
					cout << "除数不能为0！" << endl;
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
	//取操作数
	//根据运算符，对操作数进行运算
	//计算后将结果推入栈
	int outLen = int(output.get_length());

	MyString temp;
	for (int i = 0; i < outLen; i++) {
		if (output[i] != ' ') {
			temp += output[i];
			continue;
		}
		else {
			//非运算符
			if ((temp[0] >= '0' && temp[0] <= '9') || temp.get_length() >= 2)
				calStack.Push(to_beNum(temp));
			//运算符
			else
				DoOperator(temp);

			temp = "";   //最后置空temp字符串
		}
	}
}

void Caculate()
{
	Conversion();

	//操作完成后，栈内的最后一个元素就是表达式的结果
	calStack.getTop(res);
}

void printRes()
{
	cout << "转换后的后缀表达式：" << endl;
	cout << output << endl;

	//<<" \n"[i == outLen - 1];

	cout << "该后缀表达式的结果是：" << res << endl;
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