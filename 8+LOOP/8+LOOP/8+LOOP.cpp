#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <ctime>
#include <chrono>
#include <random>
#include <iomanip>
#include <limits>
#include <string>
using namespace std;
using namespace std::chrono;

//Структура элемента очереди из предыдущей работы
struct QueueNode {
    int data;           // Данные - номер вершины
    QueueNode* next;    // Указатель на следующий элемент
};

//Самодельная очередь на основе предыдущей работы
class CustomQueue {
private:
    QueueNode* front;   // Указатель на начало очереди
    QueueNode* rear;    // Указатель на конец очереди

public:
    //Конструктор
    CustomQueue() : front(nullptr), rear(nullptr) {}

    //Деструктор для очистки памяти
    ~CustomQueue() {
        while (!empty()) {
            pop();
        }
    }

    //Добавление элемента в конец очереди
    void push(int value) {
        QueueNode* newNode = new QueueNode;
        newNode->data = value;
        newNode->next = nullptr;

        if (rear == nullptr) {
            //Если очередь пуста - новый элемент становится и началом и концом
            front = rear = newNode;
        }
        else {
            //Добавляем элемент в конец очереди
            rear->next = newNode;
            rear = newNode; // Обновляем указатель на конец очереди
        }
    }

    //Удаление элемента из начала очереди
    void pop() {
        if (empty()) {
            return;
        }

        //Сохраняем указатель на удаляемый элемент
        QueueNode* temp = front;

        //Перемещаем начало очереди на следующий элемент
        front = front->next;

        //Если после удаления очередь стала пустой
        if (front == nullptr) {
            rear = nullptr; // Обнуляем указатель на конец
        }

        delete temp; // Освобождаем память удаленного элемента
    }

    //Получение первого элемента очереди без удаления
    int frontValue() {
        if (empty()) {
            return -1; // Возвращаем -1 если очередь пуста
        }
        return front->data;
    }

    //Проверка, пуста ли очередь
    bool empty() {
        return front == nullptr;
    }

    //Получение размера очереди
    size_t size() {
        size_t count = 0;
        QueueNode* current = front;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        return count;
    }
};

//Структура для узла списка смежности
struct AdjListNode {
    int dest;// Номер вершины
    AdjListNode* next;// Указатель на следующий узел
    //Конструктор
    AdjListNode(int d) : dest(d), next(nullptr) {}
};

//Структура для представления списка смежности(одна вершина)
struct AdjList {
    AdjListNode* head;// Указатель на начало списка смежных вершин
};

//Класс для графа (матрица смежности + списки смежности)
class Graph {
private:
    int V;// Количество вершин
    vector<vector<int>> adjMatrix;// Матрица смежности
    vector<AdjList> adjLists;// Вектор списков смежности

public:
    //Конструктор графа
    Graph(int vertices) : V(vertices) {
        //Инициализация матрицы смежности нулями
        adjMatrix.resize(V, vector<int>(V, 0));
        //Инициализация списков смежности
        adjLists.resize(V);
        for (int i = 0; i < V; i++) {
            adjLists[i].head = nullptr;// Изначально все списки пустые
        }
    }

