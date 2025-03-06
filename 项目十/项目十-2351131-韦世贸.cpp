#include<iostream>
#include<ctime>
using namespace std;

class Sort 
{
public:
    Sort() : randomNumber(0) {}  // 构造函数，初始化随机数个数
    ~Sort() {}                   // 析构函数

    void initialInterface();     // 初始化界面，获取随机数个数

    // 各种排序算法函数声明
    long long bubbleSort(int* data, double& sortTime);
    long long selectionSort(int* data, double& sortTime);
    long long directInsertionSort(int* data, double& sortTime);
    long long shellSort(int* data, double& sortTime);
    long long quickSort(int* data, double& sortTime, int left, int right);
    long long quickSortHelper(int* array, int left, int right);
    long long heapSort(int* data, double& sortTime);
    long long mergeSort(int* data, double& sortTime);
    long long radixSort(int* data, double& sortTime);

    // 归并排序的辅助函数
    // 递归和合并
    void merge(int* array, int* temp, int left, int mid, int right, long long& exchangeCount);
    void mergeSortRecursive(int* array, int* temp, int left, int right, long long& exchangeCount);

    // 辅助函数声明
    long long siftDown(int parent, int* data, int number);
    int getMaxBit(int* data, int n);

    int randomNumber;  // 随机数个数
};

// 初始化界面，获取随机数个数
void Sort::initialInterface() 
{
    cout << "**                 排序算法比较                    **" << endl;
    cout << "=====================================================" << endl;
    cout << "**                 1.冒泡排序                      **" << endl;
    cout << "**                 2.选择排序                      **" << endl;
    cout << "**                 3.直接插入排序                  **" << endl;
    cout << "**                 4.希尔排序                      **" << endl;
    cout << "**                 5.快速排序                      **" << endl;
    cout << "**                 6.堆排序                        **" << endl;
    cout << "**                 7.归并排序                      **" << endl;
    cout << "**                 8.基数排序                      **" << endl;
    cout << "**                 9.退出程序                      **" << endl;
    cout << "=====================================================" << endl;
    cout << endl;
    cout << "请输入要产生的随机数的个数:" << endl;
    cin >> randomNumber;
}

// 冒泡排序，返回交换次数和排序时间
long long Sort::bubbleSort(int* data, double& sortTime) 
{
    int i, j, temp;
    long long exchangeCount = 0;
    int* array = new int[randomNumber];
    for (i = 0; i < randomNumber; i++) {
        array[i] = data[i];
    }
    clock_t startTime = clock();
    for (i = 1; i < randomNumber; i++) {
        for (j = randomNumber - 1; j >= i; j--) {
            if (array[j] < array[j - 1]) {
                temp = array[j];
                array[j] = array[j - 1];
                array[j - 1] = temp;
                exchangeCount++;
            }
        }
    }
    clock_t endTime = clock();
    sortTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    delete[] array;
    return exchangeCount;
}

