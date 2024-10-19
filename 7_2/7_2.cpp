#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

const int INF = std::numeric_limits<int>::max(); // Определяем бесконечность

// Функция для вывода матрицы
void printMatrix(const std::vector<std::vector<int>> &matrix) {
  int V = matrix.size();
  for (int i = 0; i < V; ++i) {
    for (int j = 0; j < V; ++j) {
      if (matrix[i][j] == INF)
        std::cout << "-"
                  << " ";
      else
        std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

// Алгоритм Флойда для нахождения кратчайших путей
void floydWarshall(std::vector<std::vector<int>> &graph) {
  int V = graph.size();
  std::vector<std::vector<int>> dist = graph;

  for (int k = 0; k < V; ++k) {
    for (int i = 0; i < V; ++i) {
      for (int j = 0; j < V; ++j) {
        if (dist[i][k] != INF && dist[k][j] != INF &&
            dist[i][k] + dist[k][j] < dist[i][j]) {
          dist[i][j] = dist[i][k] + dist[k][j];
        }
      }
    }
  }

  std::cout << "Кратчайшие пути между всеми парами вершин:" << std::endl;
  printMatrix(dist);
}

int main() {
  std::ifstream a("/Users/user/CLionProjects/siaod_speed_run/input.txt");
  int V;
  a >> V;

  // Создаем матрицу смежности графа
  std::vector<std::vector<int>> graph(V, std::vector<int>(V, INF));

  for (int i = 0; i < V; ++i) {
    for (int j = 0; j < V; ++j) {
      int weight;
      a >> weight;
      if (i == j) {
        graph[i][j] = 0; // Расстояние от вершины до самой себя
      } else if (weight != 0) {
        graph[i][j] = weight;
      }
    }
  }

  // Создаем матрицу смежности графа
  std::vector<std::vector<int>> graph_test = {
      {0, 2, 4, 5, INF, INF},      // Ребра от вершины 1
      {INF, 0, INF, 4, 3, INF},    // Ребра от вершины 2
      {INF, INF, 0, INF, INF, 7},  // Ребра от вершины 3
      {INF, INF, INF, 0, 6, 4},  // Ребра от вершины 4
        {INF, INF, INF, INF, 0, 2},  // Ребра от вершины 5
        {INF, INF, INF, INF, INF, 0}   // Ребра от вершины 6
    };

    // Вызов алгоритма Флойда
    floydWarshall(graph_test);

    return 0;
}
