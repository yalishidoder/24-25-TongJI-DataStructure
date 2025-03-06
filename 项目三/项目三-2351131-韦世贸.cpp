#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX_ROWS = 100;
const int MAX_COLS = 100;
struct Maze                   //把rows, cols,maze数组,dir数组封装起来
{
    // 迷宫的行数和列数
    int rows, cols;

    // 迷宫地图
    int maze[MAX_ROWS][MAX_COLS];

    // 方向数组，分别表示上、右、下、左
    int directions[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

    Maze() :rows(0), cols(0)
    {
        // 初始化迷宫为全障碍
        for (int i = 0; i < NewMaze.rows; ++i) {
            for (int j = 0; j < NewMaze.cols; ++j) {
                NewMaze.maze[i][j] = 1;
            }
        }
    }
}NewMaze;

struct LinkNode                         //链表节点
{
    int x, y;                           //xy坐标
    LinkNode* link;                     //链表指针

    LinkNode(int input_x = 0, int input_y = 0, LinkNode* ptr = NULL)      //输入信息
    {
        link = ptr;
        x = input_x;
        y = input_y;
    }
};

class LinkedStack                        //链式栈
{
private:
    LinkNode* top;                       //栈顶指针，即链表头指针

public:
    LinkedStack() :top(NULL) {}            //构造函数

    ~LinkedStack() { ; }                 //析构函数


    bool isEmpty()const                  //判断栈是否为空
    {
        return top == NULL;
    }

    void Push(int input_x, int input_y)  //将新元素压入栈
    {
        LinkNode* New = new LinkNode(input_x, input_y);

        if (New == NULL) {
            cout << "存储分配失败！" << endl;
            return;
        }

        New->link = top;                 //更新栈顶
        top = New;
    }

    bool Pop()  //弹出栈顶元素
    {
        if (isEmpty())
            return false;

        LinkNode* temp = top;
        top = top->link;
        /*output_x = temp->x;           //需要时可以去掉注释
        output_y = temp->y;*/
        delete temp;

        return true;
    }

    bool getTop(int& output_x, int& output_y)   //获取栈顶元素
    {
        if (isEmpty())
            return false;

        output_x = top->x;
        output_y = top->y;

        return true;
    }

    int getSize()const                   //获取栈的元素个数
    {
        LinkNode* p = top;
        int size = 0;

        while (p != NULL)
        {
            p = p->link;
            size++;
        }

        return size;
    }

