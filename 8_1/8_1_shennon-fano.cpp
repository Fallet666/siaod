#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Symbol {
  char character;
  int frequency;
};

bool compare(const Symbol &a, const Symbol &b) {
  return a.frequency > b.frequency;
}

void shannonFano(const std::vector<Symbol> &symbols,
                 std::unordered_map<char, std::string> &codes, int start,
                 int end, const std::string &code) {
  if (start == end) {
    codes[symbols[start].character] = code;
    return;
  }

  int total = 0;
  for (int i = start; i <= end; ++i)
    total += symbols[i].frequency;

  int leftSum = 0;
  int splitIndex = start;

  // Найдем индекс, при котором деление будет сбалансированным по весам
  for (int i = start; i < end; ++i) {
    leftSum += symbols[i].frequency;
    if (leftSum >= total / 2) {
      splitIndex = i;
      break;
    }
  }

  shannonFano(symbols, codes, start, splitIndex, code + "0");
  shannonFano(symbols, codes, splitIndex + 1, end, code + "1");
}

// Функция восстановления строки
std::string
decodeShannonFano(const std::string &encoded,
                  const std::unordered_map<std::string, char> &reverseCodes) {
  std::string decoded;
  std::string currentCode;

  for (char c : encoded) {
    currentCode += c;
    if (reverseCodes.find(currentCode) != reverseCodes.end()) {
      decoded += reverseCodes.at(currentCode);
      currentCode = "";
    }
  }

  return decoded;
}

void test(const std::string &input, bool debug = false) {
  std::cout << "Задана строка: " << input << std::endl;

  // Подсчитаем частоты символов
  std::unordered_map<char, int> freq;
  for (char c : input) {
    freq[c]++;
  }

  // Создаем вектор символов
  std::vector<Symbol> symbols;
  symbols.reserve(freq.size());
  for (const auto &entry : freq) {
    symbols.push_back({entry.first, entry.second});
  }

  // Сортируем символы по частоте
  std::sort(symbols.begin(), symbols.end(), compare);

  // Генерируем коды
  std::unordered_map<char, std::string> codes;
  shannonFano(symbols, codes, 0, symbols.size() - 1, "");

  // Закодируем строку
  std::string encoded;
  for (char c : input) {
    encoded += codes[c];
  }

  // Расчет размеров
  size_t originalSize = input.size() * 8; // в битах
  size_t compressedSize = encoded.size(); // в битах

  std::cout << "Размер до сжатия: " << originalSize << " бит" << std::endl;
  std::cout << "Размер после сжатия: " << compressedSize << " бит" << std::endl;

  // Расчет коэффициента сжатия
  double compressionRatio = (double)compressedSize / originalSize * 100;
  std::cout << "Коэффициент сжатия: " << compressionRatio << "%" << std::endl;

  if (debug) {
    // Вывод кодов символов
    std::cout << "\nКоды символов:" << std::endl;
    for (const auto &code : codes) {
      std::cout << "'" << code.first << "' : " << code.second << std::endl;
    }
  }

  // Создаем обратный словарь для декодирования
  std::unordered_map<std::string, char> reverseCodes;
  for (const auto &code : codes) {
    reverseCodes[code.second] = code.first;
  }

  // Восстанавливаем строку
  std::string decoded = decodeShannonFano(encoded, reverseCodes);

  std::cout << "\nВосстановленная строка: " << decoded << std::endl;

    // Проверка корректности восстановления
    if (input == decoded) {
        std::cout << "Декодирование успешно." << std::endl;
    } else {
        std::cout << "Декодирование не удалось." << std::endl;
    }
}

int main() {
    std::string input = "Ана-дэус-рики-паки, Дормы-кормыконсту-таки, Энус-дэус-кана-дэус-БАЦ!";
    test(input, true);

    return 0;
}
