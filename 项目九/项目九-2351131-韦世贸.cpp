#include <iostream>

using namespace std;

// ����ߵĽṹ�壬�������u���յ�v��Ȩ��w������˳��input_order
struct Edge
{
    int u, v, w, input_order;
};

// ��ʽ���кͽڵ�Ķ���
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

// ����u�����������u��ͬ����input_order��������
void sort_critical_activities(struct Edge* activities, int count)
{
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            // �����ǰ�ߵ�u������һ���ߵ�u������u��ͬ����ǰ�ߵ�input_orderС����һ���ߵ�input_order
            if (activities[j].u > activities[j + 1].u ||
                (activities[j].u == activities[j + 1].u && activities[j].input_order < activities[j + 1].input_order))
            {
                // ����������
                struct Edge temp = activities[j];
                activities[j] = activities[j + 1];
                activities[j + 1] = temp;
            }
        }
    }
}

// Ѱ�ҹؼ��
void search_critical_activities()
{
    LinkedQueue <int>topo_queque; // ������������Ķ���
    int N, M; // �ڵ����ͱ���

    cout << "��������������N��N��=100����M������N�����񽻽ӵ�����������ӵ㰴1��N��ţ�M�������������" << endl;

    cin >> N >> M; // ��ȡ�ڵ����ͱ���

    // ��̬���������ڴ�
    struct Edge *edges=new Edge[M]; // �洢���б�
    int* indegree = new int[N + 1]();// �洢ÿ���ڵ�����
    int* outdegree = new int[N + 1](); // �洢ÿ���ڵ�ĳ���
    int *earliest = new int[N + 1](); // �洢ÿ���ڵ���������ʱ��
    int *latest = new int[N + 1](); // �洢ÿ���ڵ���������ʱ��
    int *topo_order = new int[N + 1](); // �洢��������˳��

    // ��ȡ���бߵ���Ϣ����ͳ����Ⱥͳ���
    cout << "���������ʼ�������ƵĽ��ӵ����Լ���ɸ���������Ҫ��ʱ�䣬�������ÿո�ָ�: " << endl;
    for (int i = 0; i < M; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
        edges[i].input_order = i; // ��¼�ߵ�����˳��
        indegree[edges[i].v]++; // �յ���������
        outdegree[edges[i].u]++; // ���ĳ�������
    }
    cout << endl << endl;

    // ��ʼ������������У����������Ϊ0�Ľڵ�������
    for (int u = 1; u <= N; u++) {
        if (indegree[u] == 0) {
            topo_queque.EnQueue(u);
        }
    }

    int topo_len = 0; // ��������ĳ���
    // ִ����������
    while (!topo_queque.isEmpty()) {
        int u; // ȡ������ͷ���Ľڵ�
        topo_queque.DeQueue(u);
        topo_order[topo_len++] = u; // ���ýڵ������������˳��
        // �������бߣ��������
        for (int j = 0; j < M; j++) {
            if (edges[j].u == u) {
                indegree[edges[j].v]--; // �յ����ȼ�1
                if (indegree[edges[j].v] == 0) {
                    // ������Ϊ0���������
                    topo_queque.EnQueue(edges[j].v);
                }
            }
        }
    }

    // ����Ƿ��л�
    if (topo_len != N) {
        cout << "������Ȳ����У�" << endl;
        cout << 0 << endl; // �л������0
    }
    else {
        // ��ʼ���������ʱ��Ϊ0
        for (int u = 1; u <= N; u++) {
            earliest[u] = 0;
        }
        // ��������˳������������ʱ��
        for (int i = 0; i < topo_len; i++) {
            int u = topo_order[i];
            // �������д�u�����ıߣ������յ���������ʱ��
            for (int j = 0; j < M; j++) {
                if (edges[j].u == u) {
                    if (earliest[edges[j].v] < earliest[u] + edges[j].w) {
                        earliest[edges[j].v] = earliest[u] + edges[j].w;
                    }
                }
            }
        }

        int total_time = 0; // ��ʱ��
        // �ҵ������յ�ڵ���������ʱ������ֵ
        for (int u = 1; u <= N; u++) {
            if (outdegree[u] == 0) { // ������յ�ڵ�
                if (earliest[u] > total_time) {
                    total_time = earliest[u];
                }
            }
        }

        // ��ʼ���������ʱ��
        for (int u = 1; u <= N; u++) {
            if (outdegree[u] == 0) { // ������յ�ڵ�
                latest[u] = earliest[u];
            }
            else {
                latest[u] = 999999; // �����ڵ��ʼ��Ϊһ������
            }
        }

        // �Ӻ���ǰ�����������ʱ��
        for (int i = topo_len - 1; i >= 0; i--) {
            int u = topo_order[i];
            if (outdegree[u] == 0) {
                latest[u] = earliest[u]; // �յ�ڵ���������ʱ������������ʱ��
            }
            else {
                int min_latest = 9999999; // ��ʼ��Ϊһ������
                // �������д�u�����ıߣ��ҵ���С��latest[v] - w
                for (int j = 0; j < M; j++) {
                    if (edges[j].u == u) {
                        if (latest[edges[j].v] - edges[j].w < min_latest) {
                            min_latest = latest[edges[j].v] - edges[j].w;
                        }
                    }
                }
                latest[u] = min_latest; // ����u���������ʱ��
            }
        }

        struct Edge *critical_activities=new Edge[M]; // �洢�ؼ��
        int crit_count = 0; // �ؼ��������
        // �ҳ����йؼ����������latest[v] - earliest[u] - w == 0�ı�
        for (int i = 0; i < M; i++) {
            if (latest[edges[i].v] - earliest[edges[i].u] - edges[i].w == 0) {
                critical_activities[crit_count++] = edges[i];
            }
        }

        // �Թؼ����������
        sort_critical_activities(critical_activities, crit_count);

        // �����ʱ��
        cout << "���������Ŀ��Ҫ�����ʱ��: " << total_time << endl;
        cout << "���еĹؼ��: " << endl;
        // ������йؼ����u��v
        for (int i = 0; i < crit_count; i++) {
            cout << critical_activities[i].u << "->" << critical_activities[i].v << endl;
        }
    }
}

int main()
{
    search_critical_activities();

    return 0;
}