// 选择排序，返回交换次数和排序时间
long long Sort::selectionSort(int* data, double& sortTime) 
{
    int i, j, minIndex, temp;
    long long exchangeCount = 0;
    int* array = new int[randomNumber];
    for (i = 0; i < randomNumber; i++) {
        array[i] = data[i];
    }
    clock_t startTime = clock();
    for (i = 0; i < randomNumber - 1; i++) {
        minIndex = i;
        for (j = i + 1; j < randomNumber; j++) {
            if (array[j] < array[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            temp = array[i];
            array[i] = array[minIndex];
            array[minIndex] = temp;
            exchangeCount++;
        }
    }
    clock_t endTime = clock();
    sortTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    delete[] array;
    return exchangeCount;
}

// 直接插入排序，返回交换次数和排序时间
long long Sort::directInsertionSort(int* data, double& sortTime) {
    int* array = new int[randomNumber];
    memcpy(array, data, sizeof(int) * randomNumber);
    long long exchangeCount = 0;

    clock_t startTime = clock();
    for (int i = 1; i < randomNumber; i++) {
        int key = array[i];
        int j = i - 1;
        // 移动元素不再计数，仅统计实际交换
        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = key;
    }
    clock_t endTime = clock();

    sortTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    delete[] array;
    return exchangeCount; // 插入排序无交换操作，返回0
}

// 希尔排序，返回交换次数和排序时间
long long Sort::shellSort(int* data, double& sortTime) 
{
    int gap, i, j, temp;
    long long exchangeCount = 0;
    int* array = new int[randomNumber];
    for (i = 0; i < randomNumber; i++) {
        array[i] = data[i];
    }
    clock_t startTime = clock();
    for (gap = randomNumber / 2; gap > 0; gap /= 2) {
        for (i = gap; i < randomNumber; i++) {
            temp = array[i];
            j = i;
            while (j >= gap && array[j - gap] > temp) {
                array[j] = array[j - gap];
                j -= gap;
                exchangeCount++;
            }
            array[j] = temp;
        }
    }
    clock_t endTime = clock();
    sortTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    delete[] array;
    return exchangeCount;
}

// 快速排序，返回交换次数和排序时间
long long Sort::quickSort(int* data, double& sortTime, int left, int right) {
    int* array = new int[randomNumber];
    memcpy(array, data, sizeof(int) * randomNumber);

    clock_t startTime = clock();
    long long exchangeCount = quickSortHelper(array, 0, randomNumber - 1);
    clock_t endTime = clock();

    sortTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    delete[] array;
    return exchangeCount;
}

long long Sort::quickSortHelper(int* array, int left, int right) {
    if (left >= right) return 0;

    int pivot = array[(left + right) / 2]; // 使用中间元素作为pivot
    int i = left, j = right;
    long long exchanges = 0;

    while (i <= j) {
        while (array[i] < pivot) i++;
        while (array[j] > pivot) j--;
        if (i <= j) {
            std::swap(array[i], array[j]);
            exchanges++;
            i++;
            j--;
        }
    }

    exchanges += quickSortHelper(array, left, j);
    exchanges += quickSortHelper(array, i, right);
    return exchanges;
}

// 堆排序，返回交换次数和排序时间
long long Sort::heapSort(int* data, double& sortTime) {
    int* array = new int[randomNumber + 1];
    for (int i = 0; i < randomNumber; i++)
        array[i + 1] = data[i];

    clock_t startTime = clock();
    long long exchangeCount = 0;

    // 建大堆
    for (int i = randomNumber / 2; i >= 1; i--)
        exchangeCount += siftDown(i, array, randomNumber);

    // 排序
    for (int i = randomNumber; i > 1; i--) {
        std::swap(array[1], array[i]);
        exchangeCount++;
        exchangeCount += siftDown(1, array, i - 1);
    }

    clock_t endTime = clock();
    sortTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    delete[] array;
    return exchangeCount;
}

long long Sort::siftDown(int parent, int* data, int size) {
    long long exchanges = 0;
    int temp = data[parent];
    while (parent * 2 <= size) {
        int child = parent * 2;
        if (child + 1 <= size && data[child + 1] > data[child])
            child++;
        if (data[child] > temp) {
            data[parent] = data[child];
            parent = child;
            exchanges++;
        }
        else break;
    }
    data[parent] = temp;
    return exchanges;
}

// 归并排序，返回交换次数和排序时间
long long Sort::mergeSort(int* data, double& sortTime) 
{
    int i;
    long long exchangeCount = 0;
    int* array = new int[randomNumber];
    for (i = 0; i < randomNumber; i++) {
        array[i] = data[i];
    }
    clock_t startTime = clock();
    int* temp = new int[randomNumber];
    mergeSortRecursive(array, temp, 0, randomNumber - 1, exchangeCount);
    clock_t endTime = clock();
    sortTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    delete[] temp;
    delete[] array;
    return exchangeCount;
}

// 归并排序递归函数
void Sort::mergeSortRecursive(int* array, int* temp, int left, int right, long long& exchangeCount) 
{
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSortRecursive(array, temp, left, mid, exchangeCount);
        mergeSortRecursive(array, temp, mid + 1, right, exchangeCount);
        merge(array, temp, left, mid, right, exchangeCount);
    }
}

// 归并排序合并函数
void Sort::merge(int* array, int* temp, int left, int mid, int right, long long& exchangeCount) 
{
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        if (array[i] <= array[j]) {
            temp[k++] = array[i++];
        }
        else {
            temp[k++] = array[j++];
            exchangeCount += mid - i + 1; // 统计逆序数
        }
    }
    while (i <= mid) {
        temp[k++] = array[i++];
    }
    while (j <= right) {
        temp[k++] = array[j++];
    }
    for (i = left; i <= right; i++) {
        array[i] = temp[i];
    }

    exchangeCount += (right - left + 1) * 2; // 复制到temp和复制回来
}

