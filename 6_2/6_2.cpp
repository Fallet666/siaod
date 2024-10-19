#include <chrono> // Для замера времени
#include <iostream>
#include <random> // Для генерации случайных строк
#include <string>
#include <vector>

const int prime = 101; // Простое число для расчета хеша

// Функция для вычисления хеш-значения строки
long long createHash(const std::string &str, int end) {
  long long hash = 0;
  for (int i = 0; i <= end; ++i) {
    hash += str[i] * pow(prime, i);
  }
  return hash;
}

// Функция для пересчета хеша при сдвиге окна
long long recalculateHash(const std::string &str, int oldIndex, int newIndex,
                          long long oldHash, int patternLen) {
  long long newHash = oldHash - str[oldIndex];
  newHash /= prime;
  newHash += str[newIndex] * pow(prime, patternLen - 1);
  return newHash;
}

// Функция для сравнения строк при совпадении хешей (для избежания коллизий)
bool checkEqual(const std::string &str1, int start1, int end1,
                const std::string &str2, int start2, int end2) {
  if (end1 - start1 != end2 - start2)
    return false;
  while (start1 <= end1 && start2 <= end2) {
    if (str1[start1] != str2[start2])
      return false;
    ++start1;
    ++start2;
  }
  return true;
}

// Основная функция алгоритма Рабина-Карпа
std::vector<int> rabinKarp(const std::string &text,
                           const std::string &pattern) {
  int m = pattern.length();
  int n = text.length();
  std::vector<int> result;

  if (m > n)
    return result;

  long long patternHash = createHash(pattern, m - 1);
  long long textHash = createHash(text, m - 1);

  for (int i = 0; i <= n - m; ++i) {
    if (patternHash == textHash &&
        checkEqual(text, i, i + m - 1, pattern, 0, m - 1)) {
      result.push_back(i); // Нашли вхождение
    }
    if (i < n - m) {
      textHash = recalculateHash(text, i, i + m, textHash, m);
    }
  }
  return result;
}

// Функция для генерации случайной строки заданной длины
std::string generateRandomString(int length) {
  std::string chars = "abcde";
  std::string result;
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<> dist(0, chars.size() - 1);

  for (int i = 0; i < length; ++i) {
    result += chars[dist(generator)];
  }

  return result;
}

// Функция для тестирования
void test(int textLength, int patternLength) {
  // Генерация случайного текста и образца
  std::string text = generateRandomString(textLength);
  std::string pattern = generateRandomString(patternLength);

  // Измерение времени выполнения
  auto start = std::chrono::high_resolution_clock::now();
  std::vector<int> result = rabinKarp(text, pattern);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;

  // Проверка успешности поиска
  bool found = !result.empty();

  // Вывод результатов
  std::cout << "Тестирование с текстом длины " << textLength
            << " и образцом длины " << patternLength << std::endl;
    std::cout << "Успешность поиска: " << (found ? "Образец найден" : "Образец не найден") << std::endl;
    std::cout << "Количество вхождений: " << result.size() << std::endl;
    std::cout << "Время выполнения: " << duration.count() * 1000 << " миллисекунд" << std::endl;
    std::cout << "------------------------" << std::endl;
}

int main() {
    test(100, 2);
    test(1000, 2);
    test(10000, 2);
    test(100000, 2);

    return 0;
}
