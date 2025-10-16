#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define MAX_VERTICES 20// Максимальное количество вершин
#define STACK_SIZE 100// Размер стека для нерекурсивного обхода

//Структура для узла списка смежности
typedef struct Node {
    int vertex;// Номер вершины, с которой есть связь
    struct Node* next;// Указатель на следующий узел в списке
} Node;

// Глобальные переменные
int visited[MAX_VERTICES];// Массив для отслеживания посещенных вершин
Node* adjacencyList[MAX_VERTICES];// Массив указателей на списки смежности
int stack[STACK_SIZE];// Стек для хранения вершин при нерекурсивном обходе
int top = -1;// Вершина стека

// Функция добавления вершины в стек
void push(int vertex) {
    if (top < STACK_SIZE - 1) {
        stack[++top] = vertex;
    }
}

// Функция извлечения вершины из стека
int pop() {
    if (top >= 0) {
        return stack[top--];
    }
    return -1;  // Возвращаем -1 если стек пуст
}

// Функция проверки пустоты стека
int isEmpty() {
    return top == -1;
}

// Рекурсивная функция обхода в глубину (для матрицы смежности)
void DFS_matrix(int graph[MAX_VERTICES][MAX_VERTICES], int vertex, int num_vertices) {
    visited[vertex] = 1;// Помечаем текущую вершину как посещенную
    printf("%d ", vertex + 1);
    //Просматриваем все смежные вершины
    for (int i = 0; i < num_vertices; i++) {
        //Если есть связь с вершиной i и она еще не посещена (исключаем петли при обходе)
        if (graph[vertex][i] == 1 && !visited[i] && vertex != i) {
            DFS_matrix(graph, i, num_vertices);// Рекурсивно переходим к вершине i
        }
    }
}

//Нерекурсивный обход в глубину для матрицы смежности
void DFS_nonRecursive_matrix(int graph[MAX_VERTICES][MAX_VERTICES], int start_vertex, int num_vertices) {
    int visited_local[MAX_VERTICES];// Массив для отслеживания посещенных вершин
    //Инициализация массива посещенных вершин
    for (int i = 0; i < num_vertices; i++) {
        visited_local[i] = 0;
    }
    //Помещаем начальную вершину в стек и отмечаем как посещенную
    push(start_vertex);
    visited_local[start_vertex] = 1;
    printf("Порядок обхода в глубину (нерекурсивный): ");
    //Пока стек не пуст, продолжаем обход
    while (!isEmpty()) {
        int current_vertex = pop();// Извлекаем вершину из стека
        printf("%d ", current_vertex + 1);// Выводим номер вершины (начиная с 1)
        //Обходим смежные вершины в обратном порядке
        for (int i = num_vertices - 1; i >= 0; i--) {
            //Если есть связь и вершина не посещена (исключаем петли при обходе)
            if (graph[current_vertex][i] == 1 && !visited_local[i] && current_vertex != i) {
                push(i);// Добавляем вершину в стек
                visited_local[i] = 1;// Отмечаем как посещенную
            }
        }
    }
    printf("\n");
}

//Функция добавления ребра в список смежности
void addEdge(int src, int dest) {
    // Создаем новый узел для направления src -> dest
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = dest;
    newNode->next = adjacencyList[src];
    adjacencyList[src] = newNode;

    // Для неориентированного графа добавляем обратное ребро dest -> src (кроме петель)
    if (src != dest) {
        newNode = (Node*)malloc(sizeof(Node));
        newNode->vertex = src;
        newNode->next = adjacencyList[dest];
        adjacencyList[dest] = newNode;
    }
}

//Функция генерации случайного графа в виде списков смежности (с петлями)
void generateRandomAdjacencyList(int num_vertices) {
    //Инициализация всех списков смежности как пустых
    for (int i = 0; i < num_vertices; i++) {
        adjacencyList[i] = NULL;
    }
    //Генерация случайных ребер между вершинами (включая петли)
    for (int i = 0; i < num_vertices; i++) {
        for (int j = i; j < num_vertices; j++) { 
            if (rand() % 3 == 1) { 
                addEdge(i, j);
            }
        }
    }
}

//Рекурсивная функция обхода в глубину (для списков смежности)
void DFS_adjacency_list_recursive(int vertex) {
    visited[vertex] = 1;// Помечаем текущую вершину как посещенную
    printf("%d ", vertex + 1);
    //Проходим по всем смежным вершинам в списке смежности
    Node* temp = adjacencyList[vertex];
    while (temp != NULL) {
        int adjVertex = temp->vertex;// Получаем номер смежной вершины
        //Если смежная вершина еще не посещена (исключаем петли при обходе)
        if (!visited[adjVertex] && vertex != adjVertex) {
            DFS_adjacency_list_recursive(adjVertex);
        }
        temp = temp->next;// Переходим к следующему элементу списка
    }
}

//Нерекурсивный обход в глубину для (списков смежности)
void DFS_adjacency_list_nonRecursive(int start_vertex, int num_vertices) {
    int visited_local[MAX_VERTICES];// Массив для отслеживания посещенных вершин
    //Инициализация массива посещенных вершин
    for (int i = 0; i < num_vertices; i++) {
        visited_local[i] = 0;
    }
    //Помещаем начальную вершину в стек и отмечаем как посещенную
    push(start_vertex);
    visited_local[start_vertex] = 1;
    printf("Порядок обхода в глубину (списки смежности, нерекурсивный): ");
    //Пока стек не пуст, продолжаем обход
    while (!isEmpty()) {
        int current_vertex = pop();// Извлекаем вершину из стека
        printf("%d ", current_vertex + 1);
        //Проходим по всем смежным вершинам в списке смежности текущей вершины
        Node* temp = adjacencyList[current_vertex];
        //Сначала собираем все смежные вершины в временный стек для обратного порядка
        int temp_stack[MAX_VERTICES];
        int temp_top = -1;
        while (temp != NULL) {
            int adjVertex = temp->vertex;
            //Исключаем петли при обходе
            if (!visited_local[adjVertex] && current_vertex != adjVertex) {
                temp_stack[++temp_top] = adjVertex;
                visited_local[adjVertex] = 1;// Сразу отмечаем как посещенную
            }
            temp = temp->next;
        }
        //Добавляем вершины из временного стека в основной стек (в обратном порядке)
        for (int i = temp_top; i >= 0; i--) {
            push(temp_stack[i]);
        }
    }
    printf("\n");
}

