#include <iostream>

using namespace std;
const int MaxSize = 10000;     //定义最大长度

//优先级队列的类定义
template<class T>
class PQueue
{
public:
    PQueue(int size = MaxSize)
        :maxsize(size),count(0)
    {
        pqelements = new T[size];
        if (pqelements == NULL) {
            cout << "存储分配失败！" << endl;
            exit(1);
        }
    }

    ~PQueue()
    {
        delete[] pqelements;
    }

    bool isFull()
    {
        return count == maxsize;             //判断队列是否满
    }

    bool isEmpty()
    {
        return count == 0;               //判断队列是否空
    }

    //插入元素
    bool Insert(T x)
    {
        //若优先级队列不满，则将元素插入队尾，再排序，否则出错
        if (isFull())
            return false;

        pqelements[count] = x;                   //插入队尾
        count++;
        adjust();                                //调整排序
        return true;
    }

    //删除最小的元素并取得它
    bool RemoveMin(T& x)        
    {
        if (isEmpty())            //队列为空，无法删除元素
            return false;

        x = pqelements[0];

        int min = 1;
        //取得最小元素的位置
        for (int i = 2; i < count; i++)
            if (pqelements[i] < pqelements[min])     
                min = i;   

        //更改最小元素的位置
        pqelements[0] = pqelements[min];
        pqelements[min] = pqelements[count - 1];
        count--;                                     //优先级队列元素个数-1
        return true;
    }

    //取得队列长度
    int getLength()
    {
        return count;
    }
private:
    T* pqelements;
    int count;
    int maxsize;
    //调整所有元素的排列顺序，使其从小到大排列
    void adjust()
    {
        T temp = pqelements[count - 1];
        //从队尾开始排序
        int j;
        for (j = count - 2; j >= 0; j--) {
            //小于等于temp跳出循环
            if (pqelements[j] <= temp)
                break;
            //比temp大则往后移一位
            else
                pqelements[j + 1] = pqelements[j];
        }
        pqelements[j + 1] = temp;               //temp插入适当位置
    }
};

PQueue<int>Cost;              //费用的优先级队列
int total_cost;               //总消费

//输入每段木头的长度
void inputLength(int &N)
{
    int x;        //x为每段木头的长度

    cout << "请输入每段木头的长度，以空格或者回车分隔" << endl;
    for (int i = 0; i < N; ++i) {
        while (1) {
            cin >> x;
            if (cin.fail() || x >= 10000) {
                cout << "输入木头长度有误，请重新输入！" << endl;
                cin.clear();
                cin.ignore(1024,'\n');

                continue;
            }
            else
                break;
        }

        Cost.Insert(x);
    }
}

//计算花费
void Calculate()
{
    while (Cost.getLength() > 1) {
        int x, y;
        Cost.RemoveMin(x);
        Cost.RemoveMin(y);
        total_cost += (x + y);
        Cost.Insert(x + y);
    }

    cout << "需要的总花费为：" << total_cost << endl;
    return;
}

int main() 
{
    int N;

    while (1)
    {
        cout << "给出正整数N（N《10000），表示要将木头锯成N块：";
        cin >> N;

        if (cin.fail() || N >= 10000) {
            cout << "输入N有误，请重新输入！" << endl;
            cin.clear();
            cin.ignore(1024,'\n');

            continue;
        }
        else
            break;
    }

    if (N == 1) {              //若n=1,则不需要任何花费
        cout << "需要的总花费为：" << total_cost << endl;
        return 0;
    }
    
    inputLength(N);
    Calculate();

    return 0;
}