    void output()                       //按逻辑输出栈内元素
    {
        LinkNode* current = top;

        while (current != NULL) {
            cout << "<" << current->x << "," << current->y << ">";

            if (current->link != NULL)
                cout << "--->";

            current = current->link;
        }
    }
}Path;                         //初始化栈
//用于记录路径

void Wait_for_enter()
{
    cout << "按回车键继续...";
    cin.clear();
    cin.ignore(65472, '\n');
    cout << endl;
    while (cin.get() != '\n')
        ;
}

// 检查某个位置是否在迷宫内且是通路，用于寻找路径
bool PathisValid(int x, int y) 
{
    return x > 0 && x < NewMaze.rows && y > 0 && y < NewMaze.cols && NewMaze.maze[x][y] == 0;
}

// 检查某个位置是否在迷宫内且是障碍物，用于建立迷宫
bool MazeisValid(int x, int y) 
{
    return x > 0 && x < NewMaze.rows && y > 0 && y < NewMaze.cols && NewMaze.maze[x][y] == 1;
}

// 打印迷宫
void printMaze()
{
    cout << "        ";
    for (int i = 0; i < NewMaze.cols; i++)
        cout << i << "列    ";
    cout << endl;

    for (int i = 0; i < NewMaze.rows; ++i) {
        for (int j = 0; j < NewMaze.cols; ++j) {
            if (j == 0)
                cout << i << "行      ";

            if (NewMaze.maze[i][j] == 1)
                cout << "#";
            else if (NewMaze.maze[i][j] == 2)
                cout << "x";
            else
                cout << NewMaze.maze[i][j];

            cout << "      ";
        }
        cout << endl;
        cout << endl;
    }
}

// 回溯法求解迷宫问题
bool solveMaze(int startX, int startY, int endX, int endY) 
{
    
    // 如果当前位置是出口，则找到路径
    if (startX == endX && startY == endY) {
        // 将当前位置入栈
        Path.Push(startX, startY);
        NewMaze.maze[endX][endY] = 2;
        return true;
    }

    // 标记当前位置为已访问
    NewMaze.maze[startX][startY] = 2;

    // 尝试四个方向
    for (int i = 0; i < 4; ++i) {
        int newX = startX + NewMaze.directions[i][0];
        int newY = startY + NewMaze.directions[i][1];

        if (PathisValid(newX, newY)) {
            if (solveMaze(newX, newY, endX, endY)) {
                // 将当前位置入栈
                Path.Push(startX, startY);
                return true;
            }
        }
    }

    // 如果所有方向都无法走通，则回溯
    Path.Pop();
    NewMaze.maze[startX][startY] = 0; // 恢复当前位置为通路
    return false;
}

// 深度优先搜索生成迷宫
void dfsGenerateMaze(int x, int y) 
{
    // 将当前位置设为通路
    NewMaze.maze[x][y] = 0;

    // 随机选择一个方向
    int dir[4] = { 0, 1, 2, 3 };
    for (int i = 0; i < 4; ++i) {
        int r = rand() % 4;
        swap(dir[i], dir[r]);
    }

    // 尝试四个方向
    for (int i = 0; i < 4; ++i) {
        int newX = x + NewMaze.directions[dir[i]][0] * 2;
        int newY = y + NewMaze.directions[dir[i]][1] * 2;

        if (MazeisValid(newX, newY)) {
            // 打通中间的墙
            NewMaze.maze[x + NewMaze.directions[dir[i]][0]][y + NewMaze.directions[dir[i]][1]] = 0;
            dfsGenerateMaze(newX, newY);
        }
    }
}

// 生成迷宫
void generateMaze(int startX, int startY, int endX, int endY) 
{
    // 初始化迷宫为全障碍
    for (int i = 0; i < NewMaze.rows; ++i) {
        for (int j = 0; j < NewMaze.cols; ++j) {
            NewMaze.maze[i][j] = 1;
        }
    }

    // 确保入口和出口是通路
    NewMaze.maze[startX][startY] = 0;
    NewMaze.maze[endX][endY] = 0;

    // 使用深度优先搜索生成迷宫
    dfsGenerateMaze(startX, startY);

    // 确保有路通向出口
    int l = rand() % 2;
    int r = rand() % 2;

    if (l == 0)
        r = 1;
    else if (r == 0)
        l = 1;
    else if (r == 1 && l == 1)
        l = 0;

    NewMaze.maze[endX - l][endY - r] = 0;
}

void input()
{
    while (1)
    {
        // 输入迷宫的行数和列数
        cout << "请输入迷宫的行数和列数：";
        cin >> NewMaze.rows >> NewMaze.cols;

        if (cin.fail() || NewMaze.rows > 100 || NewMaze.cols > 100) {
            cin.clear();
            cin.ignore();
            cout << "输入行列不合法，请重新输入！" << endl;
        }
        else
            break;
    }

    // 确保行数和列数是奇数
    if (NewMaze.rows % 2 == 0) 
        NewMaze.rows++;
    if (NewMaze.cols % 2 == 0) 
        NewMaze.cols++;
}

int main() 
{
    input();
    // 确定入口和出口的坐标
    int startX = 1, startY = 1;
    int endX = NewMaze.rows - 2, endY = NewMaze.cols - 2;

    // 生成迷宫
    generateMaze(startX, startY, endX, endY);
    cout << "生成的迷宫：" << endl;
    cout << endl;
    printMaze();

    Wait_for_enter();   

    solveMaze(startX, startY, endX, endY);

    // 打印生成的迷宫
    cout << "迷宫地图：" << endl;
    cout << endl;
    printMaze();

    // 求解迷宫问题
    cout << "迷宫路径：" << endl;
    cout << endl;
    // 打印路径
    Path.output();

    return 0;
}