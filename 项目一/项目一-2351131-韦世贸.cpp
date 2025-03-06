#define _CRT_SECURE_NO_WARNINGS        //用了strcpy
#include <iostream>
#include <cstring>
//把 string 改写成 自己的类

using namespace std;

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

const char Tips[] = "考号     姓名     性别     年龄     报考类别";

struct Information                      //定义考生的信息
{
	int No;                             //考号
	MyString Name;                        //考生姓名
	MyString Sex;                         //考生性别
	int Age;                            //考生年龄
	MyString ApplyCategory;               //考生报考项目

	Information() :
		No(0), Name("蔡徐坤"), Sex("只因"), Age(25), ApplyCategory("练习生")
	{
	};                                 //带默认参数的构造函数

	void input()                        //用于输入信息
	{
		cin >> No;
		cin >> Name;
		cin >> Sex;
		cin >> Age;
		cin >> ApplyCategory;

		return;
	}
};

struct LinkNode                         //链表节点
{
	Information stuInformation;         //链表元素是考生信息
	LinkNode* link;                     //链表指针

	//friend Information;                 //在链表结点中可以访问信息结构体的元素

	LinkNode(LinkNode* ptr = NULL)      //输入信息
	{
		link = ptr;
	}
};

class Informationlist                   //信息链表
{
private:
	LinkNode* first;                    //链表头指针 

public:
	Informationlist()                   //构造函数
	{
		first = new LinkNode();         //附加头结点
	}

	~Informationlist()                  //析构函数
	{
		;
	}

	void CreateNewList(int n)               //建立新链表（后插法）
	{
		LinkNode* newNode, * last;

		last = first;

		for (int i = 1; i <= n; i++) {     //按顺序输入元素
			newNode = new LinkNode();
			newNode->stuInformation.input();

			if (newNode == NULL) {
				cout << "存储分配错误！" << endl;
				return;
			}

			last->link = newNode;          //尾指针指向新结点，连接起新结点
			last = newNode;                //更新尾指针的位置
		}

		last->link = NULL;                 //链表结尾
		return;
	}

	bool Insert(int location)     //结点插入函数,插入后的元素成为链表中的第location个元素
	{
		LinkNode* current = first->link;     //从第一个结点开始检测

		for (int i = 1; i < location - 1; i++) {      //找到目标位置的前一个结点
			if (current == NULL)
				break;
			else
				current = current->link;
		}

		if (current == NULL) {
			cout << "无效的插入位置" << endl;        //非空表但链太短
			return false;
		}
		else {
			LinkNode* newNode = new LinkNode();
			newNode->stuInformation.input();

			if (newNode == NULL) {
				cout << "存储分配错误！" << endl;
				return false;
			}

			newNode->link = current->link;
			current->link = newNode;

			return true;
		}
	}

	bool Remove(int NO, int& location)      //结点删除函数，删除考号为no的元素
	{
		LinkNode* del, * current = first;

		while (current->link != NULL) {
			if (current->link->stuInformation.No == NO) {
				cout << "你删除的考生信息是：";                   //打印提示
				cout << current->link->stuInformation.No << "        " << current->link->stuInformation.Name << "     "
					<< current->link->stuInformation.Sex << "       " << current->link->stuInformation.Age << "       "
					<< current->link->stuInformation.ApplyCategory << endl;

				break;
			}
			else
			{
				current = current->link;
				location++;                                //同时能够找到结点在链表的逻辑位置
			}
		}

		if (current == NULL || current->link == NULL) {    //删除不成功
			cout << "无效的删除位置！" << endl;
			return false;
		}

		del = current->link;
		current->link = del->link;

		delete del;

		return true;
	}

	bool Change(int NO)
	{
		LinkNode* current = first;

		while (current != NULL) {
			if (current->stuInformation.No == NO) {                   //找到了目标考生
				cout << "请输入要修改的考生信息：";                   //输入提示
				cin >> current->stuInformation.No >> current->stuInformation.Name >>
					current->stuInformation.Sex >> current->stuInformation.Age >>
					current->stuInformation.ApplyCategory;

				return true;
			}
			else
			{
				current = current->link;
			}
		}

		if (current == NULL || current->link == NULL) {    //修改不成功
			cout << "无效的修改位置！" << endl;
			return false;
		}

		return true;
	}

