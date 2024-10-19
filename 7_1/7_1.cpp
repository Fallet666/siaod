#include <algorithm>
#include <iostream>

// Структура узла дерева
struct Node {
  int key;
  Node *left;
  Node *right;
  int height;

  explicit Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

// Функция для получения высоты узла
int height(const Node *node) { return node ? node->height : 0; }

// Функция для вычисления баланса узла
int getBalance(const Node *node) {
  return node ? height(node->left) - height(node->right) : 0;
}

// Поворот вправо
Node *rotateRight(Node *y) {
  Node *x = y->left;
  Node *T2 = x->right;

  // Выполнение поворота
  x->right = y;
  y->left = T2;

  // Обновление высот
  y->height = std::max(height(y->left), height(y->right)) + 1;
  x->height = std::max(height(x->left), height(x->right)) + 1;

  return x;
}

// Поворот влево
Node *rotateLeft(Node *x) {
  Node *y = x->right;
  Node *T2 = y->left;

  // Выполнение поворота
  y->left = x;
  x->right = T2;

  // Обновление высот
  x->height = std::max(height(x->left), height(x->right)) + 1;
  y->height = std::max(height(y->left), height(y->right)) + 1;

  return y;
}

// Вставка элемента в АВЛ-дерево с балансировкой
Node *insert(Node *node, int key) {
  if (!node)
    return new Node(key);

  if (key < node->key) {
    node->left = insert(node->left, key);
  } else if (key > node->key) {
    node->right = insert(node->right, key);
  } else {
    return node; // Дубликаты не допускаются
  }

  node->height = 1 + std::max(height(node->left), height(node->right));

  int balance = getBalance(node);

  // Левый Левый случай
  if (balance > 1 && key < node->left->key) {
    return rotateRight(node);
  }

  // Правый Правый случай
  if (balance < -1 && key > node->right->key) {
    return rotateLeft(node);
  }

  // Левый Правый случай
  if (balance > 1 && key > node->left->key) {
    node->left = rotateLeft(node->left);
    return rotateRight(node);
  }

  // Правый Левый случай
  if (balance < -1 && key < node->right->key) {
    node->right = rotateRight(node->right);
    return rotateLeft(node);
  }

  return node;
}

// Симметричный обход (in-order)
void inOrder(Node *root) {
  if (root) {
    inOrder(root->left);
    std::cout << root->key << " ";
    inOrder(root->right);
  }
}

// Обратный обход (post-order)
void postOrder(Node *root) {
  if (root) {
    postOrder(root->left);
    postOrder(root->right);
    std::cout << root->key << " ";
  }
}

// Нахождение суммы значений листьев
int sumOfLeaves(Node *root) {
  if (!root)
    return 0;

  if (!root->left && !root->right) {
    return root->key;
  }

  return sumOfLeaves(root->left) + sumOfLeaves(root->right);
}

// Нахождение высоты дерева
int treeHeight(Node *root) { return height(root); }

void printTree(Node *root, int space = 0, int height = 10) {
  // Увеличиваем расстояние между уровнями
  int count = height;

  if (root == nullptr)
    return;

  // Увеличиваем расстояние между уровнями
  space += count;

  // Рисуем правое поддерево
  printTree(root->right, space);

  // Выводим текущий узел после отступа
  std::cout << std::endl;
  for (int i = count; i < space; i++)
    std::cout << " ";
  std::cout << root->key << "\n";

  // Рисуем левое поддерево
  printTree(root->left, space);
}

// Меню для работы с деревом
void menu(Node *&root) {
  int choice;
  do {
    std::cout << "1. Вставить элемент\n";
    std::cout << "2. Симметричный обход (in-order)\n";
    std::cout << "3. Обратный обход (post-order)\n";
    std::cout << "4. Нахождение суммы значений листьев\n";
    std::cout << "5. Нахождение высоты дерева\n";
    std::cout << "6. Выйти\n";
    std::cout << "7. Напечатать дерево\n";
    std::cout << "Выберите действие: ";
    std::cin >> choice;

    switch (choice) {
    case 1: {
      int key;
      std::cout << "Введите ключ для вставки: ";
      std::cin >> key;
      root = insert(root, key);
      break;
    }
    case 2:
      std::cout << "Симметричный обход: ";
      inOrder(root);
      std::cout << std::endl;
      break;
    case 3:
      std::cout << "Обратный обход: ";
      postOrder(root);
      std::cout << std::endl;
                break;
            case 4:
                std::cout << "Сумма значений листьев: " << sumOfLeaves(root) << std::endl;
                break;
            case 5:
                std::cout << "Высота дерева: " << treeHeight(root) << std::endl;
                break;
            case 6:
                std::cout << "Выход..." << std::endl;
                break;
            case 7:
                printTree(root);
                break;
            default:
                std::cout << "Некорректный ввод, попробуйте снова." << std::endl;
                break;
        }
    } while (choice != 6);
}

int main() {
    Node* root = nullptr;
    menu(root);
    return 0;
}
