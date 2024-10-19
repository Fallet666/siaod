#include <algorithm>
#include <iostream>
#include <vector>

enum Direction { NORTH, SOUTH, EAST, WEST };

class Castle {
private:
  int rows;
  int cols;
  std::vector<std::vector<int>> walls; // 0 - нет стены, 1 - стена
  std::vector<std::vector<bool>> visited;

public:
  Castle(int m, int n) : rows(m), cols(n) {
    walls.resize(rows, std::vector<int>(cols, 0)); // Инициализируем стены
    visited.resize(rows,
                   std::vector<bool>(
                       cols, false)); // Инициализируем массив посещенных клеток
  }

  void setWall(int x, int y, Direction dir) {
    if (dir == NORTH) {
      if (x > 0)
        walls[x - 1][y] |= (1 << SOUTH);
    } else if (dir == SOUTH) {
      if (x < rows - 1)
        walls[x][y] |= (1 << SOUTH);
    } else if (dir == EAST) {
      if (y < cols - 1)
        walls[x][y] |= (1 << EAST);
    } else if (dir == WEST) {
      if (y > 0)
        walls[x][y] |= (1 << WEST);
    }
  }

  bool hasWall(int x, int y, Direction dir) {
    return (walls[x][y] & (1 << dir)) != 0;
  }

  int countArea(int x, int y) {
    // Проверка выхода за границы
    if (x < 0 || x >= rows || y < 0 || y >= cols || visited[x][y]) {
      return 0;
    }

    // Проверка наличия стены
    if (hasWall(x, y, NORTH) && (x == 0 || visited[x - 1][y]))
      return 0;
    if (hasWall(x, y, SOUTH) && (x == rows - 1 || visited[x + 1][y]))
      return 0;
    if (hasWall(x, y, WEST) && (y == 0 || visited[x][y - 1]))
      return 0;
    if (hasWall(x, y, EAST) && (y == cols - 1 || visited[x][y + 1]))
      return 0;

    visited[x][y] = true; // Помечаем клетку как посещенную
    int area = 1;         // Считаем текущую клетку

    // Обход всех соседей
    area += countArea(x - 1, y); // Север
    area += countArea(x + 1, y); // Юг
    area += countArea(x, y - 1); // Запад
    area += countArea(x, y + 1); // Восток

    return area; // Возвращаем общую площадь
  }

  void findRooms(int &roomCount, int &maxArea) {
    visited.assign(rows, std::vector<bool>(cols, false));
    roomCount = 0;
    maxArea = 0;

    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        if (!visited[i][j]) {
          roomCount++;
          int area = countArea(i, j);
          maxArea = std::max(maxArea, area);
        }
      }
    }
  }

  void bruteForce(int &maxCombinedArea, std::pair<int, int> &bestWallToRemove) {
    maxCombinedArea = 0;
    std::pair<int, int> currentWall;

    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        // Проверка стен
        if (hasWall(i, j, NORTH) && i > 0) {
          // Удаление стены на севере
          walls[i][j] ^= (1 << NORTH);
          walls[i - 1][j] ^= (1 << SOUTH);

          int roomCount, maxArea;
          findRooms(roomCount, maxArea);
          int combinedArea = maxArea; // Площадь объединенной комнаты

          if (combinedArea > maxCombinedArea) {
            maxCombinedArea = combinedArea;
            bestWallToRemove = {i, j}; // Запоминаем стену
          }

          // Возврат стены на место
          walls[i][j] |= (1 << NORTH);
          walls[i - 1][j] |= (1 << SOUTH);
        }

        if (hasWall(i, j, SOUTH) && i < rows - 1) {
          // Удаление стены на юге
          walls[i][j] ^= (1 << SOUTH);
          walls[i + 1][j] ^= (1 << NORTH);

          int roomCount, maxArea;
          findRooms(roomCount, maxArea);
          int combinedArea = maxArea;

          if (combinedArea > maxCombinedArea) {
            maxCombinedArea = combinedArea;
            bestWallToRemove = {i, j}; // Запоминаем стену
          }

          // Возврат стены на место
          walls[i][j] |= (1 << SOUTH);
          walls[i + 1][j] |= (1 << NORTH);
        }

        if (hasWall(i, j, WEST) && j > 0) {
          // Удаление стены на западе
          walls[i][j] ^= (1 << WEST);
          walls[i][j - 1] ^= (1 << EAST);

          int roomCount, maxArea;
          findRooms(roomCount, maxArea);
          int combinedArea = maxArea;

          if (combinedArea > maxCombinedArea) {
            maxCombinedArea = combinedArea;
            bestWallToRemove = {i, j}; // Запоминаем стену
          }

          // Возврат стены на место
          walls[i][j] |= (1 << WEST);
          walls[i][j - 1] |= (1 << EAST);
        }

        if (hasWall(i, j, EAST) && j < cols - 1) {
          // Удаление стены на востоке
          walls[i][j] ^= (1 << EAST);
          walls[i][j + 1] ^= (1 << WEST);

          int roomCount, maxArea;
          findRooms(roomCount, maxArea);
          int combinedArea = maxArea;

          if (combinedArea > maxCombinedArea) {
            maxCombinedArea = combinedArea;
            bestWallToRemove = {i, j}; // Запоминаем стену
          }

          // Возврат стены на место
          walls[i][j] |= (1 << EAST);
          walls[i][j + 1] |= (1 << WEST);
        }
      }
    }
  }

  void printRoomInfo(int roomCount, int maxArea,
                     std::pair<int, int> wallPosition) {
    std::cout << "Количество комнат: " << roomCount << std::endl;
    std::cout << "Площадь наибольшей комнаты: " << maxArea << std::endl;
    std::cout << "Стену следует удалить из клетки (" << wallPosition.first
              << ", " << wallPosition.second << ")\n";
  }
};

int main() {
  Castle castle(5, 5);

  // Пример установки стен
  castle.setWall(1, 0, EAST);
  castle.setWall(1, 1, EAST);
  castle.setWall(1, 2, EAST);
  castle.setWall(1, 3, EAST);
    castle.setWall(2, 0, SOUTH);
    castle.setWall(2, 1, SOUTH);
    castle.setWall(2, 2, SOUTH);
    castle.setWall(2, 3, SOUTH);
    castle.setWall(3, 0, SOUTH);
    castle.setWall(3, 1, SOUTH);
    castle.setWall(3, 2, SOUTH);
    castle.setWall(3, 3, SOUTH);

    int roomCount, maxArea;
    castle.findRooms(roomCount, maxArea);

    int maxCombinedArea;
    std::pair<int, int> wallPosition;
    castle.bruteForce(maxCombinedArea, wallPosition);

    castle.printRoomInfo(roomCount, maxArea, wallPosition);

    return 0;
}
