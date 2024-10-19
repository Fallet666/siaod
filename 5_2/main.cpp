#include <algorithm> // Для std::shuffle
#include <fstream>
#include <iostream>
#include <random> // Для std::default_random_engine
#include <vector>

const std::string RESET = "\033[0m";
const std::string GREEN = "\033[32m";

struct CityRecord {
  int cityCode;       // Код города
  char cityName[100]; // Название города
};

void createBinaryFile(const std::string &filename, int numRecords) {
  std::ofstream outFile(filename, std::ios::binary);

  if (!outFile) {
    std::cerr << "Error opening file for writing!" << std::endl;
    return;
  }

  // Создаем вектор уникальных ключей
  std::vector<int> keys(numRecords);
  for (int i = 0; i < numRecords; ++i) {
    keys[i] = i; // Используем i в качестве уникального кода города
  }

  // Перемешиваем ключи
  std::random_device rd;
  std::default_random_engine rng(rd());
  std::shuffle(keys.begin(), keys.end(), rng);

  // Записываем записи в бинарный файл
  for (int i = 0; i < numRecords; ++i) {
    CityRecord record{};
    record.cityCode = keys[i];
    snprintf(record.cityName, sizeof(record.cityName), "City%d", i);

    outFile.write(reinterpret_cast<char *>(&record), sizeof(record));
  }

  outFile.close();
}

void dumpBinaryToText(const std::string &binaryFilename,
                      const std::string &textFilename) {
  std::ifstream inFile(binaryFilename, std::ios::binary);
  std::ofstream outFile(textFilename);

  if (!inFile || !outFile) {
    std::cerr << "Error opening file for reading/writing!" << std::endl;
    return;
  }

  CityRecord record{};
  while (inFile.read(reinterpret_cast<char *>(&record), sizeof(record))) {
    outFile << record.cityCode << ", " << record.cityName << std::endl;
  }

  inFile.close();
  outFile.close();
}

void test_creating() {
  const std::string binaryFilename = "../5_2/city_data.bin";
  createBinaryFile(binaryFilename, 100);
  dumpBinaryToText(binaryFilename, "../5_2/view.txt");
}

CityRecord *linearSearch(const std::string &filename, int key) {
  std::ifstream inFile(filename, std::ios::binary);
  if (!inFile) {
    std::cerr << "Error opening file for reading!" << std::endl;
    return nullptr;
  }

  CityRecord record{};
  while (inFile.read(reinterpret_cast<char *>(&record), sizeof(record))) {
    if (record.cityCode == key) {
      inFile.close();
      auto *result = new CityRecord(record);
      return result;
    }
  }

  inFile.close();
  return nullptr;
}

void test_linear(int numRecords) {
  const std::string binaryFilename = "../5_2/city_data.bin";
  const int keyToFind = 1;
  createBinaryFile(binaryFilename, numRecords);

  auto start = std::chrono::high_resolution_clock::now();
  CityRecord *result = linearSearch(binaryFilename, keyToFind);
  auto end = std::chrono::high_resolution_clock::now();

  if (result) {
    std::cout << "Record found: Code = " << result->cityCode
              << ", Name = " << result->cityName << std::endl;
    delete result;
  } else {
    std::cout << "Record not found." << std::endl;
  }

  std::chrono::duration<double, std::milli> duration = end - start;
  std::cout << "Time taken: " << duration.count() << " milliseconds"
            << std::endl;
}

void temp_test_linear() {
  for (int i = 100; i <= 10000; i *= 10) {
    test_linear(i);
    std::cout << "\n";
  }
}

// Функция для создания таблицы смещений
std::unordered_map<int, std::streampos>
createOffsetTable(const std::string &filename) {
  std::ifstream inFile(filename, std::ios::binary);
  std::unordered_map<int, std::streampos> offsetTable;

  if (!inFile) {
    std::cerr << "Error opening file for reading!" << std::endl;
    return offsetTable;
  }

  CityRecord record{};
  std::streampos offset = 0;
  while (inFile.read(reinterpret_cast<char *>(&record), sizeof(record))) {
    offsetTable[record.cityCode] = offset;
    offset += sizeof(record);
  }

  inFile.close();
  return offsetTable;
}