// 基数排序，返回交换次数和排序时间
long long Sort::radixSort(int* data, double& sortTime) {
    int* array = new int[randomNumber];
    memcpy(array, data, sizeof(int) * randomNumber);
    long long exchangeCount = 0;

    clock_t startTime = clock();
    int maxDigit = getMaxBit(array, randomNumber);
    int* temp = new int[randomNumber];
    int count[10];

    for (int d = 1, digit = 1; d <= maxDigit; d++, digit *= 10) {
        memset(count, 0, sizeof(count));

        for (int i = 0; i < randomNumber; i++)
            count[(array[i] / digit) % 10]++;

        for (int i = 1; i < 10; i++)
            count[i] += count[i - 1];

        for (int i = randomNumber - 1; i >= 0; i--) {
            int k = (array[i] / digit) % 10;
            temp[--count[k]] = array[i];
            exchangeCount++; // 分配计数
        }

        memcpy(array, temp, sizeof(int) * randomNumber);
        exchangeCount += randomNumber; // 收集计数
    }

    clock_t endTime = clock();
    sortTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    delete[] temp;
    delete[] array;
    return exchangeCount;
}

// 堆调整函数，返回交换次数
long long Sort::siftDown(int parent, int* data, int number) 
{
    int child;
    long long exchangeCount = 0;
    int temp = data[parent];
    while (parent * 2 <= number) {
        child = parent * 2;
        if (child + 1 <= number && data[child + 1] < data[child]) {
            child++;
        }
        if (data[child] < temp) {
            data[parent] = data[child];
            exchangeCount++;
            parent = child;
        }
        else {
            break;
        }
        data[child] = temp;
    }
    return exchangeCount;
}

// 获取最大位数
int Sort::getMaxBit(int* data, int n) 
{
    int maxData = data[0];
    for (int i = 1; i < n; i++) {
        if (data[i] > maxData) {
            maxData = data[i];
        }
    }
    int maxBit = 0;
    while (maxData > 0) {
        maxData /= 10;
        maxBit++;
    }
    return maxBit;
}

int main() 
{
    int choice = 0;
    long long exchangeCount;
    double sortTime;

    int* data1; // 用于存储随机数的数组
    int* data2;

    Sort sort; // 创建Sort类实例

    srand((unsigned int)time(NULL)); // 种随机数种子
    sort.initialInterface(); // 调用初始化界面函数


    int* originalData = new int[sort.randomNumber];
    for (int i = 0; i < sort.randomNumber; i++) {
        originalData[i] = rand();
    }

    while (choice != 9) {
        int* data = new int[sort.randomNumber];
        memcpy(data, originalData, sizeof(int) * sort.randomNumber);

        switch (choice) {
            // 所有排序函数调用都使用data数组
            case 5:
                exchangeCount = sort.quickSort(data, sortTime, 0, sort.randomNumber - 1);
                break;
                // ...其他case
        }
        delete[] data;
    }
    delete[] originalData;

    return 0;
}