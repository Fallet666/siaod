#include <iostream>
#include <optional>
#include <string>
#include <vector>

struct Enterprise {
  int licenseNumber;   // Номер лицензии (ключ)
  std::string name;    // Название предприятия
  std::string founder; // Учредитель
};

class HashTable {
private:
  std::vector<std::optional<Enterprise>> table;
  int tableSize;
  int count; // Текущие элементы

  // Первая хеш-функция
  [[nodiscard]] int hash1(int key) const { return key % tableSize; }

  // Вторая хеш-функция для двойного хеширования
  [[nodiscard]] int hash2(int key) const { return 1 + (key % (tableSize - 1)); }

  // Расширение таблицы и рехеширование
  void rehash() {
    std::cout << "Rehashing...\n";
    std::vector<std::optional<Enterprise>> oldTable = table;
    tableSize *= 2;
    table = std::vector<std::optional<Enterprise>>(tableSize);
    count = 0;

    for (auto &entry : oldTable) {
      if (entry) {
        insert(entry->licenseNumber, entry->name, entry->founder);
      }
    }
  }

public:
  explicit HashTable(int size) : tableSize(size), count(0) {
    table.resize(size);
  }

  // Вставка нового элемента
  void insert(int licenseNumber, const std::string &name,
              const std::string &founder) {
    if (count >= tableSize * 0.75) {
      rehash();
    }

    int index = hash1(licenseNumber);
    int step = hash2(licenseNumber);

    for (int i = 0; i < tableSize; ++i) {
      int newIndex = (index + i * step) % tableSize;
      if (!table[newIndex] ||
          table[newIndex]->licenseNumber ==
              -1) { // Если ячейка пустая или помечена как удаленная
        table[newIndex] = Enterprise{licenseNumber, name, founder};
        ++count;
        std::cout << "Inserted: " << licenseNumber << " at index " << newIndex
                  << "\n";
        return;
      }
    }

    std::cerr << "Error: Table is full, cannot insert.\n";
  }

  // Поиск элемента по ключу (номеру лицензии)
  [[nodiscard]] std::optional<Enterprise> search(int licenseNumber) const {
    int index = hash1(licenseNumber);
    int step = hash2(licenseNumber);

    for (int i = 0; i < tableSize; ++i) {
      int newIndex = (index + i * step) % tableSize;
      if (table[newIndex] && table[newIndex]->licenseNumber == licenseNumber) {
        return table[newIndex];
      } else if (!table[newIndex]) {
        return std::nullopt;
      }
    }

    return std::nullopt;
  }

  // Удаление элемента по ключу
  void remove(int licenseNumber) {
    int index = hash1(licenseNumber);
    int step = hash2(licenseNumber);

    for (int i = 0; i < tableSize; ++i) {
      int newIndex = (index + i * step) % tableSize;
      if (table[newIndex] && table[newIndex]->licenseNumber == licenseNumber) {
        table[newIndex]->licenseNumber = -1; // Помечаем как удалённый
        std::cout << "Removed: " << licenseNumber << " from index " << newIndex
                  << "\n";
        --count;
        return;
      } else if (!table[newIndex]) {
        std::cout << "License number " << licenseNumber << " not found.\n";
        return;
      }
    }
  }

  // Вывод таблицы
  void print() const {
    for (int i = 0; i < tableSize; ++i) {
      if (table[i] && table[i]->licenseNumber != -1) {
        std::cout << i << ": " << table[i]->licenseNumber << ", "
                  << table[i]->name << ", " << table[i]->founder << "\n";
      } else {
        std::cout << i << ": [Empty]\n";
      }
    }
  }

  void autoFill() {
    insert(1001, "Enterprise A", "Founder A");
    insert(1010, "Enterprise B", "Founder B");
    insert(1100, "Enterprise C", "Founder C");
    insert(1200, "Enterprise D", "Founder D");
    insert(1300, "Enterprise E", "Founder E");
  }
};

void showMenu() {
  std::cout << "Commands:\n";
  std::cout << "1. insert <license_number> <name> <founder> - Insert new "
               "enterprise\n";
  std::cout << "2. search <license_number> - Search for enterprise by license "
               "number\n";
  std::cout
      << "3. remove <license_number> - Remove enterprise by license number\n";
  std::cout << "4. print - Print the entire hash table\n";
  std::cout << "5. exit - Exit the program\n";
}

void commandLoop(HashTable &ht) {
  std::string command;
  int licenseNumber;
  std::string name, founder;

  while (true) {
    showMenu();
    std::cout << "Enter command: ";
    std::cin >> command;

    if (command == "insert") {
      std::cout << "Enter license number, name, and founder: ";
      std::cin >> licenseNumber >> name >> founder;
      ht.insert(licenseNumber, name, founder);
    } else if (command == "search") {
      std::cout << "Enter license number: ";
      std::cin >> licenseNumber;
      auto result = ht.search(licenseNumber);
      if (result) {
        std::cout << "Found: " << result->licenseNumber << ", " << result->name
                  << ", " << result->founder << "\n";
      } else {
        std::cout << "License number not found.\n";
      }
    } else if (command == "remove") {
      std::cout << "Enter license number: ";
      std::cin >> licenseNumber;
      ht.remove(licenseNumber);
    } else if (command == "print") {
      ht.print();
    } else if (command == "exit") {
      break;
        } else {
            std::cout << "Unknown command.\n";
        }
    }
}


int main() {
    HashTable ht(7);  // Начальный размер таблицы

    // Автоматическое заполнение таблицы
    ht.autoFill();

    // Запуск командного интерфейса
    commandLoop(ht);

    return 0;
}