int fibonacciSearchInTable(
    const std::unordered_map<int, std::streampos> &offsetTable,
    const std::vector<int> &sortedKeys, int key) {
  int n = sortedKeys.size();
  int fibMMm2 = 0;              // (m-2)'th Fibonacci Number
  int fibMMm1 = 1;              // (m-1)'th Fibonacci Number
  int fibM = fibMMm1 + fibMMm2; // m'th Fibonacci Number

  while (fibM < n) {
    fibMMm2 = fibMMm1;
    fibMMm1 = fibM;
    fibM = fibMMm1 + fibMMm2;
  }

  int offset = -1;

  while (fibM > 1) {
    int i = std::min(offset + fibMMm2, n - 1);

    if (sortedKeys[i] < key) {
      fibM = fibMMm1;
      fibMMm1 = fibMMm2;
      fibMMm2 = fibM - fibMMm1;
      offset = i;
    } else if (sortedKeys[i] > key) {
      fibM = fibMMm2;
      fibMMm1 = fibMMm1 - fibMMm2;
      fibMMm2 = fibM - fibMMm1;
    } else {
      return i;
    }
  }

  if (fibMMm1 && sortedKeys[offset + 1] == key) {
    return offset + 1;
  }

  return -1;
}

CityRecord *searchWithOffsetTable(
    const std::string &filename,
    const std::unordered_map<int, std::streampos> &offsetTable,
    const std::vector<int> &sortedKeys, int key) {
  int index = fibonacciSearchInTable(offsetTable, sortedKeys, key);
  if (index == -1) {
    return nullptr; // Ключ не найден в таблице смещений
  }

  auto it = offsetTable.find(sortedKeys[index]);
  if (it == offsetTable.end()) {
    return nullptr;
  }

  std::ifstream inFile(filename, std::ios::binary);
  if (!inFile) {
    std::cerr << "Error opening file for reading!" << std::endl;
    return nullptr;
  }

  CityRecord record{};
  inFile.seekg(it->second); // Перемещаемся к нужному смещению
  inFile.read(reinterpret_cast<char *>(&record), sizeof(record));

  inFile.close();
  auto *result = new CityRecord(record);
  return result;
}

void test_fibonacciSearch(int numRecords) {

  const std::string binaryFilename = "../5_2/city_data.bin";
  createBinaryFile(binaryFilename, numRecords);
  const int keyToFind = 0;

  // Создаем таблицу смещений
  auto offsetTable = createOffsetTable(binaryFilename);

  // Создаем отсортированный вектор ключей из таблицы смещений
  std::vector<int> sortedKeys;
  sortedKeys.reserve(offsetTable.size());
  for (const auto &entry : offsetTable) {
    sortedKeys.push_back(entry.first);
  }
  std::sort(sortedKeys.begin(), sortedKeys.end());

  auto start = std::chrono::high_resolution_clock::now();
  CityRecord *result =
      searchWithOffsetTable(binaryFilename, offsetTable, sortedKeys, keyToFind);
  auto end = std::chrono::high_resolution_clock::now();

  if (result) {
    std::cout << "Record found: Code = " << result->cityCode
              << ", Name = " << result->cityName << std::endl;
    delete result;
  } else {
    std::cout << "Record not found." << std::endl;
  }

  // Измерение времени в миллисекундах
  std::chrono::duration<double, std::milli> duration = end - start;
  std::cout << "Time taken: " << duration.count() << " milliseconds"
            << std::endl;
}

void temp_test_fibonacciSearch() {
    for(int i=100; i<=10000; i*=10) {
        test_fibonacciSearch(i);
        std::cout<<"\n";
    }
}



int main() {
    std::cout<<GREEN<<"Linear search test's \n"<<RESET;
    temp_test_linear();
    std::cout<<GREEN<<"Fibonacci search test's \n"<<RESET;
    temp_test_fibonacciSearch();
    // test_creating();
}
