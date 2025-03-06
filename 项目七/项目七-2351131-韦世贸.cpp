#include <iostream>

using namespace std;
const int MaxSize = 10000;     //������󳤶�

//���ȼ����е��ඨ��
template<class T>
class PQueue
{
public:
    PQueue(int size = MaxSize)
        :maxsize(size),count(0)
    {
        pqelements = new T[size];
        if (pqelements == NULL) {
            cout << "�洢����ʧ�ܣ�" << endl;
            exit(1);
        }
    }

    ~PQueue()
    {
        delete[] pqelements;
    }

    bool isFull()
    {
        return count == maxsize;             //�ж϶����Ƿ���
    }

    bool isEmpty()
    {
        return count == 0;               //�ж϶����Ƿ��
    }

    //����Ԫ��
    bool Insert(T x)
    {
        //�����ȼ����в�������Ԫ�ز����β�������򣬷������
        if (isFull())
            return false;

        pqelements[count] = x;                   //�����β
        count++;
        adjust();                                //��������
        return true;
    }

    //ɾ����С��Ԫ�ز�ȡ����
    bool RemoveMin(T& x)        
    {
        if (isEmpty())            //����Ϊ�գ��޷�ɾ��Ԫ��
            return false;

        x = pqelements[0];

        int min = 1;
        //ȡ����СԪ�ص�λ��
        for (int i = 2; i < count; i++)
            if (pqelements[i] < pqelements[min])     
                min = i;   

        //������СԪ�ص�λ��
        pqelements[0] = pqelements[min];
        pqelements[min] = pqelements[count - 1];
        count--;                                     //���ȼ�����Ԫ�ظ���-1
        return true;
    }

    //ȡ�ö��г���
    int getLength()
    {
        return count;
    }
private:
    T* pqelements;
    int count;
    int maxsize;
    //��������Ԫ�ص�����˳��ʹ���С��������
    void adjust()
    {
        T temp = pqelements[count - 1];
        //�Ӷ�β��ʼ����
        int j;
        for (j = count - 2; j >= 0; j--) {
            //С�ڵ���temp����ѭ��
            if (pqelements[j] <= temp)
                break;
            //��temp����������һλ
            else
                pqelements[j + 1] = pqelements[j];
        }
        pqelements[j + 1] = temp;               //temp�����ʵ�λ��
    }
};

PQueue<int>Cost;              //���õ����ȼ�����
int total_cost;               //������

//����ÿ��ľͷ�ĳ���
void inputLength(int &N)
{
    int x;        //xΪÿ��ľͷ�ĳ���

    cout << "������ÿ��ľͷ�ĳ��ȣ��Կո���߻س��ָ�" << endl;
    for (int i = 0; i < N; ++i) {
        while (1) {
            cin >> x;
            if (cin.fail() || x >= 10000) {
                cout << "����ľͷ�����������������룡" << endl;
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

//���㻨��
void Calculate()
{
    while (Cost.getLength() > 1) {
        int x, y;
        Cost.RemoveMin(x);
        Cost.RemoveMin(y);
        total_cost += (x + y);
        Cost.Insert(x + y);
    }

    cout << "��Ҫ���ܻ���Ϊ��" << total_cost << endl;
    return;
}

int main() 
{
    int N;

    while (1)
    {
        cout << "����������N��N��10000������ʾҪ��ľͷ���N�飺";
        cin >> N;

        if (cin.fail() || N >= 10000) {
            cout << "����N�������������룡" << endl;
            cin.clear();
            cin.ignore(1024,'\n');

            continue;
        }
        else
            break;
    }

    if (N == 1) {              //��n=1,����Ҫ�κλ���
        cout << "��Ҫ���ܻ���Ϊ��" << total_cost << endl;
        return 0;
    }
    
    inputLength(N);
    Calculate();

    return 0;
}