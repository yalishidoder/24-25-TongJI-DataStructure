#define _CRT_SECURE_NO_WARNINGS        //����strcpy
#include <iostream>

using namespace std;

//�Զ���string�࣬������string����
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
    friend bool operator==(const MyString& lhs, const MyString& rhs); //���رȽ������
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

bool operator==(const MyString& lhs, const MyString& rhs)
{
    if (lhs.get_length() != rhs.get_length())
        return false;
    return strcmp(lhs.m_data, rhs.m_data) == 0;
}

//��ʽ��㶨��
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

//ʹ�ö��ж������б���
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

//���ڵ㶨��
template <class T>
class TreeNode
{
    template <class T>
    friend class MultiTree;
private:
    T data;
    TreeNode<T>* firstChild;          //�ӽ��
    TreeNode<T>* nextSibling;         //�ֵܽ��
    TreeNode<T>* parent;              //˫�׽��

    TreeNode(T& value) : data(value), firstChild(nullptr), nextSibling(nullptr), parent(nullptr) {}
};

//���������
template <class T>
class MultiTree
{
private:
    TreeNode<T>* root;

public:
    MultiTree() : root(NULL) {}

    ~MultiTree()
    {
    }

    void DeleteSubtree(TreeNode<T>* node)
    {
        if (node != NULL)
        {
            DeleteSubtree(node->firstChild);
            DeleteSubtree(node->nextSibling);
            delete node;
        }
    }

    //����һ����
    void Creat()
    {
        cout << "���������ȵ�������";
        T rootname;

        cin >> rootname;

        TreeNode<T>* New = new TreeNode<T>(rootname);

        if (New == NULL) {
            cout << "�洢�������" << endl;
            return;
        }

        root = New;
        cout << "�˼��׵������ǣ�" << New->data << endl;
    }

    //�Զ�������������������
    TreeNode<T>* Lookingfor(T data, TreeNode<T>* p)
    {
        TreeNode<T>* res = NULL;             //���ؽ��
        LinkedStack<TreeNode<T>*> TreeStack;    //��ջ�洢���

        while (p || !TreeStack.isEmpty())       //��������
        {
            if (p) {
                TreeStack.Push(p);
                p = p->firstChild;
            }
            else {
                TreeStack.Pop(p);

                if (p->data == data)
                {
                    res = p;
                    return res;
                }

                p = p->nextSibling;
            }
        }

        return res;
    }

    void print_Child(TreeNode<T>* f)
    {
        // ���˫�׽��
        if (f->parent != NULL)
            cout << f->data << "�����һ��������" << f->parent->data << " " << endl;
        else
            cout << f->data << "���������ĵ�һ��" << endl;

        if (f->firstChild != nullptr) {
            cout << f->data << "�ĵ�һ��������" << f->firstChild->data << " ";  // ��ӡ��һ����Ů��Ա
            f = f->firstChild;

            while (f->nextSibling != NULL) {  // ��ӡ����Ů���ֵܳ�Ա
                cout << f->nextSibling->data << " ";
                f = f->nextSibling;
            }
        }
        cout << endl;
    }

    //���Ƽ���
    bool Add()
    {
        cout << "��������Ҫ������ͥ���˵�������";

        T parent_data;

        cin >> parent_data;

        TreeNode<T>* f = Lookingfor(parent_data, root);

        if (f != NULL) {
            TreeNode<T>* p = f;
            cout << "������" << p->data << "�Ķ�Ů������";

            int num;

            cin >> num;

            cout << "����������" << p->data << "�Ķ�Ů����" << endl;

            for (int i = 1; i <= num; i++)
            {
                T child_data;
                cin >> child_data;

                TreeNode<T>* New = new TreeNode<T>(child_data);

                if (New == NULL) {
                    cout << "�洢�������" << endl;
                    return false;
                }

                if (i == 1) {             //��һ����Ů
                    p->firstChild = New;  //���ӵ�p����Ů���
                    New->parent = p;      //����p��λ��
                    p = p->firstChild;
                }
                else {
                    p->nextSibling = New;
                    New->parent = p->parent;
                    p = p->nextSibling;
                }
            }

            print_Child(f);
            return true;
        }
        else {
            cout << "���޴��ˣ�" << endl;
            return false;
        }
    }

