#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#define MAX_VERTICES 20// Количество вершин
#define STACK_SIZE 100 

int stack[STACK_SIZE];// Стек для хранения вершин при обходе
int top = -1;// Вершина стека

//Функция добавления вершины в стек
void push(int vertex) {
    if (top < STACK_SIZE - 1) {
        stack[++top] = vertex;
    }
}

//Функция извлечения вершины из стека
int pop() {
    if (top >= 0) {
        return stack[top--];
    }
    return -1;  // Возвращаем -1 если стек пуст
}

//Функция проверки пустоты стека
int isEmpty() {
    return top == -1;
}

//Нерекурсивный обход в глубину
void DFS_nonRecursive(int graph[MAX_VERTICES][MAX_VERTICES], int start_vertex, int num_vertices) {
    int visited[MAX_VERTICES];// Массив для отслеживания посещенных вершин
    //Инициализация массива посещенных вершин
    for (int i = 0; i < num_vertices; i++) {
        visited[i] = 0;
    }
    //Помещаем начальную вершину в стек и отмечаем как посещенную
    push(start_vertex);
    visited[start_vertex] = 1;
    printf("Порядок обхода в глубину (нерекурсивный): ");
    //Пока стек не пуст, продолжаем обход
    while (!isEmpty()) {
        int current_vertex = pop();// Извлекаем вершину из стека
        printf("%d ", current_vertex + 1);// Выводим номер вершины (начиная с 1)
        //Обходим смежные вершины в обратном порядке (для сохранения порядка как в рекурсивной версии)
        for (int i = num_vertices - 1; i >= 0; i--) {
            // Если есть связь и вершина не посещена
            if (graph[current_vertex][i] == 1 && !visited[i]) {
                push(i);// Добавляем вершину в стек
                visited[i] = 1;// Отмечаем как посещенную
            }
        }
    }
    printf("\n");
}

//Функция для генерации случайной матрицы смежности
void generateRandomGraph(int graph[MAX_VERTICES][MAX_VERTICES], int num_vertices) {
    srand(time(NULL));
    //Заполнение матрицы смежности
    for (int i = 0; i < num_vertices; i++) {
        for (int j = i; j < num_vertices; j++) {
            if (i == j) {
                graph[i][j] = 0;// Нет петель
            }
            else {
                graph[i][j] = rand() % 2;
                graph[j][i] = graph[i][j];
            }
        }
    }
}

//Функция для вывода матрицы смежности
void printGraph(int graph[MAX_VERTICES][MAX_VERTICES], int num_vertices) {
    printf("\nМатрица смежности:\n");
    printf("   ");
    for (int i = 0; i < num_vertices; i++) {
        printf("%2d ", i + 1);
    }
    printf("\n");
    for (int i = 0; i < num_vertices; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < num_vertices; j++) {
            printf("%2d ", graph[i][j]);
        }
        printf("\n");
    }
}

int main() {
    setlocale(LC_ALL, "ru");
    int num_vertices;// Количество вершин в графе
    int graph[MAX_VERTICES][MAX_VERTICES];// Матрица смежности
    printf("Введите количество вершин графа (от 1 до %d): ", MAX_VERTICES);
    scanf("%d", &num_vertices);
    if (num_vertices < 1 || num_vertices > MAX_VERTICES) {
        printf("Ошибка: количество вершин должно быть от 1 до %d\n", MAX_VERTICES);
        return 1;
    }
    generateRandomGraph(graph, num_vertices);
    printGraph(graph, num_vertices);
    //Выполнение нерекурсивного обхода в глубину
    DFS_nonRecursive(graph, 0, num_vertices);  // Начинаем обход с вершины 0
    return 0;
}