    //Деструктор для очистки памяти
    ~Graph() {
        //Очистка списков смежности
        for (int i = 0; i < V; i++) {
            AdjListNode* current = adjLists[i].head;
            while (current != nullptr) {
                AdjListNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    //Генерация случайного графа
    void generateRandomGraph() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 1);// Равномерное распределение 0 или 1
        //Генерация матрицы смежности
        for (int i = 0; i < V; i++) {
            for (int j = i + 1; j < V; j++) {
                int edge = dis(gen);// Случайное решение: есть ребро или нет
                adjMatrix[i][j] = edge;
                adjMatrix[j][i] = edge;
            }
        }
        //Построение списков смежности на основе матрицы
        buildAdjListsFromMatrix();
    }

    //Генерация случайного графа с петлями
    void generateRandomGraphWithLoops() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 1);// Равномерное распределение 0 или 1
        //Генерация матрицы смежности с возможными петлями
        for (int i = 0; i < V; i++) {
            for (int j = i; j < V; j++) {// Начинаем с j = i для возможности петель
                if (i == j) {
                    //Для диагональных элементов (петли) - 50% вероятность
                    int edge = dis(gen);
                    adjMatrix[i][j] = edge;
                }
                else {
                    //Для обычных ребер
                    int edge = dis(gen);
                    adjMatrix[i][j] = edge;
                    adjMatrix[j][i] = edge;
                }
            }
        }
        //Построение списков смежности на основе матрицы
        buildAdjListsFromMatrix();
    }

    //Построение списков смежности из матрицы смежности
    void buildAdjListsFromMatrix() {
        //Очистка существующих списков (если были созданы ранее)
        for (int i = 0; i < V; i++) {
            AdjListNode* current = adjLists[i].head;
            //Удаляем все узлы текущего списка
            while (current != nullptr) {
                AdjListNode* temp = current;
                current = current->next;
                delete temp;// Освобождаем память
            }
            adjLists[i].head = nullptr;// Обнуляем указатель
        }

        //Построение новых списков на основе матрицы смежности
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (adjMatrix[i][j] == 1) {// Если есть ребро между i и j
                    // Добавляем ребро, даже если это петля (i == j)
                    addEdgeToList(i, j);// Добавляем в список смежности
                }
            }
        }
    }

    //Добавление ребра в список смежности для вершины src
    void addEdgeToList(int src, int dest) {
        //Создаем новый узел для вершины назначения
        AdjListNode* newNode = new AdjListNode(dest);
        //Вставляем новый узел в начало списка
        newNode->next = adjLists[src].head;
        adjLists[src].head = newNode;
    }

    //Вывод матрицы смежности
    void printAdjMatrix() {
        cout << "Матрица смежности:" << endl;
        cout << "  ";
        //Вывод заголовка
        for (int i = 0; i < V; i++) {
            cout << setw(3) << i;
        }
        cout << endl;

        //Вывод самой матрицы
        for (int i = 0; i < V; i++) {
            cout << setw(2) << i << "";// Номер строки
            for (int j = 0; j < V; j++) {
                cout << setw(3) << adjMatrix[i][j];// Элемент матрицы
            }
            cout << endl;
        }
        cout << endl;
    }

    //Вывод списков смежности
    void printAdjLists() {
        cout << "Списки смежности:" << endl;
        for (int i = 0; i < V; i++) {
            cout << i << ": ";// Номер вершины
            AdjListNode* current = adjLists[i].head;
            //Проходим по всему списку смежных вершин
            while (current != nullptr) {
                cout << current->dest;
                if (current->dest == i) {
                    cout << "(петля)";// Помечаем петли
                }
                if (current->next != nullptr) {
                    cout << " -> ";
                }
                current = current->next;
            }
            cout << endl;
        }
        cout << endl;
    }

    //Обход в ширину с использованием матрицы смежности и стандартной очереди (Задание 1)
    vector<int> BFS_Matrix_StdQueue(int start) {
        vector<int> result;// Вектор для хранения порядка обхода
        vector<bool> visited(V, false);// Вектор посещенных вершин
        queue<int> q;// Стандартная очередь
        //Включаем таймер
        auto start_time = high_resolution_clock::now();
        //Начало обхода со стартовой вершины
        visited[start] = true;
        q.push(start);
        //Пока очередь не пуста
        while (!q.empty()) {
            int current = q.front();// Берем первую вершину из очереди
            q.pop();// Удаляем ее из очереди
            result.push_back(current);// Добавляем в результат
            //Просматриваем всех соседей текущей вершины через матрицу смежности
            for (int i = 0; i < V; i++) {
                //Если есть ребро и вершина еще не посещена, и если это не петля или если петля, то только если i == current
                if (adjMatrix[current][i] == 1 && !visited[i] && (i != current || (i == current && !visited[i]))) {
                    visited[i] = true;// Помечаем как посещенную
                    q.push(i);// Добавляем в очередь для дальнейшего обхода
                }
            }
        }
        //Останавливаем таймер
        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end_time - start_time);
        cout << "Время обхода (матрица + queue): " << duration.count() << " микросекунд" << endl;
        return result;
    }

    //Обход в ширину с использованием списков смежности и стандартной очереди (Задание 1*)
    vector<int> BFS_List_StdQueue(int start) {
        vector<int> result;// Вектор для хранения порядка обхода
        vector<bool> visited(V, false);// Вектор посещенных вершин
        queue<int> q;// Стандартная очередь
        //Включаем таймер
        auto start_time = high_resolution_clock::now();
        //Начало обхода со стартовой вершины
        visited[start] = true;
        q.push(start);
        //Основной цикл
        while (!q.empty()) {
            int current = q.front();// Берем первую вершину из очереди
            q.pop();// Удаляем ее из очереди
            result.push_back(current);// Добавляем в результат
            //Просматриваем всех соседей через список смежности
            AdjListNode* neighbor = adjLists[current].head;
            while (neighbor != nullptr) {
                //Если соседняя вершина еще не посещена, И если это не петля или если петля, то только если neighbor->dest == current
                if (!visited[neighbor->dest] && (neighbor->dest != current || (neighbor->dest == current && !visited[neighbor->dest]))) {
                    visited[neighbor->dest] = true;// Помечаем как посещенную
                    q.push(neighbor->dest);// Добавляем в очередь
                }
                neighbor = neighbor->next;// Переход к следующему соседу
            }
        }
        //Останавливаем таймер
        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end_time - start_time);
        cout << "Время обхода (списки + queue): " << duration.count() << " микросекунд" << endl;
        return result;
    }

    //Обход в ширину с использованием матрицы смежности и самодельной очереди (Задание 2*)
    vector<int> BFS_Matrix_CustomQueue(int start) {
        vector<int> result;// Вектор для хранения порядка обхода
        vector<bool> visited(V, false);// Вектор посещенных вершин
        CustomQueue q;// Самодельная очередь
        //Включаем таймер
        auto start_time = high_resolution_clock::now();
        //Начало обхода со стартовой вершины
        visited[start] = true;
        q.push(start);
        //Основной цикл
        while (!q.empty()) {
            int current = q.frontValue();// Берем первую вершину из очереди
            q.pop();// Удаляем ее из очереди
            result.push_back(current);// Добавляем в результат
            //Просматриваем всех соседей текущей вершины через матрицу смежности
            for (int i = 0; i < V; i++) {
                // Если есть ребро и вершина еще не посещена, И если это не петля или если петля, то только если i == current
                if (adjMatrix[current][i] == 1 && !visited[i] && (i != current || (i == current && !visited[i]))) {
                    visited[i] = true;// Помечаем как посещенную
                    q.push(i);// Добавляем в самодельную очередь
                }
            }
        }
        //Останавливаем таймер
        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end_time - start_time);
        cout << "Время обхода (матрица + custom queue): " << duration.count() << " микросекунд" << endl;
        return result;
    }

    //Обход в ширину с использованием списков смежности и самодельной очереди
    vector<int> BFS_List_CustomQueue(int start) {
        vector<int> result;// Вектор для хранения порядка обхода
        vector<bool> visited(V, false);// Вектор посещенных вершин
        CustomQueue q;// Самодельная очередь
        //Включаем таймер
        auto start_time = high_resolution_clock::now();
        //Начало обхода со стартовой вершины
        visited[start] = true;
        q.push(start);
        //Основной цикл
        while (!q.empty()) {
            int current = q.frontValue();// Берем первую вершину из очереди
            q.pop();// Удаляем ее из очереди
            result.push_back(current);// Добавляем в результат
            //Просматриваем всех соседей через список смежности
            AdjListNode* neighbor = adjLists[current].head;
            while (neighbor != nullptr) {
                //Если соседняя вершина еще не посещена, И если это не петля или если петля, то только если neighbor->dest == current
                if (!visited[neighbor->dest] && (neighbor->dest != current || (neighbor->dest == current && !visited[neighbor->dest]))) {
                    visited[neighbor->dest] = true;// Помечаем как посещенную
                    q.push(neighbor->dest);// Добавляем в самодельную очередь
                }
                neighbor = neighbor->next;// Переход к следующему соседу
            }
        }
        //Останавливаем таймер
        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end_time - start_time);
        cout << "Время обхода (списки + custom queue): " << duration.count() << " микросекунд" << endl;
        return result;
    }

    //Вывод результатов
    static void printBFSResult(const vector<int>& result, const string& method) {
        cout << "Результат обхода в ширину (" << method << "): ";
        for (size_t i = 0; i < result.size(); i++) {
            cout << result[i];
            if (i != result.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl << endl;
    }

    //Геттер для получения количества вершин
    int getVertexCount() const {
        return V;
    }
};

//Функция для ввода
int safeInputInt(const string& prompt, int minVal = numeric_limits<int>::min(), int maxVal = numeric_limits<int>::max()) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        //Проверка на некорректный ввод (буквы, символы)
        if (cin.fail()) {
            cin.clear();// Сбрасываем флаг ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n');// Очищаем буфер ввода
            cout << "Ошибка: введите корректное число!" << endl;
        }
        //Проверка на выход за границы диапазона
        else if (value < minVal || value > maxVal) {
            cout << "Ошибка: число должно быть в диапазоне " << minVal << "-" << maxVal << "!" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            //Успешный ввод - очищаем буфер и возвращаем значение
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

//Функция для отображения главного меню
void displayMainMenu() {
    cout << "\nМеню" << endl;
    cout << "1. Показать текущий граф" << endl;
    cout << "2. Выполнить обход графа" << endl;
    cout << "3. Изменить стартовую вершину" << endl;
    cout << "4. Перегенерировать граф" << endl;
    cout << "5. Перегенерировать граф с петлями" << endl;
    cout << "6. Создать новый граф" << endl;
    cout << "0. Выход" << endl;
}

//Функция для отображения меню выбора методов обхода
void displayBFSMenu() {
    cout << "\nВыберите метод обхода" << endl;
    cout << "1. Матрица смежности + стандартная очередь" << endl;
    cout << "2. Списки смежности + стандартная очередь" << endl;
    cout << "3. Матрица смежности + самодельная очередь" << endl;
    cout << "4. Списки смежности + самодельная очередь" << endl;
    cout << "5. Все методы последовательно" << endl;
    cout << "0. Назад в главное меню" << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int V = 0;// Количество вершин
    int start = 0;// Стартовая вершина для обхода
    Graph* g = nullptr;// Указатель на граф (изначально не создан)
    //Первоначальный ввод параметров графа
    V = safeInputInt("Введите количество вершин графа: ", 1);
    start = safeInputInt("Введите стартовую вершину (0-" + to_string(V - 1) + "): ", 0, V - 1);
    //Создание и генерация начального графа
    g = new Graph(V);
    g->generateRandomGraph();
    int mainChoice;
    do {
        displayMainMenu();
        mainChoice = safeInputInt("Ваш выбор: ", 0, 6);
        vector<int> result;// Для хранения результатов обхода
        switch (mainChoice) {
        case 1:// Показать текущий граф
            if (g != nullptr) {
                cout << "Количество вершин: " << V << endl;
                cout << "Стартовая вершина: " << start << endl;
                g->printAdjMatrix();// Вывод матрицы смежности
                g->printAdjLists();// Вывод списков смежности
            }
            else {
                cout << "Граф не создан!" << endl;
            }
            break;

        case 2:// Выполнить обход графа
            if (g != nullptr) {
                int bfsChoice;// Переменная для выбора метода обхода
                do {
                    displayBFSMenu();
                    bfsChoice = safeInputInt("Ваш выбор: ", 0, 5);
                    switch (bfsChoice) {
                    case 1:// Матрица + стандартная очередь
                        cout << "\nМетод 1: Матрица смежности + стандартная очередь" << endl;
                        result = g->BFS_Matrix_StdQueue(start);
                        Graph::printBFSResult(result, "матрица + queue");
                        break;

                    case 2:// Списки + стандартная очередь
                        cout << "\nМетод 2: Списки смежности + стандартная очередь" << endl;
                        result = g->BFS_List_StdQueue(start);
                        Graph::printBFSResult(result, "списки + queue");
                        break;

                    case 3:// Матрица + самодельная очередь
                        cout << "\nМетод 3: Матрица смежности + самодельная очередь" << endl;
                        result = g->BFS_Matrix_CustomQueue(start);
                        Graph::printBFSResult(result, "матрица + custom queue");
                        break;

                    case 4:// Списки + самодельная очередь
                        cout << "\nМетод 4: Списки смежности + самодельная очередь" << endl;
                        result = g->BFS_List_CustomQueue(start);
                        Graph::printBFSResult(result, "списки + custom queue");
                        break;

                    case 5:// Все методы последовательно
                        cout << "\nВсе методы последовательно" << endl;
                        cout << "\n1. Матрица смежности + стандартная очередь:" << endl;
                        result = g->BFS_Matrix_StdQueue(start);
                        Graph::printBFSResult(result, "матрица + queue");
                        cout << "2. Списки смежности + стандартная очередь:" << endl;
                        result = g->BFS_List_StdQueue(start);
                        Graph::printBFSResult(result, "списки + queue");
                        cout << "3. Матрица смежности + самодельная очередь:" << endl;
                        result = g->BFS_Matrix_CustomQueue(start);
                        Graph::printBFSResult(result, "матрица + custom queue");
                        cout << "4. Списки смежности + самодельная очередь:" << endl;
                        result = g->BFS_List_CustomQueue(start);
                        Graph::printBFSResult(result, "списки + custom queue");
                        break;

                    case 0:// Возврат в главное меню
                        break;
                    }
                } while (bfsChoice != 0);// Цикл меню обхода
            }
            else {
                cout << "Граф не создан!" << endl;
            }
            break;

        case 3:// Изменить стартовую вершину
            if (g != nullptr) {
                start = safeInputInt("Введите новую стартовую вершину (0-" + to_string(V - 1) + "): ", 0, V - 1);
                cout << "Стартовая вершина изменена на: " << start << endl;
            }
            else {
                cout << "Граф не создан!" << endl;
            }
            break;

        case 4:// Перегенерировать граф
            if (g != nullptr) {
                g->generateRandomGraph();
                g->printAdjMatrix();
                g->printAdjLists();
            }
            else {
                cout << "Граф не создан!" << endl;
            }
            break;

        case 5:// Перегенерировать граф с петлями
            if (g != nullptr) {
                g->generateRandomGraphWithLoops();
                g->printAdjMatrix();
                g->printAdjLists();
            }
            else {
                cout << "Граф не создан!" << endl;
            }
            break;

        case 6:// Создать полностью новый граф
        {
            int newV = safeInputInt("Введите количество вершин для нового графа: ", 1);
            V = newV;
            delete g;
            g = new Graph(V);
            int graphType = safeInputInt("Выберите тип графа (1 - без петель, 2 - с петлями): ", 1, 2);
            if (graphType == 1)
                g->generateRandomGraph();
            else {
                g->generateRandomGraphWithLoops();
                cout << "Создан новый граф с возможными петлями" << endl;
            }
            start = safeInputInt("Введите стартовую вершину (0-" + to_string(V - 1) + "): ", 0, V - 1);
            g->printAdjMatrix();
            g->printAdjLists();
        }
        break;

        case 0:// Выход из программы
            break;
        }

    } while (mainChoice != 0);// Главный цикл программы
    //Освобождение памяти перед выходом
    if (g != nullptr) {
        delete g;
    }
    return 0;
}