//Функция для генерации случайной матрицы смежности (с петлями)
void generateRandomMatrix(int graph[MAX_VERTICES][MAX_VERTICES], int num_vertices) {
    //Заполняем матрицу смежности
    for (int i = 0; i < num_vertices; i++) {
        for (int j = i; j < num_vertices; j++) {
            if (i == j) {
                graph[i][j] = rand() % 2;
            }
            else {
                graph[i][j] = rand() % 2;
                graph[j][i] = graph[i][j];  // Граф неориентированный
            }
        }
    }
}

// Функция для вывода матрицы смежности
void printMatrix(int graph[MAX_VERTICES][MAX_VERTICES], int num_vertices) {
    printf("\nМатрица смежности:\n");
    printf("   ");
    for (int i = 0; i < num_vertices; i++) {
        printf("%2d ", i + 1);
    }
    printf("\n");

    // Вывод самой матрицы
    for (int i = 0; i < num_vertices; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < num_vertices; j++) {
            printf("%2d ", graph[i][j]);
        }
        printf("\n");
    }
}

//Функция вывода списков смежности
void printAdjacencyList(int num_vertices) {
    printf("\nСписки смежности:\n");
    for (int i = 0; i < num_vertices; i++) {
        printf("Вершина %d: ", i + 1);
        Node* temp = adjacencyList[i];
        //Выводим все вершины, смежные с текущей
        while (temp != NULL) {
            printf("%d ", temp->vertex + 1);
            temp = temp->next;
        }
        printf("\n");
    }
}

//Функция инициализации массива посещенных вершин
void initializeVisited(int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        visited[i] = 0;// 0 - вершина не посещена, 1 - посещена
    }
}

//Функция освобождения памяти, занятой списками смежности
void freeAdjacencyList(int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        Node* current = adjacencyList[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
        adjacencyList[i] = NULL;
    }
}

//Отображение главного меню
void displayMenu() {
    printf("1 - Матрица смежности (рекурсивный)\n");
    printf("2 - Матрица смежности (нерекурсивный)\n");
    printf("3 - Списки смежности (рекурсивный)\n");
    printf("4 - Списки смежности (нерекурсивный)\n");
    printf("0 - Выход из программы\n");
    printf("Ваш выбор: ");
}

int main() {
    setlocale(LC_ALL, "ru");
    srand(time(NULL));
    int num_vertices;// Количество вершин в графе
    int choice;// Выбор пользователя
    int graph[MAX_VERTICES][MAX_VERTICES]; // Матрица смежности

    while (1) {
        displayMenu();
        scanf("%d", &choice);
        if (choice == 0) {
            printf("Выход из программы. До свидания!\n");
            break;
        }
        printf("\nВведите количество вершин графа (от 1 до %d): ", MAX_VERTICES);
        scanf("%d", &num_vertices);
        if (num_vertices < 1 || num_vertices > MAX_VERTICES) {
            printf("Ошибка: количество вершин должно быть от 1 до %d\n", MAX_VERTICES);
            continue;// Возвращаемся в начало цикла
        }
        switch (choice) {
        case 1: {
            //Матрица смежности (рекурсивный обход)
            generateRandomMatrix(graph, num_vertices);
            printMatrix(graph, num_vertices);
            initializeVisited(num_vertices);
            printf("\nПорядок обхода в глубину (матрица смежности, рекурсивный): ");
            for (int i = 0; i < num_vertices; i++) {
                if (!visited[i]) {
                    DFS_matrix(graph, i, num_vertices);
                }
            }
            printf("\n");
            break;
        }
        case 2: {
            //Матрица смежности (нерекурсивный обход)
            generateRandomMatrix(graph, num_vertices);
            printMatrix(graph, num_vertices);
            //Сброс стека
            top = -1;
            //Выполнение нерекурсивного обхода
            DFS_nonRecursive_matrix(graph, 0, num_vertices);
            break;
        }
        case 3: {
            //Списки смежности (рекурсивный обход)
            generateRandomAdjacencyList(num_vertices);
            printAdjacencyList(num_vertices);
            initializeVisited(num_vertices);
            printf("\nПорядок обхода в глубину (списки смежности, рекурсивный): ");
            for (int i = 0; i < num_vertices; i++) {
                if (!visited[i]) {
                    DFS_adjacency_list_recursive(i);
                }
            }
            printf("\n");
            //Освобождение памяти
            freeAdjacencyList(num_vertices);
            break;
        }
        case 4: {
            //Списки смежности (нерекурсивный обход)
            generateRandomAdjacencyList(num_vertices);
            printAdjacencyList(num_vertices);
            //Сброс стека
            top = -1;
            //Выполнение нерекурсивного обхода
            DFS_adjacency_list_nonRecursive(0, num_vertices);
            //Освобождение памяти
            freeAdjacencyList(num_vertices);
            break;
        }
        default:
            printf("Ошибка: неверный выбор! Пожалуйста, выберите 1, 2, 3, 4 или 0.\n");
            break;
        }
    }
    return 0;
}