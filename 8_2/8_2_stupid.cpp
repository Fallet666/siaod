#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

enum Direction { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };

class Castle
{
private:
  int rows;
  int cols;
  std::vector<std::vector<int>>
  walls; // 0 - нет стены, битовая маска для сторон
  std::vector<std::vector<bool>> visited;

public:
  Castle(int m, int n) : rows(m), cols(n)
  {
    walls.resize(rows, std::vector<int>(cols, 0)); // Инициализируем стены
    visited.resize(rows,
                   std::vector<bool>(cols, false)); // Массив посещённых клеток
  }

  void setWall(int x, int y, Direction dir)
  {
    if (dir == NORTH && x > 0)
    {
      walls[x][y] |= (1 << NORTH);
      walls[x - 1][y] |= (1 << SOUTH);
    }
    else if (dir == SOUTH && x < rows - 1)
    {
      walls[x][y] |= (1 << SOUTH);
      walls[x + 1][y] |= (1 << NORTH);
    }
    else if (dir == EAST && y < cols - 1)
    {
      walls[x][y] |= (1 << EAST);
      walls[x][y + 1] |= (1 << WEST);
    }
    else if (dir == WEST && y > 0)
    {
      walls[x][y] |= (1 << WEST);
      walls[x][y - 1] |= (1 << EAST);
    }
  }

  bool hasWall(int x, int y, Direction dir) { return walls[x][y] & (1 << dir); }

  int countArea(int x, int y)
  {
    if (x < 0 || x >= rows || y < 0 || y >= cols || visited[x][y])
    {
      return 0;
    }

    visited[x][y] = true;
    int area = 1;

    // Проверяем соседей
    if (!hasWall(x, y, NORTH))
    {
      area += countArea(x - 1, y);
    }
    if (!hasWall(x, y, SOUTH))
    {
      area += countArea(x + 1, y);
    }
    if (!hasWall(x, y, EAST))
    {
      area += countArea(x, y + 1);
    }
    if (!hasWall(x, y, WEST))
    {
      area += countArea(x, y - 1);
    }

    return area;
  }

  void findRooms(int& roomCount, int& maxArea)
  {
    visited.assign(rows, std::vector<bool>(cols, false));
    roomCount = 0;
    maxArea = 0;

    for (int i = 0; i < rows; ++i)
    {
      for (int j = 0; j < cols; ++j)
      {
        if (!visited[i][j])
        {
          roomCount++;
          maxArea = std::max(maxArea, countArea(i, j));
        }
      }
    }
  }

  void bruteForce(int& maxCombinedArea, std::pair<int, int>& bestWallToRemove,
                  Direction& bestDirection)
  {
    maxCombinedArea = 0;

    for (int i = 0; i < rows; ++i)
    {
      for (int j = 0; j < cols; ++j)
      {
        for (int dir = 0; dir < 4; ++dir)
        {
          int nx = i + (dir == SOUTH) - (dir == NORTH);
          int ny = j + (dir == EAST) - (dir == WEST);

          // Проверяем, можно ли удалить стену
          if (nx >= 0 && nx < rows && ny >= 0 && ny < cols &&
            hasWall(i, j, static_cast<Direction>(dir)))
          {
            // Удаляем стену
            walls[i][j] ^= (1 << dir);
            walls[nx][ny] ^= (1 << ((dir + 2) % 4));

            visited.assign(rows, std::vector<bool>(cols, false));
            int area1 = countArea(i, j);

            visited.assign(rows, std::vector<bool>(cols, false));
            int area2 = countArea(nx, ny);

            int combinedArea = area1 + area2;

            if (combinedArea > maxCombinedArea)
            {
              maxCombinedArea = combinedArea;
              bestWallToRemove = {i, j};
              bestDirection = static_cast<Direction>(dir);
            }

            // Восстанавливаем стену
            walls[i][j] ^= (1 << dir);
            walls[nx][ny] ^= (1 << ((dir + 2) % 4));
          }
        }
      }
    }
  }

  void printLayout()
  {
    for (int i = 0; i < rows; ++i)
    {
      for (int j = 0; j < cols; ++j)
      {
        std::cout << "+";
        if (hasWall(i, j, NORTH) || i == 0)
        {
          std::cout << "---";
        }
        else
        {
          std::cout << "   ";
        }
      }
      std::cout << "+" << std::endl;

      for (int j = 0; j < cols; ++j)
      {
        if (hasWall(i, j, WEST) || j == 0)
        {
          std::cout << "|";
        }
        else
        {
          std::cout << " ";
        }
        std::cout << "   ";
      }
      std::cout << "|" << std::endl;
    }

    for (int j = 0; j < cols; ++j)
    {
      std::cout << "+---";
    }
    std::cout << "+" << std::endl;
  }

  void printRoomInfo(int roomCount, int maxArea,
                     const std::pair<int, int>& wallPosition,
                     Direction bestDir)
  {
    std::cout << "Количество комнат: " << roomCount << std::endl;
    std::cout << "Площадь наибольшей комнаты: " << maxArea << std::endl;
    std::cout << "Стену следует удалить из клетки (" << wallPosition.first
      << ", " << wallPosition.second << ") в направлении " << bestDir
      << std::endl;
  }
};

int main()
{
  Castle castle(5, 5);

  // Пример установки стен
  // Добавление горизонтальных стен
  castle.setWall(0, 0, EAST);
  castle.setWall(0, 1, EAST);
  castle.setWall(1, 0, EAST);
  castle.setWall(1, 1, EAST);
  castle.setWall(1, 2, EAST);
  castle.setWall(2, 1, EAST);
  castle.setWall(2, 2, EAST);
  castle.setWall(2, 3, EAST);
  castle.setWall(3, 3, EAST);

  // Добавление вертикальных стен
  castle.setWall(0, 0, SOUTH);
  castle.setWall(0, 1, SOUTH);
  castle.setWall(1, 1, SOUTH);
  castle.setWall(1, 2, SOUTH);
  castle.setWall(2, 2, SOUTH);
  castle.setWall(2, 3, SOUTH);
  castle.setWall(3, 3, SOUTH);
  castle.setWall(3, 4, SOUTH);

  int roomCount, maxArea;
  castle.findRooms(roomCount, maxArea);


  int maxCombinedArea;
  std::pair<int, int> wallPosition;
  Direction bestDir;
  auto start = std::chrono::high_resolution_clock::now();
  castle.bruteForce(maxCombinedArea, wallPosition, bestDir);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Время выполнения findBestWallToRemove: " << std::chrono::duration_cast<
    std::chrono::nanoseconds>(end - start).count() << " наносекунд" << std::endl;
  castle.printRoomInfo(roomCount, maxArea, wallPosition, bestDir);
  std::cout << "\nПланировка замка:\n";
  castle.printLayout();

  return 0;
}
