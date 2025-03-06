#include <iostream>
#include <string>

using namespace std;

// 电网的顶点类
class Node
{
    friend class Linked_list;
    friend class ElectricFrence;
public:
    Node();
private:
    bool isVisited;                   // 是否访问过
    string startName;                 // 出发顶点名称
    string currentName;               // 当前顶点名称
    double weight;                    // 边的权值
    Node* nextNode;                   // 指向下一个节点的指针
};

// Node类的构造函数
Node::Node()
{
    isVisited = false;
    currentName = '?';
    weight = 0.0;
    nextNode = nullptr;
}

// 链表类，表示顶点的邻接表
class Linked_list
{
    friend class ElectricFrence;
public:
    Linked_list();                    // 构造函数
    void printMinimumSpanningTree(Node* node);  // 输出最小生成树
    static void insertEdge(Linked_list* list, string nodeName, double pathWeight);  // 插入边
    static bool hasEdge(Linked_list* list, string nodeName);   // 检查是否有边
    static int countAdjacentNodes(Linked_list& list);          // 统计相邻顶点数
    string nodeName;                // 顶点名称
    Node* adjacentNode;             // 相邻顶点指针
    Linked_list* nextLink;          // 下一个顶点链表指针
};

// Linked_list类的构造函数
Linked_list::Linked_list()
{
    nodeName = "?";
    adjacentNode = nullptr;
    nextLink = nullptr;
}

// 输出最小生成树
void Linked_list::printMinimumSpanningTree(Node* node)
{
    if (node->nextNode)
    {
        printMinimumSpanningTree(node->nextNode);

        cout << node->startName << "-(" << node->weight << ")->" << node->currentName << " ";
    }
    else
    {
        cout << node->currentName << " ";
    }
}

// 插入边到邻接表中
void Linked_list::insertEdge(Linked_list* list, string nodeName, double pathWeight)
{
    if (hasEdge(list, nodeName))
    {
        cout << "两顶点之间已经有边，无法插入新边。" << endl;
    }
    else
    {
        Node* newNode = new Node;
        newNode->currentName = nodeName;
        newNode->weight = pathWeight;
        newNode->nextNode = list->adjacentNode;
        list->adjacentNode = newNode;
    }
}

// 检查是否有边连接到指定顶点
bool Linked_list::hasEdge(Linked_list* list, string nodeName)
{
    Node* currentNode = list->adjacentNode;
    while (currentNode)
    {
        if (currentNode->currentName == nodeName)
            return true;
        currentNode = currentNode->nextNode;
    }
    return false;
}

// 统计相邻顶点的数量
int Linked_list::countAdjacentNodes(Linked_list& list)
{
    int count = 0;
    Node* currentNode = list.adjacentNode;
    while (currentNode)
    {
        count++;
        currentNode = currentNode->nextNode;
    }
    return count;
}

// 电网类，表示整个图
class ElectricFrence
{
    friend class Linked_list;
public:
    void createVertices(ElectricFrence& grid);          // 创建顶点
    void insertEdges(ElectricFrence& grid);             // 插入边
    bool isVertexExist(ElectricFrence& grid, string name); // 判断顶点是否存在
    void changeVisitedFlag(ElectricFrence& grid, string name); // 更改访问标志
    int countVertices(ElectricFrence& grid);            // 统计顶点数
    Node* findMinimumPath(ElectricFrence grid, Linked_list& edgeList, string startName); // 找最小路径
    void buildMinimumSpanningTree(ElectricFrence grid, Linked_list& edgeList, string startName); // 构建最小生成树
private:
    Linked_list* vertexHead;
};

// 创建电网顶点
void ElectricFrence::createVertices(ElectricFrence& grid)
{
    cout << "请输入顶点的个数：" << endl;
    int n;
    cin >> n;
    cout << "请依次输入各顶点的名称：" << endl;
    Linked_list* current = nullptr;
    for (int i = 0; i < n; i++)
    {
        Linked_list* newList = new Linked_list;
        cin >> newList->nodeName;
        if (i == 0)
        {
            grid.vertexHead = newList;
            current = grid.vertexHead;
        }
        else
        {
            current->nextLink = newList;
            current = newList;
        }
    }
}

// 插入边到电网中
void ElectricFrence::insertEdges(ElectricFrence& grid)
{
    cout << "请输入两个顶点以及边的权值（输入? ? 0结束）：" << endl;
    string name1, name2;
    double pathWeight;
    cin >> name1 >> name2 >> pathWeight;
    if (name1 != "?" || name2 != "?" || pathWeight != 0)
    {
        if (isVertexExist(grid, name1) && isVertexExist(grid, name2))
        {
            Linked_list* current = grid.vertexHead;
            while (current)
            {
                if (current->nodeName == name1)
                {
                    Linked_list::insertEdge(current, name2, pathWeight);
                }
                else if (current->nodeName == name2)
                {
                    Linked_list::insertEdge(current, name1, pathWeight);
                }
                current = current->nextLink;
            }
            insertEdges(grid);
        }
        else
        {
            cout << "错误的顶点，请重新输入：" << endl;
            insertEdges(grid);
        }
    }
}

