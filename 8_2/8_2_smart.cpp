#include <algorithm>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <vector>

enum Direction { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };

class Castle
{
private:
    int rows;
    int cols;
    std::vector<std::vector<int>> walls; // Хранение стен в виде битовой маски
    std::vector<std::vector<bool>> visited; // Для обхода при подсчёте комнат
    std::vector<std::vector<int>> roomIds; // Идентификаторы комнат
    std::unordered_map<int, int> roomSizes; // Размеры комнат

    const int dx[4] = {-1, 0, 1, 0};
    const int dy[4] = {0, 1, 0, -1};

    int currentRoomId = 0;

public:
    Castle(int m, int n) : rows(m), cols(n)
    {
        walls.resize(rows, std::vector<int>(cols, 0));
        visited.resize(rows, std::vector<bool>(cols, false));
        roomIds.resize(rows, std::vector<int>(cols, -1));
    }

    void setWall(int x, int y, Direction dir)
    {
        walls[x][y] |= (1 << dir);
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        if (nx >= 0 && nx < rows && ny >= 0 && ny < cols)
        {
            walls[nx][ny] |= (1 << ((dir + 2) % 4));
        }
    }

    bool hasWall(int x, int y, Direction dir) const
    {
        return walls[x][y] & (1 << dir);
    }

    int countArea(int x, int y)
    {
        if (x < 0 || x >= rows || y < 0 || y >= cols || visited[x][y])
        {
            return 0;
        }

        visited[x][y] = true;
        roomIds[x][y] = currentRoomId;
        int area = 1;

        for (int dir = 0; dir < 4; ++dir)
        {
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols &&
                !hasWall(x, y, static_cast<Direction>(dir)))
            {
                area += countArea(nx, ny);
            }
        }

        return area;
    }

    void findRooms(int& roomCount, int& maxArea)
    {
        visited.assign(rows, std::vector<bool>(cols, false));
        roomSizes.clear();
        currentRoomId = 0;
        roomCount = 0;
        maxArea = 0;

        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                if (roomIds[i][j] == -1)
                {
                    currentRoomId++;
                    int area = countArea(i, j);
                    roomSizes[currentRoomId] = area;
                    roomCount++;
                    maxArea = std::max(maxArea, area);
                }
            }
        }
    }

    void findBestWallToRemove(int& maxCombinedArea,
                              std::pair<int, int>& wallPosition,
                              Direction& bestDir)
    {
        maxCombinedArea = 0;

        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                for (int dir = 0; dir < 4; ++dir)
                {
                    int nx = i + dx[dir];
                    int ny = j + dy[dir];

                    if (nx >= 0 && nx < rows && ny >= 0 && ny < cols &&
                        hasWall(i, j, static_cast<Direction>(dir)))
                    {
                        int room1 = roomIds[i][j];
                        int room2 = roomIds[nx][ny];

                        if (room1 != room2)
                        {
                            // Если комнаты разные
                            int combinedArea = roomSizes[room1] + roomSizes[room2];
                            if (combinedArea > maxCombinedArea)
                            {
                                maxCombinedArea = combinedArea;
                                wallPosition = {i, j};
                                bestDir = static_cast<Direction>(dir);
                            }
                        }
                    }
                }
            }
        }
    }

    void printLayout() const
    {
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                std::cout << "+";
                if (hasWall(i, j, NORTH))
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
                if (hasWall(i, j, WEST))
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

    static void printRoomInfo(int roomCount, int maxArea,
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
    castle.findBestWallToRemove(maxCombinedArea, wallPosition, bestDir);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Время выполнения findBestWallToRemove: "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
        << " наносекунд" << std::endl;

    Castle::printRoomInfo(roomCount, maxArea, wallPosition, bestDir);
    std::cout << "\nПланировка замка:\n";
    castle.printLayout();

    return 0;
}
