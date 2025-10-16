#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#define MAX_VERTICES 20

//Структура для узла списка смежности
typedef struct Node {
    int vertex;// Номер вершины, с которой есть связь
    struct Node* next;// Указатель на следующий узел
} Node;

Node* adjacencyList[MAX_VERTICES];// Массив указателей на списки смежности для каждой вершины
int visited[MAX_VERTICES];// Массив для отслеживания посещенных вершин

//Функция добавления ребра в список смежности
void addEdge(int src, int dest) {
    //Создаем новый узел для направления src -> dest
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = dest;
    newNode->next = adjacencyList[src];
    adjacencyList[src] = newNode;
    //Для неориентированного графа добавляем обратное ребро dest -> src
    newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = src;
    newNode->next = adjacencyList[dest];
    adjacencyList[dest] = newNode;
}

//Функция генерации графа
void generateRandomGraphList(int num_vertices) {
    srand(time(NULL)); 
    //Инициализация всех списков смежности как пустых
    for (int i = 0; i < num_vertices; i++) {
        adjacencyList[i] = NULL;
    }
    //Генерация случайных ребер между вершинами
    for (int i = 0; i < num_vertices; i++) {
        for (int j = i + 1; j < num_vertices; j++) {
            if (rand() % 2 == 1) {
                addEdge(i, j);
            }
        }
    }
}

//Рекурсивная функция обхода в глубину для списков смежности
void DFS_list(int vertex) {
    visited[vertex] = 1; // Помечаем текущую вершину как посещенную
    printf("%d ", vertex + 1);// Выводим номер вершины (начиная с 1)
    //Проходим по всем смежным вершинам в списке смежности
    Node* temp = adjacencyList[vertex];
    while (temp != NULL) {
        int adjVertex = temp->vertex;// Получаем номер смежной вершины
        //Если смежная вершина еще не посещена, рекурсивно переходим к ней
        if (!visited[adjVertex]) {
            DFS_list(adjVertex);
        }
        temp = temp->next;// Переходим к следующему элементу списка
    }
}

//Функция вывода списков смежности
void printAdjacencyList(int num_vertices) {
    printf("Списки смежности:\n");
    for (int i = 0; i < num_vertices; i++) {
        printf("Вершина %d: ", i + 1);
        Node* temp = adjacencyList[i];
        while (temp != NULL) {
            printf("%d ", temp->vertex + 1);
            temp = temp->next;
        }
        printf("\n");
    }
}

int main() {
    setlocale(LC_ALL, "ru");
    int num_vertices;// Количество вершин в графе
    printf("Введите количество вершин графа (от 1 до %d): ", MAX_VERTICES);
    scanf("%d", &num_vertices);
    if (num_vertices < 1 || num_vertices > MAX_VERTICES) {
        printf("Ошибка: количество вершин должно быть от 1 до %d\n", MAX_VERTICES);
        return 1;
    }
    // Инициализация массива посещенных вершин
    for (int i = 0; i < num_vertices; i++) {
        visited[i] = 0;// 0 - вершина не посещена, 1 - посещена
    }
    generateRandomGraphList(num_vertices);
    printAdjacencyList(num_vertices);
    // Выполнение обхода в глубину
    printf("Порядок обхода в глубину: ");
    for (int i = 0; i < num_vertices; i++) {
        // Если вершина еще не посещена, начинаем обход из нее
        if (!visited[i]) {
            DFS_list(i);
        }
    }
    printf("\n");
    return 0;
}