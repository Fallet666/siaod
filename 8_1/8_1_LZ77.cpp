#include <iostream>
#include <string>
#include <vector>

struct LZ77Token {
  int offset;
  int length;
  char nextChar;
};

// Функция декодирования для LZ77
std::string decodeLZ77(const std::vector<LZ77Token> &tokens) {
  std::string decoded;

  for (const auto &token : tokens) {
    if (token.offset == 0 && token.length == 0) {
      decoded += token.nextChar;
    } else {
      int start = static_cast<int>(decoded.size()) - token.offset;
      for (int i = 0; i < token.length; ++i) {
        decoded += decoded[start + i];
      }
      decoded += token.nextChar;
    }
  }

  return decoded;
}

std::vector<LZ77Token> encodeLZ77(const std::string &input, int windowSize) {
  std::vector<LZ77Token> tokens;
  int i = 0;

  while (i < input.length()) {
    int matchLength = 0;
    int matchOffset = 0;
    char nextChar = input[i];

    // Поиск самой длинной совпадающей подстроки в пределах окна
    for (int j = std::max(0, i - windowSize); j < i; ++j) {
      int length = 0;
      while (i + length < input.length() &&
             input[j + length] == input[i + length])
        length++;

      if (length > matchLength) {
        matchLength = length;
        matchOffset = i - j;
        nextChar = input[i + matchLength];
      }
    }

    tokens.push_back({matchOffset, matchLength, nextChar});
    i += matchLength + 1;
  }

  return tokens;
}

int main() {
  std::string input = "0010100110010000001";
  int windowSize = 6;

  std::vector<LZ77Token> tokens = encodeLZ77(input, windowSize);

  std::cout << "LZ77 кодирование:" << std::endl;
  for (const auto &token : tokens) {
    std::cout << "(" << token.offset << ", " << token.length << ", "
              << token.nextChar << ")" << std::endl;
  }
  // Восстановим строку
  std::string decoded = decodeLZ77(tokens);

  std::cout << "Восстановленная строка: " << decoded << std::endl;
    std::cout << "Сходство: "<< (input==decoded?"успех":"ошибка")<< std::endl;

    return 0;
}
