#define _CRT_SECURE_NO_WARNINGS        //����strcpy
#include <iostream>
#include <cstring>
//�� string ��д�� �Լ�����

using namespace std;

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

const char Tips[] = "����     ����     �Ա�     ����     �������";

struct Information                      //���忼������Ϣ
{
	int No;                             //����
	MyString Name;                        //��������
	MyString Sex;                         //�����Ա�
	int Age;                            //��������
	MyString ApplyCategory;               //����������Ŀ

	Information() :
		No(0), Name("������"), Sex("ֻ��"), Age(25), ApplyCategory("��ϰ��")
	{
	};                                 //��Ĭ�ϲ����Ĺ��캯��

	void input()                        //����������Ϣ
	{
		cin >> No;
		cin >> Name;
		cin >> Sex;
		cin >> Age;
		cin >> ApplyCategory;

		return;
	}
};

struct LinkNode                         //����ڵ�
{
	Information stuInformation;         //����Ԫ���ǿ�����Ϣ
	LinkNode* link;                     //����ָ��

	//friend Information;                 //���������п��Է�����Ϣ�ṹ���Ԫ��

	LinkNode(LinkNode* ptr = NULL)      //������Ϣ
	{
		link = ptr;
	}
};

class Informationlist                   //��Ϣ����
{
private:
	LinkNode* first;                    //����ͷָ�� 

public:
	Informationlist()                   //���캯��
	{
		first = new LinkNode();         //����ͷ���
	}

	~Informationlist()                  //��������
	{
		;
	}

	void CreateNewList(int n)               //������������巨��
	{
		LinkNode* newNode, * last;

		last = first;

		for (int i = 1; i <= n; i++) {     //��˳������Ԫ��
			newNode = new LinkNode();
			newNode->stuInformation.input();

			if (newNode == NULL) {
				cout << "�洢�������" << endl;
				return;
			}

			last->link = newNode;          //βָ��ָ���½�㣬�������½��
			last = newNode;                //����βָ���λ��
		}

		last->link = NULL;                 //�����β
		return;
	}

	bool Insert(int location)     //�����뺯��,������Ԫ�س�Ϊ�����еĵ�location��Ԫ��
	{
		LinkNode* current = first->link;     //�ӵ�һ����㿪ʼ���

		for (int i = 1; i < location - 1; i++) {      //�ҵ�Ŀ��λ�õ�ǰһ�����
			if (current == NULL)
				break;
			else
				current = current->link;
		}

		if (current == NULL) {
			cout << "��Ч�Ĳ���λ��" << endl;        //�ǿձ���̫��
			return false;
		}
		else {
			LinkNode* newNode = new LinkNode();
			newNode->stuInformation.input();

			if (newNode == NULL) {
				cout << "�洢�������" << endl;
				return false;
			}

			newNode->link = current->link;
			current->link = newNode;

			return true;
		}
	}