	void output()                       //按逻辑输出链表的元素
	{
		LinkNode* current = first->link;

		while (current != NULL) {
			cout << current->stuInformation.No << "        " << current->stuInformation.Name << "     "
				<< current->stuInformation.Sex << "       " << current->stuInformation.Age << "       "
				<< current->stuInformation.ApplyCategory << endl;

			current = current->link;
		}
	}

	void Search(int NO)                 //查找元素
	{
		LinkNode* current = first->link;

		while (current != NULL) {
			if (current->stuInformation.No == NO) {
				cout << Tips << endl;                                        //打印信息类别
				cout << current->stuInformation.No << "        " << current->stuInformation.Name << "     "
					<< current->stuInformation.Sex << "       " << current->stuInformation.Age << "       "
					<< current->stuInformation.ApplyCategory << endl;

				return;
			}
			else
				current = current->link;
		}

		cout << "未找到该学生！" << endl;
		return;
	}
};


void printInformation(Informationlist inforlist)                 //打印系统当前保存的信息
{
	cout << Tips << endl;                                        //打印信息类别
	inforlist.output();                                          //从链表中打印
}

Informationlist setSystem()                                                  //建立考生信息系统
{
	int num;

	cout << "首先请建立考生信息系统！" << endl;
	cout << "请输入考生人数：";

	while(1)                                            //输入错误处理
	{
		cin >> num;
		if (num <= 0 || cin.fail())
		{
			cin.clear();
			cin.ignore(65472, '\n');                    //清空缓冲区
			cout << "考生人数必须大于0！" << endl;
		}
		else
			break;
	}

	cout << "请依次输入考生的考号，姓名，性别，年龄及报考类别！" << endl;

	Informationlist inforlist;

	inforlist.CreateNewList(num);              //建立新链表

	printInformation(inforlist);
	cout << "请选择您要进行的操作（1为插入，2为删除，3为查找，4为修改，5为统计，0为取消操作）" << endl;
	cout << endl;

	return inforlist;
}

int	Op_input()
{
	int op = 1;

	cout << "请选择您要进行的操作：";
	cin >> op;
	
	return op;
}

void Operate(Informationlist &inforlist, int op)
{
	int location = 0, no = 0;

	switch (op)
	{
		case 0:
			cout << "操作结束！" << endl;
			cout << "已退出系统！" << endl;
			return;
		case 1:
			cout << "请输入您要插入的考生的位置：";
			cin >> location;

			if (location <= 0)
				cout << "位置输入不正确！";
			else {
				cout << "请依次输入要插入的考生的考号，姓名，性别，年龄及报考类别！"<< endl;
				inforlist.Insert(location);
				cout << endl;
				printInformation(inforlist);
			}
			break;
		case 2:
			cout << "请输入您要删除的考生的考号：";
			cin >> no;

			if (no <= 0)
				cout << "考号必须大于0！" << endl;
			else {
				inforlist.Remove(no,location);
				cout << endl;
				printInformation(inforlist);
			}
			break;
		case 3:
			cout << "请输入您要查找的考生的考号：";
			cin >> no;

			if (no <= 0)
				cout << "考号必须大于0！" << endl;
			else {
				inforlist.Search(no);
			}
			break;
		case 4:
			cout << "请输入您要修改的考生的考号：";
			cin >> no;

			if (no <= 0)
				cout << "考号必须大于0！" << endl;
			else {
				inforlist.Change(no);                            //修改考生信息
				cout << endl;
				printInformation(inforlist);
			}
			break;
		case 5:                                                  //统计
			cout << "系统中的考生信息如下：" << endl;
			printInformation(inforlist);
			break;
		default:
			cout << "输入的选项不存在！" << endl;
			break;

	}
}

int main()
{
	Informationlist systemList = setSystem();                 //建立系统
	
	int option = 1;

	while (option) {
		option = Op_input();
		Operate(systemList, option);
	}

	return 0;
}