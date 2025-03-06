#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX_ROWS = 100;
const int MAX_COLS = 100;
struct Maze                   //��rows, cols,maze����,dir�����װ����
{
    // �Թ�������������
    int rows, cols;

    // �Թ���ͼ
    int maze[MAX_ROWS][MAX_COLS];

    // �������飬�ֱ��ʾ�ϡ��ҡ��¡���
    int directions[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

    Maze() :rows(0), cols(0)
    {
        // ��ʼ���Թ�Ϊȫ�ϰ�
        for (int i = 0; i < NewMaze.rows; ++i) {
            for (int j = 0; j < NewMaze.cols; ++j) {
                NewMaze.maze[i][j] = 1;
            }
        }
    }
}NewMaze;

struct LinkNode                         //����ڵ�
{
    int x, y;                           //xy����
    LinkNode* link;                     //����ָ��

    LinkNode(int input_x = 0, int input_y = 0, LinkNode* ptr = NULL)      //������Ϣ
    {
        link = ptr;
        x = input_x;
        y = input_y;
    }
};

class LinkedStack                        //��ʽջ
{
private:
    LinkNode* top;                       //ջ��ָ�룬������ͷָ��

public:
    LinkedStack() :top(NULL) {}            //���캯��

    ~LinkedStack() { ; }                 //��������


    bool isEmpty()const                  //�ж�ջ�Ƿ�Ϊ��
    {
        return top == NULL;
    }

    void Push(int input_x, int input_y)  //����Ԫ��ѹ��ջ
    {
        LinkNode* New = new LinkNode(input_x, input_y);

        if (New == NULL) {
            cout << "�洢����ʧ�ܣ�" << endl;
            return;
        }

        New->link = top;                 //����ջ��
        top = New;
    }

    bool Pop()  //����ջ��Ԫ��
    {
        if (isEmpty())
            return false;

        LinkNode* temp = top;
        top = top->link;
        /*output_x = temp->x;           //��Ҫʱ����ȥ��ע��
        output_y = temp->y;*/
        delete temp;

        return true;
    }

    bool getTop(int& output_x, int& output_y)   //��ȡջ��Ԫ��
    {
        if (isEmpty())
            return false;

        output_x = top->x;
        output_y = top->y;

        return true;
    }

    int getSize()const                   //��ȡջ��Ԫ�ظ���
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

    void output()                       //���߼����ջ��Ԫ��
    {
        LinkNode* current = top;

        while (current != NULL) {
            cout << "<" << current->x << "," << current->y << ">";

            if (current->link != NULL)
                cout << "--->";

            current = current->link;
        }
    }
}Path;                         //��ʼ��ջ
//���ڼ�¼·��

void Wait_for_enter()
{
    cout << "���س�������...";
    cin.clear();
    cin.ignore(65472, '\n');
    cout << endl;
    while (cin.get() != '\n')
        ;
}

// ���ĳ��λ���Ƿ����Թ�������ͨ·������Ѱ��·��
bool PathisValid(int x, int y) 
{
    return x > 0 && x < NewMaze.rows && y > 0 && y < NewMaze.cols && NewMaze.maze[x][y] == 0;
}

// ���ĳ��λ���Ƿ����Թ��������ϰ�����ڽ����Թ�
bool MazeisValid(int x, int y) 
{
    return x > 0 && x < NewMaze.rows && y > 0 && y < NewMaze.cols && NewMaze.maze[x][y] == 1;
}

// ��ӡ�Թ�
void printMaze()
{
    cout << "        ";
    for (int i = 0; i < NewMaze.cols; i++)
        cout << i << "��    ";
    cout << endl;

    for (int i = 0; i < NewMaze.rows; ++i) {
        for (int j = 0; j < NewMaze.cols; ++j) {
            if (j == 0)
                cout << i << "��      ";

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

// ���ݷ�����Թ�����
bool solveMaze(int startX, int startY, int endX, int endY) 
{
    
    // �����ǰλ���ǳ��ڣ����ҵ�·��
    if (startX == endX && startY == endY) {
        // ����ǰλ����ջ
        Path.Push(startX, startY);
        NewMaze.maze[endX][endY] = 2;
        return true;
    }

    // ��ǵ�ǰλ��Ϊ�ѷ���
    NewMaze.maze[startX][startY] = 2;

    // �����ĸ�����
    for (int i = 0; i < 4; ++i) {
        int newX = startX + NewMaze.directions[i][0];
        int newY = startY + NewMaze.directions[i][1];

        if (PathisValid(newX, newY)) {
            if (solveMaze(newX, newY, endX, endY)) {
                // ����ǰλ����ջ
                Path.Push(startX, startY);
                return true;
            }
        }
    }

    // ������з����޷���ͨ�������
    Path.Pop();
    NewMaze.maze[startX][startY] = 0; // �ָ���ǰλ��Ϊͨ·
    return false;
}

// ����������������Թ�
void dfsGenerateMaze(int x, int y) 
{
    // ����ǰλ����Ϊͨ·
    NewMaze.maze[x][y] = 0;

    // ���ѡ��һ������
    int dir[4] = { 0, 1, 2, 3 };
    for (int i = 0; i < 4; ++i) {
        int r = rand() % 4;
        swap(dir[i], dir[r]);
    }

    // �����ĸ�����
    for (int i = 0; i < 4; ++i) {
        int newX = x + NewMaze.directions[dir[i]][0] * 2;
        int newY = y + NewMaze.directions[dir[i]][1] * 2;

        if (MazeisValid(newX, newY)) {
            // ��ͨ�м��ǽ
            NewMaze.maze[x + NewMaze.directions[dir[i]][0]][y + NewMaze.directions[dir[i]][1]] = 0;
            dfsGenerateMaze(newX, newY);
        }
    }
}

// �����Թ�
void generateMaze(int startX, int startY, int endX, int endY) 
{
    // ��ʼ���Թ�Ϊȫ�ϰ�
    for (int i = 0; i < NewMaze.rows; ++i) {
        for (int j = 0; j < NewMaze.cols; ++j) {
            NewMaze.maze[i][j] = 1;
        }
    }

    // ȷ����ںͳ�����ͨ·
    NewMaze.maze[startX][startY] = 0;
    NewMaze.maze[endX][endY] = 0;

    // ʹ������������������Թ�
    dfsGenerateMaze(startX, startY);

    // ȷ����·ͨ�����
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
        // �����Թ�������������
        cout << "�������Թ���������������";
        cin >> NewMaze.rows >> NewMaze.cols;

        if (cin.fail() || NewMaze.rows > 100 || NewMaze.cols > 100) {
            cin.clear();
            cin.ignore();
            cout << "�������в��Ϸ������������룡" << endl;
        }
        else
            break;
    }

    // ȷ������������������
    if (NewMaze.rows % 2 == 0) 
        NewMaze.rows++;
    if (NewMaze.cols % 2 == 0) 
        NewMaze.cols++;
}

int main() 
{
    input();
    // ȷ����ںͳ��ڵ�����
    int startX = 1, startY = 1;
    int endX = NewMaze.rows - 2, endY = NewMaze.cols - 2;

    // �����Թ�
    generateMaze(startX, startY, endX, endY);
    cout << "���ɵ��Թ���" << endl;
    cout << endl;
    printMaze();

    Wait_for_enter();   

    solveMaze(startX, startY, endX, endY);

    // ��ӡ���ɵ��Թ�
    cout << "�Թ���ͼ��" << endl;
    cout << endl;
    printMaze();

    // ����Թ�����
    cout << "�Թ�·����" << endl;
    cout << endl;
    // ��ӡ·��
    Path.output();

    return 0;
}