// 检查顶点是否存在
bool ElectricFrence::isVertexExist(ElectricFrence& grid, string name)
{
    Linked_list* current = grid.vertexHead;
    while (current)
    {
        if (current->nodeName == name)
            return true;
        current = current->nextLink;
    }
    return false;
}

// 更改顶点的访问标志
void ElectricFrence::changeVisitedFlag(ElectricFrence& grid, string name)
{
    Linked_list* current = grid.vertexHead;
    while (current)
    {
        Node* node = current->adjacentNode;
        while (node)
        {
            if (node->currentName == name)
            {
                node->isVisited = true;
                node->weight = -1;
                break;
            }
            node = node->nextNode;
        }
        current = current->nextLink;
    }
}

// 统计电网中的顶点数量
int ElectricFrence::countVertices(ElectricFrence& grid)
{
    int count = 0;
    Linked_list* current = grid.vertexHead;
    while (current)
    {
        count++;
        current = current->nextLink;
    }
    return count;
}

// 找到最小权值的边
Node* ElectricFrence::findMinimumPath(ElectricFrence grid, Linked_list& edgeList, string startName)
{
    Linked_list* current = grid.vertexHead;
    while (current && current->nodeName != startName)
    {
        current = current->nextLink;
    }
    if (!current)
        return nullptr;
    Node* minNode = nullptr;
    Node* temp = current->adjacentNode;
    while (temp)
    {
        if (!temp->isVisited && (minNode == nullptr || temp->weight < minNode->weight))
            minNode = temp;
        temp = temp->nextNode;
    }
    return minNode;
}

// 构建最小生成树
void ElectricFrence::buildMinimumSpanningTree(ElectricFrence grid, Linked_list& edgeList, string startName)
{
    if (!isVertexExist(grid, startName))
    {
        cout << "错误的起始顶点，请重新输入：" << endl;
        return;
    }
    Linked_list* current = grid.vertexHead;
    while (current && current->nodeName != startName)
    {
        current = current->nextLink;
    }
    if (!current)
        return;
    Node* startNode = new Node;
    startNode->currentName = startName;
    startNode->startName = startName;
    startNode->weight = 0.0;
    startNode->nextNode = edgeList.adjacentNode;
    edgeList.adjacentNode = startNode;
    changeVisitedFlag(grid, startName);
    while (edgeList.countAdjacentNodes(edgeList) < grid.countVertices(grid))
    {
        Node* minEdgeNode = nullptr;
        Node* tempEdge = edgeList.adjacentNode;
        while (tempEdge)
        {
            Node* minPathNode = findMinimumPath(grid, edgeList, tempEdge->currentName);
            if (minPathNode && (!minEdgeNode || minPathNode->weight < minEdgeNode->weight))
                minEdgeNode = minPathNode;
            tempEdge = tempEdge->nextNode;
        }
        if (minEdgeNode)
        {
            Node* newNode = new Node;
            newNode->startName = minEdgeNode->startName;
            newNode->currentName = minEdgeNode->currentName;
            newNode->weight = minEdgeNode->weight;
            newNode->nextNode = edgeList.adjacentNode;
            edgeList.adjacentNode = newNode;
            changeVisitedFlag(grid, newNode->currentName);
        }
        else
            break;
    }
}

// 显示系统菜单并处理用户选择
void showSystemMenu()
{
    cout << endl;
    cout << "**                      电网造价模拟系统                    **" << endl;
    cout << "==============================================================" << endl;
    cout << "**                   请选择要执行的操作:                    **" << endl;
    cout << "**                      A---创建电网顶点                    **" << endl;
    cout << "**                      B---添加电网的边                    **" << endl;
    cout << "**                      C---构造最小生成树                  **" << endl;
    cout << "**                      D---显示最小生成树                  **" << endl;
    cout << "**                      E---退出程序                        **" << endl;
    cout << "==============================================================" << endl;

    ElectricFrence grid;
    Linked_list edgeList;

    char choice = 0;
    while (choice != 'E')
    {
        cout << "\n请选择操作：";
        cin >> choice;
        switch (choice)
        {
            case 'A':
            {
                grid.createVertices(grid);
                break;
            }
            case 'B':
            {
                grid.insertEdges(grid);
                break;
            }
            case 'C':
            {
                cout << "请输入起始顶点：";
                string startName;
                cin >> startName;
                grid.buildMinimumSpanningTree(grid, edgeList, startName);
                cout << "生成Prim最小生成树！\n";
                break;
            }
            case 'D':
            {
                cout << "最小生成树的顶点及边为：" << endl;
                edgeList.printMinimumSpanningTree(edgeList.adjacentNode);
                cout << endl;
                break;
            }
            case 'E':
                cout << "退出系统" << endl;
                break;
            default:
            {
                cout << "输入错误，请输入正确的操作：" << endl;
                break;
            }
        }
    }
}

int main()
{
    showSystemMenu();
    return 0;
}