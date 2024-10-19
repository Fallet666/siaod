#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

struct Node {
  char character;
  int frequency;
  Node *left;
  Node *right;

  Node(char c, int f)
      : character(c), frequency(f), left(nullptr), right(nullptr) {}
};

// Сравнение узлов для очереди с приоритетом
struct Compare {
  bool operator()(Node *l, Node *r) { return l->frequency > r->frequency; }
};

// Построение дерева Хаффмана
Node *buildHuffmanTree(const std::string &text) {
  // Подсчёт частот
  std::unordered_map<char, int> freq;
  for (char c : text) {
    freq[c]++;
  }

  // Создание очереди с приоритетом
  std::priority_queue<Node *, std::vector<Node *>, Compare> pq;
  for (const auto &pair : freq) {
    pq.push(new Node(pair.first, pair.second));
  }

  // Построение дерева
  while (pq.size() > 1) {
    Node *left = pq.top();
    pq.pop();
    Node *right = pq.top();
    pq.pop();

    Node *parent = new Node('\0', left->frequency + right->frequency);
    parent->left = left;
    parent->right = right;

    pq.push(parent);
  }

  return pq.top();
}

// Генерация кодов Хаффмана
void generateHuffmanCodes(Node *root,
                          std::unordered_map<char, std::string> &codes,
                          const std::string &code) {
  if (!root)
    return;

  if (!root->left && !root->right) {
    codes[root->character] = code;
  }

  generateHuffmanCodes(root->left, codes, code + "0");
  generateHuffmanCodes(root->right, codes, code + "1");
}

// Освобождение памяти дерева
void freeTree(Node *root) {
  if (!root)
    return;

  freeTree(root->left);
  freeTree(root->right);
  delete root;
}

// Функция декодирования
std::string decodeHuffman(const std::string &encoded, Node *root) {
  std::string decoded;
  Node *current = root;
  for (char bit : encoded) {
    if (bit == '0')
      current = current->left;
    else
      current = current->right;

    if (!current->left && !current->right) {
      decoded += current->character;
      current = root;
    }
  }
  return decoded;
}

void testHuffman(const std::string &input, bool debug = false) {
  std::cout << "Задана строка: " << input << std::endl;

  // Построение дерева Хаффмана
  Node *root = buildHuffmanTree(input);

  // Генерация кодов
  std::unordered_map<char, std::string> codes;
  generateHuffmanCodes(root, codes, "");

  // Кодирование строки
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

  // Декодирование строки
  std::string decoded = decodeHuffman(encoded, root);

  std::cout << "\nВосстановленная строка: " << decoded << std::endl;

  // Проверка корректности
  if (input == decoded) {
    std::cout << "Декодирование успешно." << std::endl;
    } else {
        std::cout << "Декодирование не удалось." << std::endl;
    }

    // Освобождение памяти
    freeTree(root);
}

int main() {
    std::string input = "Ана-дэус-рики-паки, Дормы-кормыконсту-таки, Энус-дэус-кана-дэус-БАЦ!";
    std::string input1 = "aboba aboba aboba aboba aboba aboba aboba aboba aboba aboba aboba";
    testHuffman(input, true);

    return 0;
}