    //��Ӽ�ͥ��Ա
    bool Insert()        //ֻ���һ����Ů���
    {
        cout << "������Ҫ��Ӷ��ӻ�Ů�����˵�������";
        T parent_data;
        cin >> parent_data;

        TreeNode<T>* res = Lookingfor(parent_data, root);

        if (res != NULL)
        {
            TreeNode<T>* temp = res;
            cout << "������" << res->data << "�������Ů��������";

            T child_data;
            cin >> child_data;

            TreeNode<T>* New = new TreeNode<T>(child_data);
            if (New == NULL) {
                cout << "�洢�������" << endl;
                return false;
            }

            if (res->firstChild == NULL)           //˫�׽�����Ů���Ϊ�գ�ֱ�ӽ���������Ϊ���ڵ����Ů���
            {
                res->firstChild = New;
                New->parent = res;
            }
            else                                   //�����������Ů�����ֵܽ��λ��
            {
                res = res->firstChild;

                while (res->nextSibling != NULL)
                    res = res->nextSibling;

                res->nextSibling = New;
                New->parent = res->parent;         //��¼˫��
            }

            print_Child(temp);
            return true;
        }
        else {
            cout << "���޴��ˣ����������룡" << endl;
            return false;
        }
    }

    //�޸ļ�ͥ��Ա������
    bool ChangeName()          //����ĳ���˵�����
    {
        cout << "������Ҫ�����������˵�Ŀǰ������";

        T now_data;
        cin >> now_data;

        TreeNode<T>* res = Lookingfor(now_data, root);

        if (res != NULL)
        {
            cout << "��������ĺ��������";
            T new_data;
            cin >> new_data;

            res->data = new_data;
            cout << now_data << "�Ѿ�����Ϊ" << res->data << endl;

            return true;
        }
        else {
            cout << "���޴��ˣ����������룡" << endl;
            return false;
        }
    }

    //��ɢ��ͥ
    bool Disband()         //��ɢ��ͥ
    {
        cout << "������Ҫ��ɢ��ͥ���˵�������";
        T parent_data;
        cin >> parent_data;

        TreeNode<T>* res = Lookingfor(parent_data, root);

        if (res != NULL)
        {
            if (res->firstChild != NULL)                              //����ý�������Ů�����ɢ��ͥ
            {
                cout << "Ҫ��ɢ��ͥ�����ǣ�" << res->data << endl;
                print_Child(res);
                DeleteSubtree(res->firstChild);
                res->firstChild = NULL;
            }
            else                                                      //���û����Ů�ͻ�δ�м�ͥ
                cout << res->data << "��δ�м�ͥ";

            return true;
        }
        else
        {
            cout << "���޴��ˣ����������룡" << endl;
            return false;
        }
    }
};

MultiTree <MyString> FamilyTree;

void  gotoSystem()
{
    cout << "**         ���׹���ϵͳ           **" << endl;
    cout << "====================================" << endl;
    cout << "**      ��ѡ��Ҫִ�еĲ�����      **" << endl;
    cout << "**      A --- ���Ƽ���            **" << endl;
    cout << "**      B --- ��Ӽ�ͥ��Ա        **" << endl;
    cout << "**      C --- ��ɢ�ֲ���ͥ        **" << endl;
    cout << "**      D --- ���ļ�ͥ��Ա����    **" << endl;
    cout << "**      E --- �˳�����            **" << endl;
    cout << "====================================" << endl << endl;

    cout << "���Ƚ���һ�����ף�" << endl;

    FamilyTree.Creat(); //��������

    char sel = 0;

    while (sel != 'E') {
        cout << endl << "��ѡ��Ҫִ�еĲ�����";

        cin >> sel;

        switch (sel)
        {
            //��Ϊ��Щ��Ա����Ϊ�������������Կ���ֱ��ʹ�÷���ֵ���д�����
            case'A':
                while (!FamilyTree.Add());
                break;
            case'B':
                while (!FamilyTree.Insert());
                break;
            case'C':
                while (!FamilyTree.Disband());
                break;
            case'D':
                while (!FamilyTree.ChangeName());
                break;
            case'E':
                cout << "�������������˳�ϵͳ��" << endl;
                break;
            default:
                cout << "��������ȷ�Ĳ�����" << endl;
                break;
        }
    }
}

int main()
{
	gotoSystem();


	return 0;
}