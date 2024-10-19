#include <iostream>
#include <unordered_map>
#include <vector>

struct LZ78Token {
  uint8_t index;
  char nextChar;
};

// Функция кодирования LZ78
std::vector<LZ78Token> encodeLZ78(const std::string &input) {
  std::unordered_map<std::string, uint8_t> dictionary;
  std::vector<LZ78Token> tokens;

  std::string current;
  int dictIndex = 1;

  for (char c : input) {
    current += c;

    if (dictionary.find(current) == dictionary.end()) {
      // Добавляем токен в список
      dictionary[current] = dictIndex++;
      // Если текущая строка имеет размер больше 1, мы берем подстроку без
      // последнего символа
      tokens.push_back({dictionary[current.substr(0, current.size() - 1)], c});
      current = "";
    }
  }

  // Если после цикла осталась непустая часть строки, добавляем её в токены
  if (!current.empty()) {
    tokens.push_back(
        {dictionary[current.substr(0, current.size() - 1)], current.back()});
  }

  return tokens;
}

// Функция декодирования LZ78
std::string decodeLZ78(const std::vector<LZ78Token> &tokens) {
  std::unordered_map<uint8_t, std::string> dictionary;
  std::string decoded;
  int dictIndex = 1;

  for (const auto &token : tokens) {
    std::string entry = (token.index > 0) ? dictionary[token.index] : "";
    entry += token.nextChar;
    decoded += entry;
    dictionary[dictIndex++] = entry;
  }

  return decoded;
}

int main() {
  std::string input = "porpoterpoterporter";

  std::vector<LZ78Token> tokens = encodeLZ78(input);

  // Вывод закодированных данных
  std::cout << "LZ78 кодирование:" << std::endl;
  for (const auto &token : tokens) {
    std::cout << "(" << static_cast<int>(token.index) << ", " << token.nextChar
              << ")" << std::endl;
  }

  // Декодирование
  std::string decoded = decodeLZ78(tokens);

  // Вывод декодированной строки и проверка сходства с исходной строкой
    std::cout << "Восстановленная строка: " << decoded << std::endl;
    std::cout << "Сходство: " << (input == decoded ? "успех" : "ошибка") << std::endl;

    return 0;
}
