#define _CRT_SECURE_NO_WARNINGS        //用了strcpy
#include <iostream>

using namespace std;

//自定义string类，功能与string相似
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
    friend bool operator==(const MyString& lhs, const MyString& rhs); //重载比较运算符
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

bool operator==(const MyString& lhs, const MyString& rhs)
{
    if (lhs.get_length() != rhs.get_length())
        return false;
    return strcmp(lhs.m_data, rhs.m_data) == 0;
}

//链式结点定义
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

//使用队列对树进行遍历
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

//树节点定义
template <class T>
class TreeNode
{
    template <class T>
    friend class MultiTree;
private:
    T data;
    TreeNode<T>* firstChild;          //子结点
    TreeNode<T>* nextSibling;         //兄弟结点
    TreeNode<T>* parent;              //双亲结点

    TreeNode(T& value) : data(value), firstChild(nullptr), nextSibling(nullptr), parent(nullptr) {}
};

//多叉树定义
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

    //建立一棵树
    void Creat()
    {
        cout << "请输入祖先的姓名：";
        T rootname;

        cin >> rootname;

        TreeNode<T>* New = new TreeNode<T>(rootname);

        if (New == NULL) {
            cout << "存储分配错误！" << endl;
            return;
        }

        root = New;
        cout << "此家谱的祖先是：" << New->data << endl;
    }

    //对多叉树进行中序遍历查找
    TreeNode<T>* Lookingfor(T data, TreeNode<T>* p)
    {
        TreeNode<T>* res = NULL;             //返回结果
        LinkedStack<TreeNode<T>*> TreeStack;    //用栈存储结点

        while (p || !TreeStack.isEmpty())       //遍历条件
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
        // 输出双亲结点
        if (f->parent != NULL)
            cout << f->data << "的最近一代祖先是" << f->parent->data << " " << endl;
        else
            cout << f->data << "是这个家族的第一代" << endl;

        if (f->firstChild != nullptr) {
            cout << f->data << "的第一代子孙是" << f->firstChild->data << " ";  // 打印第一个子女成员
            f = f->firstChild;

            while (f->nextSibling != NULL) {  // 打印该子女的兄弟成员
                cout << f->nextSibling->data << " ";
                f = f->nextSibling;
            }
        }
        cout << endl;
    }

    //完善家谱
    bool Add()
    {
        cout << "请输入需要建立家庭的人的姓名：";

        T parent_data;

        cin >> parent_data;

        TreeNode<T>* f = Lookingfor(parent_data, root);

        if (f != NULL) {
            TreeNode<T>* p = f;
            cout << "请输入" << p->data << "的儿女个数：";

            int num;

            cin >> num;

            cout << "请依次输入" << p->data << "的儿女名字" << endl;

            for (int i = 1; i <= num; i++)
            {
                T child_data;
                cin >> child_data;

                TreeNode<T>* New = new TreeNode<T>(child_data);

                if (New == NULL) {
                    cout << "存储分配错误！" << endl;
                    return false;
                }

                if (i == 1) {             //第一个子女
                    p->firstChild = New;  //连接到p的子女结点
                    New->parent = p;      //更新p的位置
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
            cout << "查无此人！" << endl;
            return false;
        }
    }

    //添加家庭成员
    bool Insert()        //只添加一个子女结点
    {
        cout << "请输入要添加儿子或女儿的人的姓名：";
        T parent_data;
        cin >> parent_data;

        TreeNode<T>* res = Lookingfor(parent_data, root);

        if (res != NULL)
        {
            TreeNode<T>* temp = res;
            cout << "请输入" << res->data << "新添加子女的姓名：";

            T child_data;
            cin >> child_data;

            TreeNode<T>* New = new TreeNode<T>(child_data);
            if (New == NULL) {
                cout << "存储分配错误！" << endl;
                return false;
            }

            if (res->firstChild == NULL)           //双亲结点的子女结点为空，直接将插入结点作为父节点的子女结点
            {
                res->firstChild = New;
                New->parent = res;
            }
            else                                   //否则放置在子女结点的兄弟结点位置
            {
                res = res->firstChild;

                while (res->nextSibling != NULL)
                    res = res->nextSibling;

                res->nextSibling = New;
                New->parent = res->parent;         //记录双亲
            }

            print_Child(temp);
            return true;
        }
        else {
            cout << "查无此人，请重新输入！" << endl;
            return false;
        }
    }

    //修改家庭成员的名字
    bool ChangeName()          //更改某个人的姓名
    {
        cout << "请输入要更改姓名的人的目前姓名：";

        T now_data;
        cin >> now_data;

        TreeNode<T>* res = Lookingfor(now_data, root);

        if (res != NULL)
        {
            cout << "请输入更改后的姓名：";
            T new_data;
            cin >> new_data;

            res->data = new_data;
            cout << now_data << "已经改名为" << res->data << endl;

            return true;
        }
        else {
            cout << "查无此人，请重新输入！" << endl;
            return false;
        }
    }

    //解散家庭
    bool Disband()         //解散家庭
    {
        cout << "请输入要解散家庭的人的姓名：";
        T parent_data;
        cin >> parent_data;

        TreeNode<T>* res = Lookingfor(parent_data, root);

        if (res != NULL)
        {
            if (res->firstChild != NULL)                              //如果该结点存在子女，则解散家庭
            {
                cout << "要解散家庭的人是：" << res->data << endl;
                print_Child(res);
                DeleteSubtree(res->firstChild);
                res->firstChild = NULL;
            }
            else                                                      //如果没有子女就还未有家庭
                cout << res->data << "尚未有家庭";

            return true;
        }
        else
        {
            cout << "查无此人，请重新输入！" << endl;
            return false;
        }
    }
};

MultiTree <MyString> FamilyTree;

void  gotoSystem()
{
    cout << "**         家谱管理系统           **" << endl;
    cout << "====================================" << endl;
    cout << "**      请选择要执行的操作：      **" << endl;
    cout << "**      A --- 完善家谱            **" << endl;
    cout << "**      B --- 添加家庭成员        **" << endl;
    cout << "**      C --- 解散局部家庭        **" << endl;
    cout << "**      D --- 更改家庭成员姓名    **" << endl;
    cout << "**      E --- 退出程序            **" << endl;
    cout << "====================================" << endl << endl;

    cout << "首先建立一个家谱！" << endl;

    FamilyTree.Creat(); //建立家谱

    char sel = 0;

    while (sel != 'E') {
        cout << endl << "请选择要执行的操作：";

        cin >> sel;

        switch (sel)
        {
            //因为这些成员函数为布尔函数，所以可以直接使用返回值进行错误处理
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
                cout << "操作结束，已退出系统！" << endl;
                break;
            default:
                cout << "请输入正确的操作！" << endl;
                break;
        }
    }
}

int main()
{
	gotoSystem();


	return 0;
}