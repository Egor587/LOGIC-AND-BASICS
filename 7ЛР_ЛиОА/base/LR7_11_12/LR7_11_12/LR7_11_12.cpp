#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#define MAX_VERTICES 20// Максимальное количество вершин в графе

int visited[MAX_VERTICES];// Массив для отслеживания посещенных вершин

//Рекурсивная функция обхода в глубину (Depth-First Search)
void DFS(int graph[MAX_VERTICES][MAX_VERTICES], int vertex, int num_vertices) {
    visited[vertex] = 1;// Помечаем текущую вершину как посещенную
    printf("%d ", vertex + 1);// Выводим номер вершины (начиная с 1)
    //Просматриваем все смежные вершины
    for (int i = 0; i < num_vertices; i++) {
        //Если есть связь с вершиной i и она еще не посещена
        if (graph[vertex][i] == 1 && !visited[i]) {
            DFS(graph, i, num_vertices);// Рекурсивно переходим к вершине i
        }
    }
}

//Функция для генерации случайной матрицы смежности
void generateRandomGraph(int graph[MAX_VERTICES][MAX_VERTICES], int num_vertices) {
    srand(time(NULL));
    //Заполняем матрицу смежности
    for (int i = 0; i < num_vertices; i++) {
        for (int j = i; j < num_vertices; j++) {
            if (i == j) {
                graph[i][j] = 0;// Нет петель
            }
            else {
                //Случайным образом определяем наличие связи между вершинами
                graph[i][j] = rand() % 2;
                graph[j][i] = graph[i][j];// Граф неориентированный
            }
        }
    }
}

//Функция для вывода матрицы смежности на экран
void printGraph(int graph[MAX_VERTICES][MAX_VERTICES], int num_vertices) {
    printf("\nМатрица смежности:\n");
    printf("   ");
    for (int i = 0; i < num_vertices; i++) {
        printf("%2d ", i + 1);// Нумерация столбцов с 1
    }
    printf("\n");
    //Вывод самой матрицы
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
    int graph[MAX_VERTICES][MAX_VERTICES];// Матрица смежности графа
    printf("Введите количество вершин графа (от 1 до %d): ", MAX_VERTICES);
    scanf("%d", &num_vertices);
    if (num_vertices < 1 || num_vertices > MAX_VERTICES) {
        printf("Ошибка: количество вершин должно быть от 1 до %d\n", MAX_VERTICES);
        return 1;
    }
    //Инициализация массива посещенных вершин
    for (int i = 0; i < num_vertices; i++) {
        visited[i] = 0; // 0 - вершина не посещена, 1 - посещена
    }

    //Генерация случайного графа
    generateRandomGraph(graph, num_vertices);

    //Вывод матрицы смежности
    printGraph(graph, num_vertices);

    //Выполнение обхода в глубину
    printf("\nПорядок обхода в глубину: ");
    for (int i = 0; i < num_vertices; i++) {
        //Если вершина еще не посещена, начинаем обход из нее
        if (!visited[i]) {
            DFS(graph, i, num_vertices);
        }
    }
    printf("\n");
    return 0;
}