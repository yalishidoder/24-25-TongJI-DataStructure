#include <iostream>

using namespace std;

// 定义边的结构体，包含起点u、终点v、权重w和输入顺序input_order
struct Edge
{
    int u, v, w, input_order;
};

// 链式队列和节点的定义
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

    //进队列
    bool EnQueue(T x)
    {
        if (front == NULL) {
            //当队列为空时，新结点作为队列的第一个结点，同时作为队首和队尾
            front = rear = new LinkNode<T>(x);
            if (front == NULL) {
                cout << "存储分配失败！" << endl;
                return false;
            }
        }
        else {
            //队列不空，将新结点接到队尾后
            rear->link = new LinkNode<T>(x);
            if (rear->link == NULL) {
                cout << "存储分配失败！" << endl;
                return false;
            }

            rear = rear->link;     //更新队尾
        }

        return true;
    }

    //出队列
    bool DeQueue(T& x)
    {
        if (isEmpty())       //队列为空
            return false;

        LinkNode<T>* del = front;
        x = del->data;

        front = front->link;
        delete del;

        return true;
    }

    //取得队首元素
    bool getFront(T& x)
    {
        if (isEmpty())       //队列为空
            return false;

        x = front->data;

        return true;
    }

    //取得队列长度
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

    //输出运算符重载
    template<class T>
    friend ostream& operator<< (ostream& os, LinkedQueue<T>& Queue);
private:
    LinkNode<T>* front, * rear;
};

// 按照u升序排序，如果u相同，则按input_order降序排序
void sort_critical_activities(struct Edge* activities, int count)
{
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            // 如果当前边的u大于下一条边的u，或者u相同但当前边的input_order小于下一条边的input_order
            if (activities[j].u > activities[j + 1].u ||
                (activities[j].u == activities[j + 1].u && activities[j].input_order < activities[j + 1].input_order))
            {
                // 交换两条边
                struct Edge temp = activities[j];
                activities[j] = activities[j + 1];
                activities[j + 1] = temp;
            }
        }
    }
}

// 寻找关键活动
void search_critical_activities()
{
    LinkedQueue <int>topo_queque; // 用于拓扑排序的队列
    int N, M; // 节点数和边数

    cout << "给出两个正整数N（N《=100）和M，其中N是任务交接点的数量，交接点按1～N编号，M是子任务的数量" << endl;

    cin >> N >> M; // 读取节点数和边数

    // 动态分配数组内存
    struct Edge *edges=new Edge[M]; // 存储所有边
    int* indegree = new int[N + 1]();// 存储每个节点的入度
    int* outdegree = new int[N + 1](); // 存储每个节点的出度
    int *earliest = new int[N + 1](); // 存储每个节点的最早完成时间
    int *latest = new int[N + 1](); // 存储每个节点的最晚完成时间
    int *topo_order = new int[N + 1](); // 存储拓扑排序顺序

    // 读取所有边的信息，并统计入度和出度
    cout << "输入该任务开始和完成设计的交接点编号以及完成该任务所需要的时间，整数间用空格分隔: " << endl;
    for (int i = 0; i < M; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
        edges[i].input_order = i; // 记录边的输入顺序
        indegree[edges[i].v]++; // 终点的入度增加
        outdegree[edges[i].u]++; // 起点的出度增加
    }
    cout << endl << endl;

    // 初始化拓扑排序队列，将所有入度为0的节点加入队列
    for (int u = 1; u <= N; u++) {
        if (indegree[u] == 0) {
            topo_queque.EnQueue(u);
        }
    }

    int topo_len = 0; // 拓扑排序的长度
    // 执行拓扑排序
    while (!topo_queque.isEmpty()) {
        int u; // 取出队列头部的节点
        topo_queque.DeQueue(u);
        topo_order[topo_len++] = u; // 将该节点加入拓扑排序顺序
        // 遍历所有边，更新入度
        for (int j = 0; j < M; j++) {
            if (edges[j].u == u) {
                indegree[edges[j].v]--; // 终点的入度减1
                if (indegree[edges[j].v] == 0) {
                    // 如果入度为0，加入队列
                    topo_queque.EnQueue(edges[j].v);
                }
            }
        }
    }

    // 检查是否有环
    if (topo_len != N) {
        cout << "任务调度不可行！" << endl;
        cout << 0 << endl; // 有环，输出0
    }
    else {
        // 初始化最早完成时间为0
        for (int u = 1; u <= N; u++) {
            earliest[u] = 0;
        }
        // 按照拓扑顺序更新最早完成时间
        for (int i = 0; i < topo_len; i++) {
            int u = topo_order[i];
            // 遍历所有从u出发的边，更新终点的最早完成时间
            for (int j = 0; j < M; j++) {
                if (edges[j].u == u) {
                    if (earliest[edges[j].v] < earliest[u] + edges[j].w) {
                        earliest[edges[j].v] = earliest[u] + edges[j].w;
                    }
                }
            }
        }

        int total_time = 0; // 总时间
        // 找到所有终点节点的最早完成时间的最大值
        for (int u = 1; u <= N; u++) {
            if (outdegree[u] == 0) { // 如果是终点节点
                if (earliest[u] > total_time) {
                    total_time = earliest[u];
                }
            }
        }

        // 初始化最晚完成时间
        for (int u = 1; u <= N; u++) {
            if (outdegree[u] == 0) { // 如果是终点节点
                latest[u] = earliest[u];
            }
            else {
                latest[u] = 999999; // 其他节点初始化为一个大数
            }
        }

        // 从后往前更新最晚完成时间
        for (int i = topo_len - 1; i >= 0; i--) {
            int u = topo_order[i];
            if (outdegree[u] == 0) {
                latest[u] = earliest[u]; // 终点节点的最晚完成时间等于最早完成时间
            }
            else {
                int min_latest = 9999999; // 初始化为一个大数
                // 遍历所有从u出发的边，找到最小的latest[v] - w
                for (int j = 0; j < M; j++) {
                    if (edges[j].u == u) {
                        if (latest[edges[j].v] - edges[j].w < min_latest) {
                            min_latest = latest[edges[j].v] - edges[j].w;
                        }
                    }
                }
                latest[u] = min_latest; // 更新u的最晚完成时间
            }
        }

        struct Edge *critical_activities=new Edge[M]; // 存储关键活动
        int crit_count = 0; // 关键活动的数量
        // 找出所有关键活动，即满足latest[v] - earliest[u] - w == 0的边
        for (int i = 0; i < M; i++) {
            if (latest[edges[i].v] - earliest[edges[i].u] - edges[i].w == 0) {
                critical_activities[crit_count++] = edges[i];
            }
        }

        // 对关键活动进行排序
        sort_critical_activities(critical_activities, crit_count);

        // 输出总时间
        cout << "完成整个项目需要的最短时间: " << total_time << endl;
        cout << "所有的关键活动: " << endl;
        // 输出所有关键活动的u和v
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