	bool Remove(int NO, int& location)      //���ɾ��������ɾ������Ϊno��Ԫ��
	{
		LinkNode* del, * current = first;

		while (current->link != NULL) {
			if (current->link->stuInformation.No == NO) {
				cout << "��ɾ���Ŀ�����Ϣ�ǣ�";                   //��ӡ��ʾ
				cout << current->link->stuInformation.No << "        " << current->link->stuInformation.Name << "     "
					<< current->link->stuInformation.Sex << "       " << current->link->stuInformation.Age << "       "
					<< current->link->stuInformation.ApplyCategory << endl;

				break;
			}
			else
			{
				current = current->link;
				location++;                                //ͬʱ�ܹ��ҵ������������߼�λ��
			}
		}

		if (current == NULL || current->link == NULL) {    //ɾ�����ɹ�
			cout << "��Ч��ɾ��λ�ã�" << endl;
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
			if (current->stuInformation.No == NO) {                   //�ҵ���Ŀ�꿼��
				cout << "������Ҫ�޸ĵĿ�����Ϣ��";                   //������ʾ
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

		if (current == NULL || current->link == NULL) {    //�޸Ĳ��ɹ�
			cout << "��Ч���޸�λ�ã�" << endl;
			return false;
		}

		return true;
	}

	void output()                       //���߼���������Ԫ��
	{
		LinkNode* current = first->link;

		while (current != NULL) {
			cout << current->stuInformation.No << "        " << current->stuInformation.Name << "     "
				<< current->stuInformation.Sex << "       " << current->stuInformation.Age << "       "
				<< current->stuInformation.ApplyCategory << endl;

			current = current->link;
		}
	}

	void Search(int NO)                 //����Ԫ��
	{
		LinkNode* current = first->link;

		while (current != NULL) {
			if (current->stuInformation.No == NO) {
				cout << Tips << endl;                                        //��ӡ��Ϣ���
				cout << current->stuInformation.No << "        " << current->stuInformation.Name << "     "
					<< current->stuInformation.Sex << "       " << current->stuInformation.Age << "       "
					<< current->stuInformation.ApplyCategory << endl;

				return;
			}
			else
				current = current->link;
		}

		cout << "δ�ҵ���ѧ����" << endl;
		return;
	}
};


void printInformation(Informationlist inforlist)                 //��ӡϵͳ��ǰ�������Ϣ
{
	cout << Tips << endl;                                        //��ӡ��Ϣ���
	inforlist.output();                                          //�������д�ӡ
}

Informationlist setSystem()                                                  //����������Ϣϵͳ
{
	int num;

	cout << "�����뽨��������Ϣϵͳ��" << endl;
	cout << "�����뿼��������";

	while(1)                                            //���������
	{
		cin >> num;
		if (num <= 0 || cin.fail())
		{
			cin.clear();
			cin.ignore(65472, '\n');                    //��ջ�����
			cout << "���������������0��" << endl;
		}
		else
			break;
	}

	cout << "���������뿼���Ŀ��ţ��������Ա����估�������" << endl;

	Informationlist inforlist;

	inforlist.CreateNewList(num);              //����������

	printInformation(inforlist);
	cout << "��ѡ����Ҫ���еĲ�����1Ϊ���룬2Ϊɾ����3Ϊ���ң�4Ϊ�޸ģ�5Ϊͳ�ƣ�0Ϊȡ��������" << endl;
	cout << endl;

	return inforlist;
}

int	Op_input()
{
	int op = 1;

	cout << "��ѡ����Ҫ���еĲ�����";
	cin >> op;
	
	return op;
}

void Operate(Informationlist &inforlist, int op)
{
	int location = 0, no = 0;

	switch (op)
	{
		case 0:
			cout << "����������" << endl;
			cout << "���˳�ϵͳ��" << endl;
			return;
		case 1:
			cout << "��������Ҫ����Ŀ�����λ�ã�";
			cin >> location;

			if (location <= 0)
				cout << "λ�����벻��ȷ��";
			else {
				cout << "����������Ҫ����Ŀ����Ŀ��ţ��������Ա����估�������"<< endl;
				inforlist.Insert(location);
				cout << endl;
				printInformation(inforlist);
			}
			break;
		case 2:
			cout << "��������Ҫɾ���Ŀ����Ŀ��ţ�";
			cin >> no;

			if (no <= 0)
				cout << "���ű������0��" << endl;
			else {
				inforlist.Remove(no,location);
				cout << endl;
				printInformation(inforlist);
			}
			break;
		case 3:
			cout << "��������Ҫ���ҵĿ����Ŀ��ţ�";
			cin >> no;

			if (no <= 0)
				cout << "���ű������0��" << endl;
			else {
				inforlist.Search(no);
			}
			break;
		case 4:
			cout << "��������Ҫ�޸ĵĿ����Ŀ��ţ�";
			cin >> no;

			if (no <= 0)
				cout << "���ű������0��" << endl;
			else {
				inforlist.Change(no);                            //�޸Ŀ�����Ϣ
				cout << endl;
				printInformation(inforlist);
			}
			break;
		case 5:                                                  //ͳ��
			cout << "ϵͳ�еĿ�����Ϣ���£�" << endl;
			printInformation(inforlist);
			break;
		default:
			cout << "�����ѡ����ڣ�" << endl;
			break;

	}
}

int main()
{
	Informationlist systemList = setSystem();                 //����ϵͳ
	
	int option = 1;

	while (option) {
		option = Op_input();
		Operate(systemList, option);
	}

	return 0;
}